#include "../common.hpp"
#include "Loader.hpp"
#include "proc.hpp"

void* gShellCodePtr = NULL;
void* gStackPtr = NULL;
size_t gShellCodeSize = 0;
bool ShellCodeLoaded = false;
char ProcName[0x20] = { };

ssize_t klog(char* fmt, ...)
{
    char buffer[0x400] = { "TEST" };
	va_list args;
	va_start(args, fmt);
	vsprintf(buffer, fmt, args);

	struct sys_read_args {
		uint64_t unk;
		uint64_t msg;
		uint64_t unk2;
	};

    sysent* sysents = sysvec->sv_table;

    auto sys_read = (int(*)(thread * td, sys_read_args * uap))sysents[601].sy_call;

	thread *td = curthread();

    td->td_retval[0] = 0;

	sys_read_args uap;
	uap.unk = 7;
	uap.msg = (uint64_t)buffer;
	uap.unk2 = 0;

	int errorno = sys_read(td, &uap);

    va_end(args);

	if(errorno)
		return -errorno;

	return (ssize_t)td->td_retval[0];
}

extern uint8_t LoaderShellCode[];
extern int32_t LoaderShellCodeSize;

bool Loader_Init(const char* Proc_Name)
{
    klog("Loader Init\n");
    size_t n;
    int err = 0;
    proc* proc = proc_find_by_name(Proc_Name);
    uint64_t thr_initial = 0;

    if(!proc) 
	{
        klog("Could not find Proc \"%s\".\n", Proc_Name);
        return false;
    }

    //store for use later
    strcpy(ProcName, (char*)Proc_Name);

    gShellCodeSize = LoaderShellCodeSize;
	gShellCodeSize += (PAGE_SIZE - (gShellCodeSize % PAGE_SIZE));
	if(proc_allocate(proc, &gShellCodePtr, gShellCodeSize)) 
    {
        klog("Failed to allocate ShellCode Memory.\n");
        return false;
    }

    size_t StackSize = 0x80000;
	if(proc_allocate(proc, &gStackPtr, StackSize)) {
        klog("Failed to allocate Stack Memory.\n");

        if (gShellCodePtr)
			proc_deallocate(proc, gShellCodePtr, gShellCodeSize);

        return false;
    }

    err = proc_rw_mem(proc, gShellCodePtr, LoaderShellCodeSize, (void *)LoaderShellCode, &n, 1);
    if(err)
    {
        klog("Failed to write Shellcode to Memory. Error: %d.\n", err);

        if (gShellCodePtr)
			proc_deallocate(proc, gShellCodePtr, gShellCodeSize);

		if (gStackPtr)
			proc_deallocate(proc, gStackPtr, 0x80000);

        return false;
    }

    if(proc->p_dynlibptr == NULL) 
    {
        klog("p_dynlibptr is NULL.\n");

        if (gShellCodePtr)
			proc_deallocate(proc, gShellCodePtr, gShellCodeSize);

		if (gStackPtr)
			proc_deallocate(proc, gStackPtr, 0x80000);

        return false;
    }

    dynlib* m_library = proc->p_dynlibptr->p_dynlib;
    while(m_library != 0)
	{
        if(!strcmp(basename(m_library->ModulePath), "libkernel.sprx"))
			thr_initial = (uint64_t)m_library->codeBase + addr_thr_initial_libkernel;

        if(!strcmp(basename(m_library->ModulePath), "libkernel_web.sprx"))
			thr_initial = (uint64_t)m_library->codeBase + addr_thr_initial_libkernel_web;

        if(!strcmp(basename(m_library->ModulePath), "libkernel_sys.sprx"))
			thr_initial = (uint64_t)m_library->codeBase + addr_thr_initial_libkernel_sys;

        m_library = m_library->dynlib_next;
    }

    if(thr_initial == 0) 
    {
		klog("Failed to resolve thr_initial.\n");

        if (gShellCodePtr)
			proc_deallocate(proc, gShellCodePtr, gShellCodeSize);

		if (gStackPtr)
			proc_deallocate(proc, gStackPtr, 0x80000);

        return false;
    }

    m_library = proc->p_dynlibptr->p_dynlib;
    while(m_library != 0)
	{ 
        proc_mprotect(proc, (void *)m_library->codeBase, (void*)m_library->codeSize, VM_PROT_ALL);
        m_library = m_library->dynlib_next;
    }

    klog("thr_initial = %llX\n", thr_initial);
    klog("gShellCodePtr = %llX\n", gShellCodePtr);

    err = proc_rw_mem(proc, gShellCodePtr + offsetof(OrbisProcHelper_header, thr_initial), sizeof(thr_initial), (void *)&thr_initial, &n, 1);
    if(err)
    {
        klog("Failed to write thr_initial to ShellCode. Error: %d.\n", err);

        if (gShellCodePtr)
			proc_deallocate(proc, gShellCodePtr, gShellCodeSize);

		if (gStackPtr)
			proc_deallocate(proc, gStackPtr, 0x80000);

        return false;
    }

    klog("Starting Shellcode Thread...\n");
    struct thread *thr = TAILQ_FIRST(&proc->p_threads);
	uint64_t ShellCodeEntry = (uint64_t)gShellCodePtr + *(uint64_t *)(LoaderShellCode + 4);
	create_thread(thr, NULL, (void*)ShellCodeEntry, NULL, (char*)gStackPtr, StackSize, NULL, NULL, NULL, 0, NULL);

    klog("!! Shellcode Loaded Success !!\n");
    return ShellCodeLoaded = true;
}

int Load_SPRX(const char* SPRX_Path)
{
    if(!ShellCodeLoaded)
    {
        klog("Loader not initialized.\n");
        return 0;
    }

    proc* proc = proc_find_by_name(ProcName);
    
    if(!proc) 
	{
        klog("Could not find Proc \"%s\".\n", ProcName);
        return 0;
    }

    klog("gShellCodePtr = %llX\n", gShellCodePtr);

    
    Backup_Jail bkjail;
    Jailbreak(proc, &bkjail);

    size_t n = 0;
    int err = 0;
	uint8_t CommandIndex = CMD_sceKernelLoadStartModule;
    uint8_t ShellCodeComplete = 0;
    uint64_t ModuleHandle = 0;

    err = proc_rw_mem(proc, gShellCodePtr + offsetof(OrbisProcHelper_header, SPRXPath), strlen(SPRX_Path), (void *)SPRX_Path, &n, 1);
    if(err)
    {
        klog("Failed to write params to ShellCode.\n");
        return 0;
    }

    err = proc_rw_mem(proc, gShellCodePtr + offsetof(OrbisProcHelper_header, ModuleHandle), sizeof(ModuleHandle), (void *)&ModuleHandle, &n, 1);
	if(err)
    {
        klog("Failed to reset ModuleHandle to zero.\n");
        return 0;
    }

    err = proc_rw_mem(proc, gShellCodePtr + offsetof(OrbisProcHelper_header, ShellCodeComplete), sizeof(ShellCodeComplete), (void *)&ShellCodeComplete, &n, 1);
	if(err)
    {
        klog("Failed to set ShellCodeComplete to zero.\n");
        return 0;
    }

    err = proc_rw_mem(proc, gShellCodePtr + offsetof(OrbisProcHelper_header, CommandIndex), sizeof(CommandIndex), (void *)&CommandIndex, &n, 1);
	if(err)
    {
        klog("Failed to set CommandIndex.\n");
        return 0;
    }

    while (!ShellCodeComplete) 
	{
        err = proc_rw_mem(proc, gShellCodePtr + offsetof(OrbisProcHelper_header, ShellCodeComplete), sizeof(ShellCodeComplete), (void *)&ShellCodeComplete, &n, 0);
        if(err)
        {
            klog("Failed to read ShellCodeComplete.\n");
            return 0;
        }

        klog("Waiting for ShellCode to compelete!\n");
        pause("", 100);
	}

    err = proc_rw_mem(proc, gShellCodePtr + offsetof(OrbisProcHelper_header, ModuleHandle), sizeof(ModuleHandle), (void *)&ModuleHandle, &n, 0);
    if(err)
    {
        klog("Failed to read ModuleHandle.\n");
        return 0;
    }

    RestoreJail(proc, bkjail);

    klog("Module Handle = %i\n", ModuleHandle);

	return (int)(ModuleHandle);
}
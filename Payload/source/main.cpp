#include "common.hpp"
#include "Util/Loader.hpp"

extern uint8_t LoaderShellCode[];
extern int32_t LoaderShellCodeSize;

struct payloadArgs {
	uint64_t sycall;
	void *payload;
	size_t psize;
};

int run_loader(struct thread *td, struct payloadArgs *args)
{
	uint64_t KernelBase = (__readmsr(0xC0000082) - addr_Xfast_syscall);

	Resolve(KernelBase);

	if(!Loader_Init("SceShellUI"))
		return 1;

	int Handle = Load_SPRX("/data/Orbis Toolbox.sprx");

	if(Handle <= 0)
		return 2;

    return 0;
}

extern "C" int _main(uint64_t* p)
{
	syscall(601, 7, "Hello World.\n", 0);

    syscall(11, run_loader, LoaderShellCode, LoaderShellCodeSize);
	
	return 0;
}
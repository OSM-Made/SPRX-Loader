#include "common.h"

extern uint8_t LoaderShellCode[];
extern int32_t LoaderShellCodeSize;

struct payloadArgs {
	uint64_t sycall;
	void *payload;
	size_t psize;
};

int run_loader(struct thread *td, struct payloadArgs *args)
{
    const char* SPRX_Name = "Mono-Menu.sprx";

    return 0;
}

int _main(void) 
{
    syscall(11, run_loader, LoaderShellCode, LoaderShellCodeSize);
	
	return 0;
}
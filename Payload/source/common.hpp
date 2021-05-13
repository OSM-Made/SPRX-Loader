#pragma once

#include <stdint.h>
#include <stdarg.h>

#include "Util/settings.hpp"
#include "Util/types.hpp"
#include "Util/syscall.hpp"
#include "Util/Utils.hpp"
#include "resolver/resolver.hpp"

extern "C"
{
    #include <sys/sysent.h>
    #include <sys/proc.h>
    #include <sys/syscall.h>
    #include <sys/ptrace.h>
    #include <sys/uio.h>
    
    #include <sys/mman.h>
    #include <sys/pcpu.h>
    #include <vm/vm.h>
    #include <vm/pmap.h>
    #include <vm/vm_map.h>

    /*#include <sys/sysent.h>

    #include <sys/lock.h>
    #include <sys/mutex.h>

    
    
    
    #include <sys/mman.h>
    #include <sys/pcpu.h>
    #include <vm/vm.h>
    #include <vm/pmap.h>
    #include <vm/vm_map.h>

    #include <errno.h>
    #include <sys/elf_common.h>
    #include <sys/elf64.h>
    #include <sys/eventhandler.h>
    #include <wait.h>
    #include <machine/reg.h>
    #include <fcntl.h>
    #include <time.h>
    #include <dirent.h>

    #include <sys/stat.h>
    #include "sys/mount.h"*/
}
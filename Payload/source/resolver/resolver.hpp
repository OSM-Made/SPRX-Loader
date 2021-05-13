#pragma once

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
}

#include "resolver-505.hpp"
#include "resolver-672.hpp"
#include "resolver-702.hpp"
#include "resolver-755.hpp"

/* Util */
extern sysentvec* sysvec;
extern prison* prison0;
extern vnode* rootvnode;
extern void (*pause)(const char *wmesg, int timo);

/* STD Lib */
extern void *M_TEMP;
extern void* M_MOUNT;
extern void *(*malloc)(unsigned long size, void *type, int flags);
extern void (*free)(void *addr, void *type);
extern void (*memcpy)(void *dst, const void *src, size_t len);
extern void *(*memset)(void *ptr, int value, size_t num);
extern int (*memcmp)(const void *ptr1, const void *ptr2, size_t num);
extern size_t (*strlen)(const char *str);
extern int (*strcpy)(char * str1, char * str2);
extern char* (*strncpy)(char *destination, const char *source, size_t num);
extern int (*strcmp)(const char * str1, const char * str2);
extern char* (*strstr)(const char * str1, const char * str2);
extern int (*sprintf)(char* dst, const char *fmt, ...);
extern int (*snprintf)(char *str, size_t size, const char *format, ...);
extern int (*vsprintf)(char* dst, const char* fmt, va_list ap);
extern int (*vprintf)(const char *fmt, va_list arg);
extern int(*sscanf)(const char *str, const char *format, ...);
extern char *(*strdup)(const char *s, void* type);
extern char *(*realloc)(void *addr, unsigned long size, void* mtp, int flags);
extern void(*kprintf)(const char* fmt, ...);

/* Virtual Memory */
extern vmspace *(*vmspace_acquire_ref)(proc* p);
extern void (*vmspace_free)(vmspace* vm);
extern void (*vm_map_lock_read)(vm_map* map);
extern void (*vm_map_unlock_read)(vm_map* map);
extern int (*vm_map_lookup_entry)(vm_map* map, uint64_t address, vm_map_entry **entries);
extern int (*vm_map_findspace)(vm_map* map, uint64_t start, uint64_t length, uint64_t *addr);
extern int (*vm_map_insert)(vm_map* map, uint64_t object, uint64_t offset, uint64_t start, uint64_t end, int prot, int max, int cow);
extern void (*vm_map_lock)(vm_map* map);
extern void (*vm_map_unlock)(vm_map* map);
extern int (*vm_map_delete)(vm_map* map, uint64_t start, uint64_t end);
extern int (*vm_map_protect)(vm_map* map, uint64_t start, uint64_t end, int new_prot, uint64_t set_max);

/* Proc */
extern proc *allproc;
extern int (*proc_rwmem)(proc *p, uio *uio);
extern int (*create_thread)(thread * td, uint64_t ctx, void* start_func, void *arg, char *stack_base, size_t stack_size, char *tls_base, long * child_tid, long * parent_tid, uint64_t flags, uint64_t rtp);

void Resolve(uint64_t kernbase);
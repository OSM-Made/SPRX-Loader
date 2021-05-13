#include "../common.hpp"
#include "Utils.hpp"

char* strrchr(const char *cp, int ch)
{
    char *save;
    char c;

    for (save = (char *) 0; (c = *cp); cp++) {
	if (c == ch)
	    save = (char *) cp;
    }

    return save;
}

char* strchr(const char *s, int c)
{
  do {
    if (*s == c)
      {
        return (char*)s;
      }
  } while (*s++);
  return (0);
}

char* basename(const char *filename)
{
  char *p = strrchr(filename, '/');
  return p ? p + 1 : (char *)filename;
}

void Jailbreak(proc* proc, Backup_Jail* jail)
{
    if(proc)
    {
        ucred* cred = proc->p_ucred;
        filedesc* fd = proc->p_fd;

        if(!cred || !fd)
            return;

        if(jail)
        {
            jail->cr_prison = cred->cr_prison;
            jail->cr_uid = cred->cr_uid;
            jail->cr_ruid = cred->cr_ruid;
            jail->cr_rgid = cred->cr_rgid;
            jail->cr_groups = cred->cr_groups[0];

            jail->fd_jdir = fd->fd_jdir;
            jail->fd_rdir = fd->fd_rdir;
        }
        
        cred->cr_prison = *(prison**)prison0;

        cred->cr_uid = 0;
        cred->cr_ruid = 0;
        cred->cr_rgid = 0;
        cred->cr_groups[0] = 0;

    
        fd->fd_jdir = *(vnode**)rootvnode;
        fd->fd_rdir = *(vnode**)rootvnode;
    }
}

void RestoreJail(proc* proc, Backup_Jail jail)
{
    if(proc)
    {
        ucred* cred = proc->p_ucred;
        filedesc* fd = proc->p_fd;

        if(!cred || !fd)
            return;

        cred->cr_prison = jail.cr_prison;
        cred->cr_uid = jail.cr_uid;
        cred->cr_ruid = jail.cr_ruid;
        cred->cr_rgid = jail.cr_rgid;
        cred->cr_groups[0] = jail.cr_groups;

        fd->fd_jdir = jail.fd_jdir;
        fd->fd_rdir = jail.fd_rdir;
    }
}
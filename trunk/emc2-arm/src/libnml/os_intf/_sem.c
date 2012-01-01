/********************************************************************
* Description: _sem.c
*
*   Derived from a work by Fred Proctor & Will Shackleford
*
* Author:
* License: LGPL Version 2
* System: Linux
*    
* Copyright (c) 2004 All rights reserved.
*
* Last change: 
* $Revision: 1.17 $
* $Author: jepler $
* $Date: 2007/05/15 01:00:29 $
********************************************************************/

#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>		/* errno */
#include <string.h>		/* strerror() */
#include <stdarg.h>		/* va_list, va_arg(), va_start(), va_end() */
#include <sys/types.h>
#include <sys/ipc.h>		/* IPC_CREATE, IPC_NOWAIT */
#include <linux/version.h>

/* There are two types of posix semaphores named and unnamed.
   unamed semaphores can either have the pshared flag set or not
   determining whether it can be shared between processes.
   Currently (12/27/02), Linux implements only unnamed posix semaphores
   that are not shared between processes. This is useless to RCSLIB so
   on Linux System V semaphores will be used instead.
*/

#include <sys/sem.h>		/* struct sembuf */
#include <math.h>		/* fmod() */
#include <signal.h>
#include <sys/time.h>

typedef int rcs_sem_t;
#define rcs_sem_t_defined

#include "_sem.h"
#include "_timer.h"		/* etime() */
#include "rcs_print.hh"

#define SEM_TAKE (-1)		/* decrement sembuf.sem_op */
#define SEM_GIVE (1)		/* increment sembuf.sem_op */

#ifdef _SEM_SEMUN_UNDEFINED
/* The user should define a union like the following to use it for arguments
   for `semctl'. Previous versions of bits/sem.h used to define this union 
   but this is incorrect.  One can test the macro _SEM_SEMUN_UNDEFINED to 
   see whether one must define the union or not.  */

union semun {
    int val;			/* value for SETVAL */
    struct semid_ds *buf;	/* buffer for IPC_STAT, IPC_SET */
    unsigned short int *array;	/* array for GETALL, SETALL */
    struct seminfo *__buf;	/* buffer for IPC_INFO */
};

#endif

/* remove semaphore from OS-- this must be done *before* sem_close,
   since rcs_sem_close frees the storage allocated for the rcs_sem_t */
int rcs_sem_destroy(rcs_sem_t * sem)
{
    /* remove OS semaphore */
    if (semctl(*sem, 0, IPC_RMID) == -1) {
	rcs_print_error("semctl(%d,0,%d) failed: (errno = %d) %s\n",
	    *sem, IPC_RMID, errno, strerror(errno));
	return -1;
    }
    return 0;
}

int sem_clear_bus_errors = 0;
void sem_clear_bus_error_handler(int sig)
{
    sem_clear_bus_errors++;
}

int rcs_sem_clear(rcs_sem_t * sem)
{
    union semun sem_arg;
    sem_arg.val = 1;
    semctl(*sem, 1, SETVAL, sem_arg);
    return (0);
}

static int rcs_sem_open_val = 0;

/* create a named binary semaphore */
rcs_sem_t *rcs_sem_open(const char *name, int oflag, /* int mode */ ...)
{
    va_list ap;
    int mode;			/* optional last arg */
    key_t key;			/* name converted to a key */
    rcs_sem_t semid, *retval;	/* semaphore id returned */
    int semflg = 0;		/* flag for perms, create, etc. */

    /* if IPC_CREAT is specified for creating the sempahore, then the
       optional arg is the mode */
    if (oflag & IPC_CREAT) {
	va_start(ap, oflag);
	mode = va_arg(ap, int);
	va_end(ap);
	semflg |= mode;
	semflg |= IPC_CREAT;
    } else {
	semflg &= ~IPC_CREAT;
    }

    /* change the char* name to a key_t-- this is impossible, so let's just
       consider the value of the char* as a key_t */
    /* FIX-ME semid is a number, not a name. Get rid of this fsck'd ... */
    key = (key_t) name;		/* ugh */
    if (key < 1) {
	rcs_print_error("rcs_sem_open: invalid key %d\n", key);
	return NULL;
    }

    if ((semid = (rcs_sem_t) semget((key_t) key, 1, semflg)) == -1) {
	rcs_print_error("semget");
	rcs_puts((char *) strerror(errno));
	return NULL;
    }

    /* we have a valid semid-- semantics say we return a pointer to the id,
       so we need to allocate space that users will free later with
       rcs_sem_close */
    retval = (rcs_sem_t *) malloc(sizeof(rcs_sem_t));
    *retval = semid;
    return retval;
}

int rcs_sem_close(rcs_sem_t * sem)
{
    if (sem != 0) {
	free(sem);
    }
    return 0;
}

int rcs_sem_wait_notimeout(rcs_sem_t * sem)
{
    int retval = -1;
    struct sembuf sops;
    union semun sem_arg;
    sem_arg.val = 0;
    sops.sem_num = 0;
    sops.sem_op = SEM_TAKE;
    sops.sem_flg = 0;
    retval = semop(*sem, &sops, 1);
    if (errno == EINTR) {
	rcs_print_debug(PRINT_SEMAPHORE_ACTIVITY, "%s %d semop interrupted\n",
	    __FILE__, __LINE__);
	return retval;
    }

    if (retval == -1) {
	rcs_print_error
	    ("semop(semid=%d, {sem_num=%d,sem_op=%d,sem_flg=%d},nsops=1): ERROR: %s %d\n",
	    *sem, sops.sem_num, sops.sem_op, sops.sem_flg, strerror(errno),
	    errno);
    }

    return retval;
}

int rcs_sem_trywait(rcs_sem_t * sem)
{
    struct sembuf sops;
    sops.sem_num = 0;
    sops.sem_op = SEM_TAKE;
    sops.sem_flg = IPC_NOWAIT;
    return semop(*sem, &sops, 1);
}

#ifndef _GNU_SOURCE
#error Must compile with -D_GNU_SOURCE else impliment your own semtimedop() \
       function. 
#endif

#if !defined (HAVE_SEMTIMEDOP) || LINUX_VERSION_CODE < KERNEL_VERSION(2,4,22)
#undef HAVE_SEMTIMEDOP
void itimer_handler(int signum)
{
#ifdef DEBUG
    rcs_print_error(stderr, "semop timed out\n");
#endif
}
#endif

int rcs_sem_wait(rcs_sem_t * sem, double timeout)
{
#ifdef HAVE_SEMTIMEDOP
    struct timespec time = {1,0};
#else
    struct sigaction sa;
    struct itimerval time;
#endif
#if DEBUG
    double start_time = etime();
    double end_time = 0;
    int error = 0;
#endif
    struct sembuf sops;
    int retval = -1;

    sops.sem_num = 0;
    sops.sem_op = SEM_TAKE;
    sops.sem_flg = 0;
    
    if (0 == sem) {
	return -1;
    }

#ifdef HAVE_SEMTIMEDOP
    if (timeout > 0 ) {
        time.tv_sec = (long int) timeout;
        time.tv_nsec = (long int) ((timeout - time.tv_sec) * 1e9);
    }
    retval = semtimedop(*sem, &sops, 1, &time);
#else
    /* semtimedop was introduced with 2.4.22 kernels, prior to that, we need
       to mess around with timers & signals.. */
    if (timeout > 0 ) {
        memset(&sa, 0, sizeof(sa));
        sa.sa_handler = &itimer_handler;
        sa.sa_flags = SA_ONESHOT;
        /* itimers are limited to three per process, better hope the limit
           is not exceeded. If it is, chances are, the HMI will exhibit
           random lockups */
        time.it_value.tv_sec = (long int) timeout;
        time.it_interval.tv_sec = 0;
        time.it_interval.tv_usec = 0;
        time.it_value.tv_usec = (long int) ((timeout - time.it_value.tv_sec) * 1e6);
        sigaction(SIGALRM, &sa, NULL);
        setitimer(ITIMER_REAL, &time, NULL);
    }
    retval = semop(*sem, &sops, 1);
#endif

#if DEBUG
    error = errno;
    end_time = etime();
    if (retval < 0) {
        perror("_sem.c: rcs_sem_wait()");
        printf("rcs_sem_wait(%d,%f) returned %d (errno %d) after %f\n", *sem, timeout, retval, error, end_time - start_time);
    }
#endif
    return retval;

}

int rcs_sem_post(rcs_sem_t * sem)
{
    struct sembuf sops;
    union semun sem_arg;
    sem_arg.val = 0;

    rcs_print_debug(PRINT_SEMAPHORE_ACTIVITY, "rcs_sem_post(%d) called.\n",
	*sem);

    sops.sem_num = 0;		/* only one semaphore in set */
    sops.sem_flg = 0;		/* wait indefinitely */
    sops.sem_op = SEM_GIVE;

    if (semctl(*sem, 0, GETVAL, sem_arg) == 1) {
	/* it's given-- leave it alone */
	return 0;
    }

    /* it's taken-- suppose now others take it again before we give it? they
       block, and this semgive will release one of them */
    while (1) {
	if (semop(*sem, &sops, 1) == -1) {
	    if (errno == EINTR) {
		/* interrupted system call-- restart it */
		rcs_print_error("semop:");
		rcs_print_error("errno=%d : %s\n", errno, strerror(errno));
		rcs_puts("restarting");
		continue;
	    } else {
		rcs_print_error("semop");
		rcs_print_error("errno=%d : %s\n", errno, strerror(errno));
		return -1;
	    }
	} else {
	    return 0;
	}
    }
    return (0);
}

int rcs_sem_flush(rcs_sem_t * sem)
{
    int semval;
    int sems_to_give;
    int ncount = -1;
    struct sembuf sops;
    union semun sem_arg;
    sem_arg.val = 0;

    sops.sem_num = 0;		/* only one semaphore in set */
    sops.sem_flg = IPC_NOWAIT;	/* wait indefinitely */
    sops.sem_op = SEM_GIVE;
    semval = semctl(*sem, 0, GETVAL, sem_arg);
    ncount = semctl(*sem, 0, GETNCNT, sem_arg);

    /* Neither ncount nor semval should ever be less than zero any way, so
       this is just paranoid */
    if (semval < 0) {
	semval = 0;
    }
    if (ncount < 0) {
	ncount = 0;
    }
    if (semval > ncount) {
	return 0;
    }

    sems_to_give = ncount - semval + 1;

    /* it's taken-- suppose now others take it again before we give it? they
       block, and this semgive will release one of them until semval = 1; */
    sops.sem_op = sems_to_give;
    while (sems_to_give > 0) {
	if (semop(*sem, &sops, 1) == -1) {
	    if (errno == EINTR) {
		/* interrupted system call-- restart it */
		rcs_print_error("semop:");
		rcs_print_error("errno=%d : %s\n", errno, strerror(errno));
		rcs_puts("restarting");
		continue;
	    } else {
		rcs_print_error("semop");
		rcs_print_error("errno=%d : %s\n", errno, strerror(errno));
		return -1;
	    }
	}
	sems_to_give -= sops.sem_op;
    }
    return (0);
}

rcs_sem_t *rcs_sem_create(unsigned long int id, int mode, int state)
{
    union semun sem_arg;
    rcs_sem_t *sem;

    if (id < 1) {
	rcs_print_error("rcs_sem_create: invalid id %d\n", id);
	return NULL;
    }

    rcs_sem_open_val = state;

    sem = rcs_sem_open((char *) id, IPC_CREAT, mode);

    if (NULL == sem) {
	rcs_print_error("sem_init: Pointer to semaphore object is NULL.\n");
	return NULL;
    }
    sem_arg.val = state;
    semctl(*sem, 0, SETVAL, sem_arg);
    return sem;
}

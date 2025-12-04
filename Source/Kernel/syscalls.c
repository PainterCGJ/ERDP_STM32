/**
 ******************************************************************************
 * @file    syscalls.c
 * @brief   System call stubs for embedded systems
 *          These functions are required by the C library but not used in
 *          embedded systems without an operating system.
 *          Supports both GCC and ARM Compiler (armclang)
 ******************************************************************************
 */

#include <errno.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>

/* Disable semihosting for ARM Compiler */
#if defined(__ARMCC_VERSION) || defined(__ARM_COMPILER_VERSION)
/* ARM Compiler 5 uses #pragma import */
#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION < 6000000)
#pragma import(__use_no_semihosting)
#endif
/* ARM Compiler 6 (armclang) uses assembly directive */
#if defined(__ARM_COMPILER_VERSION) || (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6000000))
__asm(".global __use_no_semihosting\n\t");
#endif
/* The semihosting stub functions below will handle any remaining calls */
#endif

/* ARM Compiler (armclang) may not have sys/stat.h and sys/types.h */
#if defined(__ARMCC_VERSION) || defined(__ARM_COMPILER_VERSION)
/* Define types and structures for ARM Compiler */
typedef long off_t;
typedef long ssize_t;

/* Define stat structure */
struct stat
{
    int st_mode;
    /* Other fields not needed for embedded systems */
};

/* Define file mode constants */
#ifndef S_IFCHR
#define S_IFCHR  0020000  /* Character device */
#endif

/* Define error codes if not already defined */
#ifndef EINVAL
#define EINVAL  22  /* Invalid argument */
#endif
#ifndef EBADF
#define EBADF   9   /* Bad file descriptor */
#endif
#ifndef ENOMEM
#define ENOMEM  12  /* Out of memory */
#endif

/* Define caddr_t if not already defined */
#ifndef _CADDR_T_DECLARED
typedef char *caddr_t;
#define _CADDR_T_DECLARED
#endif
#else
/* GCC compiler - use standard headers */
#include <sys/stat.h>
#include <sys/types.h>

/* Define caddr_t if not already defined */
#ifndef _CADDR_T_DECLARED
typedef char *caddr_t;
#define _CADDR_T_DECLARED
#endif
#endif

/* External symbols from linker script or startup file */
#if defined(__ARMCC_VERSION) || defined(__ARM_COMPILER_VERSION)
/* ARM Compiler (Keil) - handle both microlib and non-microlib cases */
/* RAM memory layout for STM32F407VET6: 0x20000000 - 0x20020000 (128KB) */
#define RAM_START  0x20000000
#define RAM_END    0x20020000

/* Try microlib symbols first (if __MICROLIB is defined) */
#ifdef __MICROLIB
extern char __heap_base[];                 /* Heap start (from startup file) */
extern char __initial_sp[];                /* Stack top (from startup file) */
#else
/* For non-microlib, use section symbols or RAM address calculation */
/* Try to use linker-provided symbols, but don't require them */
/* We'll use a runtime calculation based on RAM addresses */
/* Note: This is a fallback - ideally scatter file should provide Image$$RW_IRAM1$$ZI$$Limit */
#endif
#else
/* GCC compiler uses linker script symbols */
extern char _end;      /* End of BSS section, start of heap */
extern char _estack;   /* End of RAM, top of stack */
#endif

/* External function for UART output (implemented in erdp_hal_uart.cpp) */
extern void erdp_uart_putchar(char character);

/**
 * @brief  Exit program
 * @param  status Exit status
 * @retval None
 */
void _exit(int status)
{
    (void)status;
    while (1)
    {
        // Infinite loop - embedded systems don't exit
    }
}

/**
 * @brief  Send signal to process
 * @param  pid Process ID
 * @param  sig Signal number
 * @retval 0 on success, -1 on error
 */
int _kill(int pid, int sig)
{
    (void)pid;
    (void)sig;
    errno = EINVAL;
    return -1;
}

/**
 * @brief  Get process ID
 * @retval Process ID (always 1 for embedded systems)
 */
int _getpid(void)
{
    return 1;
}

/**
 * @brief  Write to file descriptor
 * @param  file File descriptor
 * @param  ptr  Pointer to data
 * @param  len  Length of data
 * @retval Number of bytes written
 * @note   For stdout/stderr (file descriptors 1/2), output to UART
 */
int _write(int file, char *ptr, int len)
{
    int i;
    
    // Output to UART for stdout (1) and stderr (2)
    if (file == 1 || file == 2)
    {
        for (i = 0; i < len; i++)
        {
            erdp_uart_putchar(ptr[i]);
        }
        return len;
    }
    
    // Other file descriptors are not supported
    errno = EBADF;
    return -1;
}

/**
 * @brief  Read from file descriptor
 * @param  file File descriptor
 * @param  ptr  Pointer to buffer
 * @param  len  Length to read
 * @retval Number of bytes read
 */
int _read(int file, char *ptr, int len)
{
    (void)file;
    (void)ptr;
    (void)len;
    return 0;
}

/**
 * @brief  Close file descriptor
 * @param  file File descriptor
 * @retval 0 on success, -1 on error
 */
int _close(int file)
{
    (void)file;
    return -1;
}

/**
 * @brief  Get file status
 * @param  fildes File descriptor
 * @param  st     Pointer to stat structure
 * @retval 0 on success, -1 on error
 */
int _fstat(int fildes, struct stat *st)
{
    (void)fildes;
    st->st_mode = S_IFCHR;
    return 0;
}

/**
 * @brief  Get length of file
 * @param  file File descriptor
 * @param  ptr  Offset
 * @param  dir  Whence (SEEK_SET, SEEK_CUR, SEEK_END)
 * @retval Length of file
 */
int _lseek(int file, int ptr, int dir)
{
    (void)file;
    (void)ptr;
    (void)dir;
    return 0;
}

/**
 * @brief  Check if file is a terminal
 * @param  file File descriptor
 * @retval 1 if terminal, 0 otherwise
 */
int _isatty(int file)
{
    (void)file;
    return 1;
}

/**
 * @brief  Program break (heap) management
 * @param  incr Increment in bytes
 * @retval Previous program break, or (caddr_t)-1 on error
 * @note   This function is required by malloc/free and other heap operations
 */
caddr_t _sbrk(int incr)
{
    static char *heap_end = NULL;
    char *prev_heap_end;
#if defined(__ARMCC_VERSION) || defined(__ARM_COMPILER_VERSION)
    /* ARM Compiler: determine heap start */
    if (heap_end == NULL)
    {
#ifdef __MICROLIB
        /* Use microlib symbols */
        heap_end = (char *)__heap_base;
#else
        /* For non-microlib, calculate heap start from RAM address */
        /* We need to find where BSS/data sections end */
        /* Since we don't have reliable symbols, we'll use a conservative approach */
        /* Find the end of initialized data by looking for a known variable */
        /* Or use a fixed offset from RAM_START */
        
        /* Method: Use a fixed offset that should be safe for most applications */
        /* This assumes BSS/data sections don't exceed 32KB */
        /* For a more accurate solution, Keil project should use microlib or scatter file */
        heap_end = (char *)RAM_START + 0x8000;  /* 32KB offset - conservative estimate */
        
        /* Note: For production use, consider:
         * 1. Enabling microlib in Keil project settings, OR
         * 2. Creating a scatter file that defines Image$$RW_IRAM1$$ZI$$Limit, OR
         * 3. Modifying startup file to export __heap_base even without microlib
         */
#endif
    }

    prev_heap_end = heap_end;

    /* Check if heap would overflow into stack */
#ifdef __MICROLIB
    /* Stack grows downward from __initial_sp */
    if (heap_end + incr > (char *)__initial_sp - 0x100)
    {
        errno = ENOMEM;
        return (caddr_t)-1;
    }
#else
    /* Stack grows downward from RAM_END, so we check against it */
    /* Leave some safety margin (16KB) to prevent stack/heap collision */
    if (heap_end + incr > (char *)RAM_END - 0x4000)
    {
        errno = ENOMEM;
        return (caddr_t)-1;
    }
#endif
#else
    /* GCC compiler: use _end as heap start */
    if (heap_end == NULL)
    {
        /* First call, initialize heap_end to _end */
        heap_end = &_end;
    }

    prev_heap_end = heap_end;

    /* Check if heap would overflow into stack */
    if (heap_end + incr > &_estack)
    {
        errno = ENOMEM;
        return (caddr_t)-1;
    }
#endif

    heap_end += incr;
    return (caddr_t)prev_heap_end;
}

/**
 * @brief  Output a character to UART (used by standard library printf)
 * @param  character Character to output
 * @retval None
 * @note   This function is called by the standard library printf implementation
 */
void _putchar(char character)
{
    erdp_uart_putchar(character);
}

/* Semihosting stub functions for ARM Compiler */
#if defined(__ARMCC_VERSION) || defined(__ARM_COMPILER_VERSION)
/**
 * @brief  File put character - override semihosting version
 * @param  ch Character to output
 * @param  file File pointer (stdout/stderr)
 * @retval Character written
 * @note   This function overrides the semihosting fputc from standard library
 *         Required when __use_no_semihosting is declared
 */
int fputc(int ch, FILE *file)
{
    (void)file;  /* Ignore file pointer - always output to UART */
    erdp_uart_putchar((char)ch);
    return ch;
}
/**
 * @brief  Semihosting stub: character output
 * @param  ch Character to output
 * @retval None
 * @note   This function is called when semihosting is disabled
 *         Redirect to UART instead of debugger
 */
void _ttywrch(int ch)
{
    erdp_uart_putchar((char)ch);
}

/**
 * @brief  Semihosting stub: system exit
 * @param  status Exit status
 * @retval None
 * @note   Embedded systems don't exit - infinite loop
 */
void _sys_exit(int status)
{
    (void)status;
    while (1)
    {
        /* Infinite loop - embedded systems don't exit */
    }
}

/**
 * @brief  Semihosting stub: command line string
 * @retval NULL (no command line in embedded systems)
 */
char *_sys_command_string(char *cmd, int len)
{
    (void)cmd;
    (void)len;
    return NULL;
}

/**
 * @brief  Semihosting stub: clock function
 * @retval 0 (no clock in embedded systems)
 */
clock_t clock(void)
{
    return 0;
}

/**
 * @brief  Semihosting stub: time function
 * @retval -1 (no time in embedded systems)
 */
time_t time(time_t *timer)
{
    (void)timer;
    return -1;
}

/**
 * @brief  Semihosting stub: remove file
 * @retval -1 (not supported)
 */
int remove(const char *filename)
{
    (void)filename;
    return -1;
}

/**
 * @brief  Semihosting stub: rename file
 * @retval -1 (not supported)
 */
int rename(const char *old, const char *new)
{
    (void)old;
    (void)new;
    return -1;
}
#endif /* __ARMCC_VERSION || __ARM_COMPILER_VERSION */

/* ARM Compiler (armclang) specific system calls */
#if defined(__ARMCC_VERSION) || defined(__ARM_COMPILER_VERSION)
/**
 * @brief  ARM Compiler specific: fstat for file descriptor
 */
int fstat(int fd, struct stat *st)
{
    return _fstat(fd, st);
}

/**
 * @brief  ARM Compiler specific: isatty for file descriptor
 */
int isatty(int fd)
{
    return _isatty(fd);
}

/**
 * @brief  ARM Compiler specific: lseek for file descriptor
 */
off_t lseek(int fd, off_t offset, int whence)
{
    return _lseek(fd, (int)offset, whence);
}

/**
 * @brief  ARM Compiler specific: close for file descriptor
 */
int close(int fd)
{
    return _close(fd);
}

/**
 * @brief  ARM Compiler specific: read for file descriptor
 */
ssize_t read(int fd, void *buf, size_t count)
{
    return _read(fd, (char *)buf, (int)count);
}

/**
 * @brief  ARM Compiler specific: write for file descriptor
 */
ssize_t write(int fd, const void *buf, size_t count)
{
    return _write(fd, (char *)buf, (int)count);
}
#endif /* __ARMCC_VERSION || __ARM_COMPILER_VERSION */


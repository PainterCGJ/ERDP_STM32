/**
 ******************************************************************************
 * @file    syscalls.c
 * @brief   System call stubs for embedded systems
 *          These functions are required by the C library but not used in
 *          embedded systems without an operating system.
 ******************************************************************************
 */

#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

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
 */
int _write(int file, char *ptr, int len)
{
    (void)file;
    (void)ptr;
    (void)len;
    return 0;
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



/********************************************************************************************************
 * @file	cfs.h
 *
 * @brief	This is the header file for cfs
 *
 * @author	Zigbee Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************************************/
#ifndef CFS_H_
#define CFS_H_

typedef int cfs_offset_t;

struct cfs_dir {
  char dummy_space[32];
};

struct cfs_dirent {
  char name[32];
  cfs_offset_t size;
};

/**
 * Specify that cfs_open() should open a file for reading.
 *
 * This constant indicates to cfs_open() that a file should be opened
 * for reading. CFS_WRITE should be used if the file is opened for
 * writing, and CFS_READ + CFS_WRITE indicates that the file is opened
 * for both reading and writing.
 *
 * \sa cfs_open()
 */
#define CFS_READ  1

/**
 * Specify that cfs_open() should open a file for writing.
 *
 * This constant indicates to cfs_open() that a file should be opened
 * for writing. CFS_READ should be used if the file is opened for
 * reading, and CFS_READ + CFS_WRITE indicates that the file is opened
 * for both reading and writing.
 *
 * \sa cfs_open()
 */
#define CFS_WRITE 2

/**
 * Specify that cfs_open() should append written data to the file rather than overwriting it.
 *
 * This constant indicates to cfs_open() that a file that should be
 * opened for writing gets written data appended to the end of the
 * file. The default behaviour (without CFS_APPEND) is that the file
 * is overwritten with the new data.
 *
 * \sa cfs_open()
 */
#define CFS_APPEND 4

/**
 * Specify that cfs_seek() should compute the offset from the beginning of the file.
 *
 * \sa cfs_seek()
 */
#define CFS_SEEK_SET 0

/**
 * Specify that cfs_seek() should compute the offset from the current position of the file pointer.
 *
 * \sa cfs_seek()
 */
#define CFS_SEEK_CUR 1

/**
 * Specify that cfs_seek() should compute the offset from the end of the file.
 *
 * \sa cfs_seek()
 */
#define CFS_SEEK_END 2

/**
 * \brief      Open a file.
 * \param name The name of the file.
 * \param flags CFS_READ, or CFS_WRITE/CFS_APPEND, or both.
 * \return     A file descriptor, if the file could be opened, or -1 if
 *             the file could not be opened.
 *
 *             This function opens a file and returns a file
 *             descriptor for the opened file. If the file could not
 *             be opened, the function returns -1. The function can
 *             open a file for reading or writing, or both.
 *
 *             An opened file must be closed with cfs_close().
 *
 * \sa         CFS_READ
 * \sa         CFS_WRITE
 * \sa         cfs_close()
 */
int cfs_open(const char *name, int flags);

/**
 * \brief      Close an open file.
 * \param fd   The file descriptor of the open file.
 *
 *             This function closes a file that has previously been
 *             opened with cfs_open().
 */
void cfs_close(int fd);

/**
 * \brief      Read data from an open file.
 * \param fd   The file descriptor of the open file.
 * \param buf  The buffer in which data should be read from the file.
 * \param len  The number of bytes that should be read.
 * \return     The number of bytes that was actually read from the file.
 *
 *             This function reads data from an open file into a
 *             buffer. The file must have first been opened with
 *             cfs_open() and the CFS_READ flag.
 */
int cfs_read(int fd, void *buf, unsigned int len);

/**
 * \brief      Write data to an open file.
 * \param fd   The file descriptor of the open file.
 * \param buf  The buffer from which data should be written to the file.
 * \param len  The number of bytes that should be written.
 * \return     The number of bytes that was actually written to the file.
 *
 *             This function reads writes data from a memory buffer to
 *             an open file. The file must have been opened with
 *             cfs_open() and the CFS_WRITE flag.
 */
int cfs_write(int fd, const void *buf, unsigned int len);

/**
 * \brief      Seek to a specified position in an open file.
 * \param fd   The file descriptor of the open file.
 * \param offset A position, either relative or absolute, in the file.
 * \param whence Determines how to interpret the offset parameter.
 * \return     The new position in the file, or (cfs_offset_t)-1 if the seek failed.
 *
 *             This function moves the file position to the specified
 *             position in the file. The next byte that is read from
 *             or written to the file will be at the position given 
 *             determined by the combination of the offset parameter 
 *             and the whence parameter.
 *
 * \sa         CFS_SEEK_CUR
 * \sa         CFS_SEEK_END
 * \sa         CFS_SEEK_SET
 */
cfs_offset_t cfs_seek(int fd, cfs_offset_t offset, int whence);

/**
 * \brief      Remove a file.
 * \param name The name of the file.
 * \retval 0   If the file was removed.
 * \return -1  If the file could not be removed or if it doesn't exist.
 */
int cfs_remove(const char *name);

/**
 * \brief      Open a directory for reading directory entries.
 * \param dirp A pointer to a struct cfs_dir that is filled in by the function.
 * \param name The name of the directory.
 * \return     0 or -1 if the directory could not be opened.
 *
 * \sa         cfs_readdir()
 * \sa         cfs_closedir()
 */
int cfs_opendir(struct cfs_dir *dirp, const char *name);

/**
 * \brief      Read a directory entry
 * \param dirp A pointer to a struct cfs_dir that has been opened with cfs_opendir().
 * \param dirent A pointer to a struct cfs_dirent that is filled in by cfs_readdir()
 * \retval 0   If a directory entry was read.
 * \retval -1  If no more directory entries can be read.
 *
 * \sa         cfs_opendir()
 * \sa         cfs_closedir()
 */
int cfs_readdir(struct cfs_dir *dirp, struct cfs_dirent *dirent);

/**
 * \brief      Close a directory opened with cfs_opendir().
 * \param dirp A pointer to a struct cfs_dir that has been opened with cfs_opendir().
 *
 * \sa         cfs_opendir()
 * \sa         cfs_readdir()
 */
void cfs_closedir(struct cfs_dir *dirp);

#endif /* CFS_H_ */

/** @} */
/** @} */

# Memory mapping.
mmap and munmap are used to map and unmap files onto virtual space of the calling process. (Provided by the standard C library)

**Include: sys/mman.h**

## Function signature:
void *mmap(void addr[.length], size_t length, int prot, int flags, int fd, off_t offset);
int munmap(void addr[.length], size_t length);

## mmap
The mmap function creates a memory mapped file in the virtual space allocated to the calling process.
On success, mmap() returns a pointer to the mapped area. On error, the value MAP_FAILED (that is, (void *) -1) is returned, and errno is set to indicate the error.

* The addr parameters specifies the starting (or base) address of the mapping being created. If set to NULL, the kernel decides the starting location (page-aligned)

If addr is not NULL, then kernel treats this parameters as a suggestion on where to place the memory map. The Linux kernel chooses the nearby page-aligned address but always above or equal to the minimum allocation address specified at /proc/sys/vm/mmap_min_addr.

The minimum allocation address space is similar to allocation granularity in windows.

If the memory is not available or other mapping exists at the specified address, then the linux kernel picks a new address which may or may not depend on the addr hint provided. The return value provides this address.

* The length argument specifies the length of the mapping (which must be greater than 0).

The content of the file map are initialized by the length parameter starting at an offset given by the offset parameter within the file descriptor object (or any object given to fd parameter).

Similar to windows, offsets must be integral values of the minimum allocation space (or paging space). This value can be found by sysconf(_SC_PAGE_SIZE).

**Note: Unlike windows, the file descriptor (aka handle of the file in windows) can be closed once the memory map has been created.**

* The prot argument describes the desired memory protection of the mapping (and must not conflict with the open mode of the file). It is either PROT_NONE or the bitwise OR of one or more of the following flags:

    - PROT_EXEC -> Pages may be executed.
    - PROT_READ -> Pages may be read.
    - PROT_WRITE -> Pages may be written.
    - PROT_NONE -> Pages may not be accessed.

* Flags argument determines weather the updates done to the mapping are visible to other processes mapping the same region and if the updates are performed on the actual file. It is determined by the following values:

    - MAP_SHARED: Share this mapping. Updates performed to the mapping are visible to other processes and if the mapped region corresponds to an actual file, the updates are written to the file as well. However, to control when updates are written to the file, use msync().

    - MAP_SHARED_VALIDATE (Linux Kernel > 4.15): MAP_SHARED by default ignores any invalid flags passed, however if flag is set to MAP_SHARED_VALIDATE, the Linux kernel will not create the memory map if it doesn't identify a flag passed to it and returns an error code of EOPNOTSUPP.

    - MAP_PRIVATE: Creates a private copy-on-write mapping. Updates performed to the mapping are not visible or shared with other processes mapping the same region and the updates are not written to the underlying file (actual file on disk). Any updates performed to the actual file may or may not be reflected on the mapped region.

    **NOTE: MAP_SHARED and MAP_PRIVATE are described by the POSIX UNIX Library. MAP_SHARED_VALIDATE is a Linux kernel extension.**

## munmap
The munmap function deletes the mapping for the specified address range and any further access or references to the address will generate invalid memory references. **The region is automatically unmapped when the process terminates but closing the descriptor doesn't unmap the region.**

* The parameter addr must be a multiple of the page size. By now, it makes sense to assume that the kernel (Linux kernel in this case) will automatically ensure this condition is satisfied. Primarily because, unlike windows it is clearly mentioned that the Linux kernel treats the address passed as a hint to select a paging address.

* The length parameter however need not be a multiple of the page size.

All pages residing in the said address range (even if multiple pages exist in that region) are unmapped and any subsequent references to these pages will generate SIGSEGV.

**NOTE: It is not an error if the indicated range does not contain any mapped pages.**

On success, munmap() returns 0.  On failure, it returns -1, and errno is set to indicate the error (probably to EINVAL).
Ore Adebayo
CS 450 - Assignment 6: Symbolic Links

APPROACH
Symbolic link support was added to xv6 across three layers: a new inode type, a new syscall, and resolution logic in the file open path. A symlink inode stores its target path as a plain string in its data blocks rather than file content, allowing the file system to follow the link at open time.

INODE TYPE
A new type T_SYMLINK 4 was added to stat.h to distinguish symlink inodes from regular files, directories, and devices. MAXPATH 128 was added to param.h to define the maximum path length used during resolution.

SYSCALL IMPLEMENTATION
sys_symlink(const char *target, const char *linkpath) was implemented in sysfile.c. It uses the existing create() helper to allocate a new inode of type T_SYMLINK at linkpath, then writes the target string into the inode's data blocks using writei(). The inode size is manually set and persisted with iupdate() to ensure the target path survives across reboots.

RESOLUTION
Symlink resolution was added to sys_open() in sysfile.c. After finding an inode via namei(), a loop of up to 10 iterations checks if the inode is a symlink. If it is, readi() reads the target path from the inode's data blocks into a buffer, the current inode is released, and namei() is called again with the new path. If the loop exhausts all 10 iterations without resolving to a non-symlink, the open fails and returns -1, preventing infinite cycles.

TESTS
testsymlink.c verifies two behaviors: reading through a symlink returns the target file's contents, and a cycle between two symlinks correctly triggers failure after the depth limit is reached.
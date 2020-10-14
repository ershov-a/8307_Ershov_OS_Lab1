#ifndef INC_8307_ERSHOV_OS_LAB1_DRIVESFOLDERSFILES_H
#define INC_8307_ERSHOV_OS_LAB1_DRIVESFOLDERSFILES_H

#include <iostream>
#include <limits>
#include <vector>
#include <map>
#include <string>
#include <bitset>
#include <windows.h>
#include <io.h>

bool checkFileExist(std::string fileName);

void listDrives();

void showDiskInfo();

void createDirectory();

void removeDirectory();

void createFile();

void moveFile();

void copyFile();

void getFileInfo();

void setFileAttributes();

void setFileTime();

// Source https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getdrivetypea
std::vector<std::string> const DRIVES_TYPE = {
        "Cannot be determined",
        "Invalid root path/Not available",
        "Removable",
        "Fixed",
        "Network",
        "CD-ROM",
        "RAMDISK",
};


// Source https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumeinformationbyhandlew
std::map<DWORD, std::string> const FS_FLAGS = {
        {FILE_CASE_PRESERVED_NAMES, "preserved case file names."},
        {FILE_CASE_SENSITIVE_SEARCH, "case-sensitive file names."},
        {FILE_FILE_COMPRESSION, "file-based compression."},
        {FILE_NAMED_STREAMS, "named streams."},
        {FILE_PERSISTENT_ACLS, "preserves and enforces access control lists (ACL)."},
        {FILE_READ_ONLY_VOLUME, "read-only mode."},
        {FILE_SEQUENTIAL_WRITE_ONCE, "a single sequential write."},
        {FILE_SUPPORTS_ENCRYPTION, "the Encrypted File System (EFS)."},
        {FILE_SUPPORTS_EXTENDED_ATTRIBUTES, "extended attributes."},
        {FILE_SUPPORTS_HARD_LINKS, "hard links."},
        {FILE_SUPPORTS_OBJECT_IDS, "object identifiers."},
        {FILE_SUPPORTS_OPEN_BY_FILE_ID, "open by FileID."},
        {FILE_SUPPORTS_REPARSE_POINTS, "reparse points."},
        {FILE_SUPPORTS_SPARSE_FILES, "sparse files."},
        {FILE_SUPPORTS_TRANSACTIONS, "transactions."},
        {FILE_SUPPORTS_USN_JOURNAL, "update sequence number (USN) journals."},
        {FILE_UNICODE_ON_DISK, "Unicode in file names."},
        {FILE_VOLUME_IS_COMPRESSED, "compression volume."},
        {FILE_VOLUME_QUOTAS, "disk quotas."},
};

// Source https://docs.microsoft.com/en-us/windows/win32/fileio/file-attribute-constants
std::map<DWORD, std::string> const FILE_ATTRIBUTES = {
        {FILE_ATTRIBUTE_ARCHIVE, "archive file or directory."}, // 0x20
        {FILE_ATTRIBUTE_COMPRESSED, "compressed file or directory."},
        {FILE_ATTRIBUTE_DEVICE, "reserved for system use."},
        {FILE_ATTRIBUTE_DIRECTORY, "handle that identifies a directory."},
        {FILE_ATTRIBUTE_ENCRYPTED, "encrypted file or directory."},
        {FILE_ATTRIBUTE_HIDDEN, "hidden file or directory."}, // 0x2
        {FILE_ATTRIBUTE_NORMAL, "not having other attributes set."}, // 0x80
        {FILE_ATTRIBUTE_NOT_CONTENT_INDEXED, "not indexed file or directory"}, // 0x2000
        {FILE_ATTRIBUTE_OFFLINE, "data of a file not available immediately."}, // 0x1000
        {FILE_ATTRIBUTE_READONLY, "read-only."}, // 0x1
        {FILE_ATTRIBUTE_REPARSE_POINT, "a file or directory that has an associated reparse point, or a file that is a symbolic link."},
        {FILE_ATTRIBUTE_SPARSE_FILE, "a file that is a sparse file."},
        {FILE_ATTRIBUTE_SYSTEM, "a file or directory that the operating system uses a part of, or uses exclusively."}, // 0x4
        {FILE_ATTRIBUTE_TEMPORARY, "a file that is being used for temporary storage."}, // 0x100
        {FILE_ATTRIBUTE_VIRTUAL, "reserved for system use."},
};

#endif //INC_8307_ERSHOV_OS_LAB1_DRIVESFOLDERSFILES_H

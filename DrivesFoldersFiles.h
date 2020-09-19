#ifndef INC_8307_ERSHOV_OS_LAB1_DRIVESFOLDERSFILES_H
#define INC_8307_ERSHOV_OS_LAB1_DRIVESFOLDERSFILES_H

#include <iostream>
#include <limits>
#include <vector>
#include <map>
#include <string>
#include <bitset>
#include <windows.h>

void listDrives();

void showDiskInfo();

void createDirectory();

void removeDirectory();

void createFile();

void moveFile();

void copyFile();

void getFileInfo();


std::vector<std::string> const DRIVES_TYPE = {
        "Cannot be determined",
        "Invalid root path/Not available",
        "Removable",
        "Fixed",
        "Network",
        "CD-ROM",
        "RAMDISK",
};

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

#endif //INC_8307_ERSHOV_OS_LAB1_DRIVESFOLDERSFILES_H

#ifndef FS_H
#define FS_H

#include <stdint.h>

#define FS_NAME_MAX 32
#define FS_MAX_ENTRIES 256
#define FS_SIGNATURE "AOPFS"
#define FS_ENTRY_SIZE sizeof(fs_entry_t)
#define FS_SECTOR 10

typedef struct __attribute__((packed)) {
    char name[FS_NAME_MAX];
    uint32_t start_address;
    uint32_t end_address;
    uint8_t used;
} fs_entry_t;

static fs_entry_t fs_root[FS_MAX_ENTRIES];

void fs_init();
void fs_save();
void fs_list_directory(fs_entry_t* dir);
fs_entry_t* fs_find_entry(fs_entry_t* dir, const char* name);
void fs_prompt_view_file(fs_entry_t* dir);

#endif

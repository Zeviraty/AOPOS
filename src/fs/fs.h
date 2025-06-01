#ifndef FS_H
#define FS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define FS_NAME_MAX 32
#define FS_MAX_FILES 64

int fs_create_file(const char* name, uint32_t start_address, uint32_t end_address);

int fs_write_file(const char* name, const void* data, uint32_t size);

int fs_read_file(const char* name, void* buffer, uint32_t size);

bool fs_load_from_disk(void);

void fs_save_to_disk(void);

typedef struct __attribute__((packed)) {
    char name[FS_NAME_MAX];
    uint32_t start_address;
    uint32_t end_address;
    uint8_t used;
} fs_entry_t;

void fs_init(void);

fs_entry_t* fs_find(const char* name);

fs_entry_t** fs_listdir(const char* folder, size_t* count);

#endif 


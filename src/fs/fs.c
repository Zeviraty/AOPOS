#include "fs.h"
#include "lib/string.h"
#include "drivers/ata.h"
#include "kernel/terminal.h"
#include <stdbool.h>

static fs_entry_t fs_table[FS_MAX_FILES];

#define FS_START_SECTOR 2
#define FS_SIGNATURE "AOPFS"
#define FS_SIGNATURE_LEN 5
#define FS_START_SECTOR 2

void fs_init(void) {
    terminal_writestring("Loading filesystem...\n");

    if (!fs_load_from_disk()) {
	//terminal_writestring("No valid FS found. Initializing new FS.\n");
	//memset(fs_table, 0, sizeof(fs_table));
	//fs_save_to_disk();
    } else {
	terminal_writestring("Filesystem loaded successfully.\n");
    }
}

fs_entry_t* fs_find(const char* name) {
    for (int i = 0; i < FS_MAX_FILES; i++) {
        if (fs_table[i].used && strncmp(fs_table[i].name, name, FS_NAME_MAX) == 0) {
            return &fs_table[i];
        }
    }
    return NULL;
}

int fs_create_file(const char* name, uint32_t start_address, uint32_t end_address) {
    if (fs_find(name) != NULL) {
        return -1;
    }

    for (int i = 0; i < FS_MAX_FILES; i++) {
        if (!fs_table[i].used) {
            strncpy(fs_table[i].name, name, FS_NAME_MAX);
            fs_table[i].name[FS_NAME_MAX - 1] = '\0';
            fs_table[i].start_address = start_address;
            fs_table[i].end_address = end_address;
            fs_table[i].used = 1;
            return 0;
        }
    }
    return -1;
}

int fs_write_file(const char* name, const void* data, uint32_t size) {
    fs_entry_t* entry = fs_find(name);
    if (!entry) return -1;

    uint32_t file_size = entry->end_address - entry->start_address;
    if (size > file_size) return -1;
    return 0;
}

int fs_read_file(const char* name, void* buffer, uint32_t size) {
    fs_entry_t* entry = fs_find(name);
    if (!entry) return -1;

    uint32_t file_size = entry->end_address - entry->start_address;
    if (size > file_size) return -1;
    return 0;
}

fs_entry_t** fs_listdir(const char* folder, size_t* count) {
    static fs_entry_t* results[FS_MAX_FILES];
    *count = 0;

    size_t folder_len = strlen(folder);
    for (int i = 0; i < FS_MAX_FILES; i++) {
        if (!fs_table[i].used)
            continue;

        const char* name = fs_table[i].name;

        if (strncmp(name, folder, folder_len) == 0 && name[folder_len] == '/') {
            const char* remainder = name + folder_len + 1;
            if (strchr(remainder, '/') == NULL) {
                results[*count] = &fs_table[i];
                (*count)++;
            }
        }
    }

    return results;
}

bool fs_load_from_disk() {
    uint8_t buffer[512];

    terminal_writestring("Reading FS start sector...\n");
    ata_read_sectors(FS_START_SECTOR, 1, buffer);

    terminal_writestring("Checking FS signature...\n");
    if (memcmp(buffer, FS_SIGNATURE, FS_SIGNATURE_LEN) != 0) {
        terminal_writestring("Invalid FS signature\n");
	terminal_writestring("No valid FS found. Initializing new FS.\n");
	memset(fs_table, 0, sizeof(fs_table));
	fs_save_to_disk();
    }

    uint32_t entries_per_first_sector = (512 - FS_SIGNATURE_LEN) / sizeof(fs_entry_t);
    char msg[64];

    terminal_writestring("Signature valid, copying entries from first sector...\n");
    memcpy(fs_table, buffer + FS_SIGNATURE_LEN, entries_per_first_sector * sizeof(fs_entry_t));

    uint32_t remaining = FS_MAX_FILES - entries_per_first_sector;
    if (remaining > 0) {
        uint32_t bytes_remaining = remaining * sizeof(fs_entry_t);
        uint32_t sectors_remaining = (bytes_remaining + 511) / 512;

        terminal_writestring("Reading remaining entries.\n");

        ata_read_sectors(FS_START_SECTOR + 1, sectors_remaining,
                         (uint8_t*)fs_table + entries_per_first_sector * sizeof(fs_entry_t));
        terminal_writestring("Finished reading remaining entries\n");
    }

    return true;
}

void fs_save_to_disk() {
    terminal_writestring("Start\n");

    uint8_t buffer[512] = {0};

    terminal_writestring("Copying signature\n");
    memcpy(buffer, FS_SIGNATURE, FS_SIGNATURE_LEN);

    uint32_t entries_per_first_sector = (512 - FS_SIGNATURE_LEN) / sizeof(fs_entry_t);
    uint32_t bytes_to_copy = entries_per_first_sector * sizeof(fs_entry_t);

    terminal_writestring("Copying fs_table entries to buffer\n");
    memcpy(buffer + FS_SIGNATURE_LEN, fs_table, bytes_to_copy);

    terminal_writestring("Writing first sector\n");
    ata_write_sectors(FS_START_SECTOR, 1, buffer);
    terminal_writestring("First sector written\n");

    uint32_t remaining = FS_MAX_FILES - entries_per_first_sector;
    if (remaining > 0) {
        uint32_t bytes_remaining = remaining * sizeof(fs_entry_t);
        uint32_t sectors_remaining = (bytes_remaining + 511) / 512;

        char msg[64];
        terminal_writestring("Writing remaining entries.\n");

        ata_write_sectors(FS_START_SECTOR + 1,
                          sectors_remaining,
                          (uint8_t*)fs_table + bytes_to_copy);
        terminal_writestring("Remaining sectors written\n");
    }

    terminal_writestring("Done\n");
}


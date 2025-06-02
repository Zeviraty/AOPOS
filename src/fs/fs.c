#include "fs.h"
#include "lib/string.h"
#include "kernel/terminal.h"
#include "drivers/ata.h"
#include "system/x86/io.h"

__attribute__((aligned(512)))
static fs_entry_t fs_root[FS_MAX_ENTRIES];

extern int input_buffer_get();

void fs_init() {
    char signature[6] = {0};
    ata_read_sectors(FS_SECTOR, 1, signature);
    terminal_writestring("Checking signature...\n");
    if (strncmp(signature, FS_SIGNATURE, 5) == 0) {
    	terminal_writestring("Reading fs sectors...\n");
        ata_read_sectors(FS_SECTOR + 1, (sizeof(fs_root) + 511) / 512, fs_root);
    	terminal_writestring("Done reading fs sectors...\n");
    } else {
        memset(fs_root, 0, sizeof(fs_root));
        fs_save();
    }
}

void fs_save() {
    uint8_t sig_block[512] = {0};
    memcpy(sig_block, FS_SIGNATURE, 5);
    ata_write_sectors(FS_SECTOR, 1, sig_block);
    ata_write_sectors(FS_SECTOR + 1, (sizeof(fs_root) + 511) / 512, fs_root);
}

void fs_list_directory(fs_entry_t* dir) {
    fs_entry_t* entries = (dir == 0) ? fs_root : (fs_entry_t*)(uintptr_t)dir->start_address;
    for (int i = 0; i < FS_MAX_ENTRIES; i++) {
        if (entries[i].used) {
            terminal_writestring("- ");
            terminal_writestring(entries[i].name);
            terminal_putchar('\n');
        }
    }
}

fs_entry_t* fs_find_entry(fs_entry_t* dir, const char* name) {
    fs_entry_t* entries = (dir == 0) ? fs_root : (fs_entry_t*)(uintptr_t)dir->start_address;
    for (int i = 0; i < FS_MAX_ENTRIES; i++) {
        if (entries[i].used && strncmp(entries[i].name, name, FS_NAME_MAX) == 0) {
            return &entries[i];
        }
    }
    return 0;
}

void fs_prompt_view_file(fs_entry_t* dir) {
    char input[FS_NAME_MAX];
    terminal_writestring("Enter file name: ");

    size_t idx = 0;
    while (1) {
        int c = input_buffer_get();
        if (c == -1) continue;

        if ((char)c == '\n' || idx >= FS_NAME_MAX - 1) {
            input[idx] = '\0';
            break;
        } else {
            input[idx++] = (char)c;
            terminal_putchar((char)c);
        }
    }
    terminal_putchar('\n');

    fs_entry_t* file = fs_find_entry(dir, input);
    if (!file || !file->used) {
        terminal_writestring("File not found.\n");
        return;
    }

    uint32_t size = file->end_address - file->start_address;
    char* data = (char*)(uintptr_t)file->start_address;
    for (uint32_t i = 0; i < size; i++) {
        terminal_putchar(data[i]);
    }
    terminal_putchar('\n');
}


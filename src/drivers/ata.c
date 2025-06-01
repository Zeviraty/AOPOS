#include "ata.h"

void ata_read_sectors(uint32_t lba, uint8_t count, void* buffer) {
    ata_lba_read(lba, count, buffer);
}

void ata_write_sectors(uint32_t lba, uint8_t count, const void* buffer) {
    ata_lba_write(lba, count, buffer);
}

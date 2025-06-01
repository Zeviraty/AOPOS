#pragma once
#include <stdint.h>

void ata_lba_read(uint32_t lba, uint8_t sectors, void* buffer);
void ata_lba_write(uint32_t lba, uint8_t sectors, const void* buffer);

void ata_read_sectors(uint32_t lba, uint8_t sectors, void* buffer);
void ata_write_sectors(uint32_t lba, uint8_t sectors, const void* buffer);

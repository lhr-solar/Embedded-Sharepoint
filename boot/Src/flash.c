#include "flash.h"
#include "stm32xx_hal.h"
#include <string.h>

static uint8_t sector_erased_bitmap = 0;

bool exec_flash_command(volatile void* buf, flash_cmd_t* cmd){
    if(cmd->data_size > 8 ||
        cmd->id >= NUM_IDS ||
        cmd->address < 0x08020000 || cmd->address > 0x0807FFFF){
        return false;
    }
    
    switch(cmd->id){
        case FLASH_WRITE:
            return flash_write(buf, cmd->address, cmd->data_size);
        case FLASH_READ_SINGLE:
            return flash_read_single(buf, cmd->address, cmd->data_size);
        case FLASH_READ_BUF:
            return flash_read_buf(buf, cmd->address, cmd->data_size);
        case FLASH_ERASE:
            return flash_erase(cmd->address);
        case FLASH_MASS_ERASE:
            return flash_mass_erase();
        
    }

    return false;
}

#define FLASH_SECTOR_MASK (0x00060000)
#define FLASH_SECTOR(addr) (((((uint32_t)addr) & FLASH_SECTOR_MASK)>>17) + 4)

static bool flash_write_single(uint64_t data, void* address, uint8_t data_size){
    uint32_t program_type;
    switch(data_size){
        case 1:
            program_type = FLASH_TYPEPROGRAM_BYTE;
            break;
        case 2:
            program_type = FLASH_TYPEPROGRAM_HALFWORD;
            break;
        case 4:
            program_type = FLASH_TYPEPROGRAM_WORD;
            break;
        case 8:
            program_type = FLASH_TYPEPROGRAM_DOUBLEWORD;
            break;
        default:
            return false;
    }

    // Try programming the data
    if(HAL_FLASH_Program(program_type, address, data) == HAL_ERROR){
        return false;
    }

    // verify
    if(memcmp((volatile void*)address, &data, data_size) != 0){
        // If the data is not the same, erase the sector and try again
        if(flash_erase(address) == false){
            return false;
        }

        if(HAL_FLASH_Program(program_type, address, data) == HAL_ERROR){
            return false;
        }

        if(memcmp((volatile void*)address, &data, data_size) != 0){
            return false;
        }
    }

    if(flash_erase(address) == false){
        return false;
    }

    return true;
}

bool flash_write(volatile void* buf, void* address, uint16_t data_size){
    if(HAL_FLASH_Unlock() == HAL_ERROR){
        return false;
    }

    while(data_size > 0){
        uint8_t write_size = 8;
        while((write_size != 0) && ((data_size&write_size) == 0)) write_size>>=1; // shift until we find most sig bit
        if(write_size == 0){
            HAL_FLASH_Lock();
            return false;
        }

        if(flash_write_single(*(uint64_t*)buf, address, write_size) == false){
            flash_erase(address); // erase the sector the write fails on
            HAL_FLASH_Lock();
            return false;
        }

        address += write_size;
        buf = ((uint8_t*)buf + write_size);
        data_size -= write_size;
    }

    if(HAL_FLASH_Lock() == HAL_ERROR){
        return false;
    }

    return true;
}

bool flash_read_single(void* data, void* address, uint8_t data_size){
    switch(data_size){
        case 1:
            *(uint8_t*)data = *(volatile uint8_t*)address;
            break;
        case 2:
            *(uint16_t*)data = *(volatile uint16_t*)address;
            break;
        case 4:
            *(uint32_t*)data = *(volatile uint32_t*)address;
            break;
        case 8:
            *(uint64_t*)data = *(volatile uint64_t*)address;
            break;
        default:
            return false;
    }

    return true;
}

bool flash_read_buf(volatile void* buf, void* address, uint16_t data_size){
    return (memcpy(buf, address, data_size) == buf);
}

bool flash_erase(void* address){
    
    // Only allow writes to application space
    if(address < 0x08020000){
        return false;
    }

    uint8_t sector = FLASH_SECTOR(address);

    // Has the sector been erased in the current session?
    if((sector_erased_bitmap & (1<<sector)) == 0){
        FLASH_EraseInitTypeDef eraseinit = {
            .TypeErase = FLASH_TYPEERASE_SECTORS,
            .Banks = FLASH_BANK_1,
            .Sector = sector,
            .NbSectors = 1,
            .VoltageRange = FLASH_VOLTAGE_RANGE_3
        };

        uint32_t sector_error;
        if(HAL_FLASHEx_Erase(&eraseinit, &sector_error) == HAL_ERROR){
            return false;
        }

        sector_erased_bitmap |= (1<<sector);
    }

    return true;
}

bool flash_mass_erase(){
    HAL_Lock
}
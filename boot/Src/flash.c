#include "flash.h"
#include "stm32xx_hal.h"
#include <string.h>

static uint8_t sector_erased_bitmap = 0;

#define HANDLE_HAL_ERROR(x, handler) if((x) == HAL_ERROR){(handler); return false;}

bool exec_flash_command(volatile void* buf, flash_cmd_t* cmd){
    // Error check
    if(cmd->data_size > MAX_BUFFER_SIZE ||
        cmd->id >= NUM_IDS ||
        cmd->address < 0x08020000 || cmd->address > 0x0807FFFF){
        return false;
    }

    bool ret;
    
    HANDLE_HAL_ERROR(HAL_FLASH_Unlock(), NULL);

    if (cmd->id == FLASH_WRITE) {
        ret = flash_write(buf, cmd->address, cmd->data_size);
    } else if (cmd->id == FLASH_READ_SINGLE) {
        ret = flash_read_single(buf, cmd->address, cmd->data_size);
    } else if (cmd->id == FLASH_READ_BUF) {
        ret = flash_read_buf(buf, cmd->address, cmd->data_size);
    } else if (cmd->id == FLASH_ERASE) {
        ret = flash_erase(cmd->address);
    } else if (cmd->id == FLASH_MASS_ERASE) {
        ret = flash_mass_erase();
    }

    HANDLE_HAL_ERROR(HAL_FLASH_Lock(), NULL);

    return ret;
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
    HANDLE_HAL_ERROR(HAL_FLASH_Program(program_type, address, data), NULL);

    // verify
    if(memcmp((volatile void*)address, &data, data_size) != 0){
        // If the data is not the same, erase the sector and try again
        if(flash_erase(address) == false){
            return false;
        }

        ASSERT(HAL_FLASH_Program(program_type, address, data) == HAL_ERROR, NULL);

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
    while(data_size > 0){
        uint8_t write_size = 8;
        while((write_size != 0) && ((data_size&write_size) == 0)) write_size>>=1; // shift until we find most sig bit
        if(write_size == 0){
            return false;
        }

        if(flash_write_single(*(uint64_t*)buf, address, write_size) == false){
            flash_erase(address); // erase the sector the write fails on
            return false;
        }

        address += write_size;
        buf = ((uint8_t*)buf + write_size);
        data_size -= write_size;
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
        FLASH_EraseInitTypeDef erase_init = {
            .TypeErase = FLASH_TYPEERASE_SECTORS,
            .Banks = FLASH_BANK_1,
            .Sector = sector,
            .NbSectors = 1,
            .VoltageRange = FLASH_VOLTAGE_RANGE_3
        };

        uint32_t sector_error;
        HANDLE_HAL_ERROR(HAL_FLASHEx_Erase(&erase_init, &sector_error), NULL);
        
        sector_erased_bitmap |= (1<<sector);
    }

    return true;
}

bool flash_mass_erase(){
    FLASH_EraseInitTypeDef erase_init = {
        .TypeErase = FLASH_TYPEERASE_MASSERASE,
        .Banks = FLASH_BANK_1,
        .VoltageRange = FLASH_VOLTAGE_RANGE_3
    };

    uint32_t sector_error;
    HANDLE_HAL_ERROR(HAL_FLASHEx_Erase(&erase_init, &sector_error), NULL);

    return true;
}
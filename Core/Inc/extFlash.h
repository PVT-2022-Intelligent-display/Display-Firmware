/*
 * extFlash.h
 *
 *  Created on: Apr 3, 2022
 *      Author: Petr
 */

#ifndef INC_EXTFLASH_H_
#define INC_EXTFLASH_H_

#define FLASH_SIZE		8388608
#define PAGE_SIZE		256

#define PICTURE_START	4096
#define SECTOR_SIZE		4096

#define GENERAL_CONFIG_SECTOR 0
#define MAX_SECTOR 		2047
#define MAX_SCREEN_SECTOR 199
#define BITMAP_LIST_SECTOR 200


unsigned int ext_flash_readid();
unsigned char ext_flash_read_CR2V();
unsigned char ext_flash_read_status_register_1();
void ext_flash_wren();


void ext_flash_print_sector(unsigned int address);
void ext_flash_write_multipage(unsigned int address, unsigned char *buff, unsigned int len);


void ext_flash_write(unsigned int address, unsigned char *buff, unsigned int len);
void ext_flash_read(unsigned int address, unsigned char *buff, unsigned int len);
void ext_flash_erase_4kB(unsigned int address);

void ext_flash_continuous_read_begin(unsigned int address);
void ext_flash_continuous_read_read(unsigned char *buff, unsigned int len);
void ext_flash_continuous_read_skip(unsigned int len);
void ext_flash_continuous_read_finish();

void ext_flash_continuous_write_begin(unsigned int address);
void ext_flash_continuous_write_write(unsigned char *buff, unsigned int len);
void ext_flash_continuous_write_finish();




#endif /* INC_EXTFLASH_H_ */

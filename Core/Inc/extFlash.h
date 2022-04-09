/*
 * extFlash.h
 *
 *  Created on: Apr 3, 2022
 *      Author: Petr
 */

#ifndef INC_EXTFLASH_H_
#define INC_EXTFLASH_H_

#define FLASH_SIZE		8388608
#define PICTURE_START	4096
#define SECTOR_SIZE		4096


unsigned int ext_flash_readid();
unsigned char ext_flash_read_CR2V();
unsigned char ext_flash_read_status_register_1();
void ext_flash_wren();


void ext_flash_write(unsigned int sector_adress, char *buff, unsigned int len);
void ext_flash_read(unsigned int sector_adress, char *buff, unsigned int len);
void ext_flash_erase_4kB(unsigned int sector_adress);



#endif /* INC_EXTFLASH_H_ */

#ifndef _HAL_H_
#define _HAL_H_

/*******************************************************************************
* Include
*******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*******************************************************************************
* Define
*******************************************************************************/






/*******************************************************************************
* API
*******************************************************************************/

/**
  * @brief         open file
  * @param[in]     uint8_t buffer[]  : buffer have all byte of file
  * @param[in]     uint64_t size     : size of file
  * @retval        if open success return true, if not return false
  */
bool hal_init(uint8_t* file_name);

/**
  * @brief         update numbers of byte in a sector
  * @param[in]     uint32_t bytes_of_sector : numbers byte in a sector
  * @retval        None
  */
void update_byte_of_sector(const uint32_t bytes_of_sector);

/**
  * @brief         read a sector from file
  * @param[inout]  uint8_t* buffer : buffer to hold all byte of a sector
  * @param[in]     uint16_t index_sector : index of sector in file
  * @retval        return buffer hold all byte of a sector
  */
bool hal_read_sector(uint8_t* buffer, const uint16_t index_sector);

/**
  * @brief         
  * @param[inout]  uint8_t* buffer : buffer to hold all byte of a sector
  * @param[in]     uint16_t index_sector : index of sector in file
  * @param[in]     uint8_t numof_sector  : number of sectors
  * @retval        return buffer hold all byte of multi sector
  */
bool hal_read_multi_sector(uint8_t* buffer, const uint16_t index_sector,\
const uint8_t numof_sector);

#endif /*_HAL_H_*/
/*******************************************************************************
* EOF
*******************************************************************************/

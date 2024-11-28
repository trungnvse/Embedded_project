/*******************************************************************************
* Include
*******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "..\inc\hal.h"


/*******************************************************************************
* Global variable
*******************************************************************************/
static  FILE*         g_ptr_file = NULL;
static  uint32_t      g_bytes_sector =  0;

/*******************************************************************************
* Code
*******************************************************************************/

bool hal_init(uint8_t* file_name)
{
	bool relvar = true;

	g_ptr_file = fopen(file_name, "rb");
	if(g_ptr_file != NULL)
	{
		relvar = true;
	}else{
		relvar = false;
	}

	return relvar;
}


void update_byte_of_sector(const uint32_t bytes_of_sector)
{
	g_bytes_sector = bytes_of_sector;
}

bool hal_read_sector(uint8_t* buffer,const uint16_t index_sector)
{
	uint32_t numsof_readed = 0;
	bool relvar = true;
	
    fseek(g_ptr_file, g_bytes_sector*index_sector , SEEK_SET);
    numsof_readed = fread(buffer,1,g_bytes_sector,g_ptr_file);
    
    if(numsof_readed == g_bytes_sector)
    {
    	relvar = true;
	} else{
		relvar = false;
	}
	
	return relvar;
}


bool hal_read_multi_sector(uint8_t* buffer, const uint16_t index_sector,\
const uint8_t numof_sector)
{
	uint32_t numsof_readed = 0;
	bool relvar = true;
	
    fseek(g_ptr_file, g_bytes_sector*index_sector , SEEK_SET);
    fread(buffer,1,g_bytes_sector*numof_sector ,g_ptr_file);
    
    if(numsof_readed == g_bytes_sector)
    {
    	relvar = true;
	} else{
		relvar = false;
	}

	return relvar;
}

/*******************************************************************************
* EOF
*******************************************************************************/

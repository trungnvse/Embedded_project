/*******************************************************************************
* Include
*******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "..\inc\fat.h"
#include "..\..\hal\inc\hal.h"

/*******************************************************************************
* Define
*******************************************************************************/
#define OFFSET_BOOT_BPB_BytsPerSec   11
#define OFFSET_BOOT_BPB_SecPerClus   13
#define OFFSET_BOOT_BPB_RsvdSecCnt   14
#define OFFSET_BOOT_BPB_BytsPerSec   11
#define OFFSET_BOOT_BPB_NumFATs      16
#define OFFSET_BOOT_BPB_RootEntCnt   17
#define OFFSET_BOOT_BPB_FATSz16      22


#define OFFSET_DIRECTORY_SIZE        28
#define OFFSET_DIRECTORY_YEAR        24
#define OFFSET_DIRECTORY_HOUR        22
#define OFFSET_DIRECTORY_EXTENT       8
#define OFFSET_DIRECTORY_ATTRI       11
#define OFFSET_FIR_CLUSTER           26
#define OFFSET_NAME_LOW_OR_UP        12

#define OFFSET_LONG_NAME_1            1
#define OFFSET_LONG_NAME_2           14
#define OFFSET_LONG_NAME_3           28

#define BYTESOF_ENTRY                32

/*******************************************************************************
* Prototype
*******************************************************************************/

/* BOOT */

/**
  * @brief         read infor from sector 0 in file
  * @param[inout]  uint8_t* start_fat        : start of fat table
  * @param[inout]  uint8_t* start_root       : start of root
  * @param[inout]  uint8_t* start_data       : start of data
  * @param[inout]  uint32_t* bytes_of_sector : number of bytes in a sector
  * @param[inout]  uint8_t* sec_per_cluster  : number sector in a scluster
  * @retval        return sector start of fat table, start of root, start of data
  */
void fat_read_boot();

/* this function is an option, it is not necessary */
void creat_fat_table(uint16_t* fat);


/* ROOT */

void creat_entry_extra_list(node_entry_extra_t**  head_entry_extra, uint8_t  entry_buff[]);

/**
  * @brief         read name from head of entry extra
  * @param[in]     node_entry_extra_t* head_entry_extra :
                   pointer head of list entry extra
  * @retval        string name
  */
uint8_t* read_long_name(node_entry_extra_t* head_entry_extra);

/**
  * @brief         read name from head of entry extra
  * @param[in]     uint8_t entry_buff[32] : read from buffer to entry
  * @retval        entry had information
  */
entry_t  read_infor_entry(const uint8_t entry_buff[32]);


/* FOLDER */


/* use for both of FILE and FOLDER */
/**
  * @brief         find value of next element in fat
  * @param[in]     uint16_t index_cluster : index of next element in fat
  * @param[in]     uint8_t buffer[] : index of next element in fat
  * @retval        value of next element in fat
  */
uint16_t value_cluster(const uint16_t index_cluster);


/**
  * @brief         change index of element to bytes in fat table
  * @param[in]     uint16_t index_cluster : read index of element
  * @retval        index byte of element in fat table
  */
uint16_t index_to_byte_cluster(const uint16_t index_cluster);

/**
  * @brief         find index sector of element
  * @param[in]     uint16_t index_cluster : read index of element
  * @retval        index sector of element
  */
uint16_t index_to_sector_cluster(const uint16_t index_cluster);

/* use for both of ROOT and FOLDER */
/**
  * @brief         creat list element in current folder
  * @param[in]     uint8_t  buffer[]   : buffer have elements
  * @param[inout]  node_entry_t** head : pointer to pointer to head of list element
  * @retval        index sector of element
  */
void creat_list_in_folder(const uint8_t  buffer[], node_entry_t** head);


/**
  * @brief         read folder from first element and create a list element
  * @param[in]     uint16_t fir_cluster  : index of first element
  * @retval        pointer to list element
  */
node_entry_t* read_folder(const uint16_t fir_cluster);


/**
  * @brief         print all byte of buffer
  * @param[in]     uint8_t buffer[]  : buffer
  * @param[in]     uint16_t index_sector  : index of sector that need read
  * @retval        pointer to list element
  */
void print_sector(const uint8_t buffer[], const uint16_t index_sector);



/* FILE */


/**
  * @brief         find index sector of element
  * @param[in]     uint16_t index_cluster : read index of element
  * @retval        index sector of element
  */
uint16_t index_to_byte_cluster_file(const uint16_t index_cluster);

/**
  * @brief         change index of element to bytes in fat table
  * @param[in]     uint16_t index_cluster : read index of element
  * @retval        index byte of element in fat table
  */
uint16_t index_to_sector_cluster_file(const uint16_t index_cluster);

/**
  * @brief         print all byte of buffer
  * @param[in]     uint8_t buffer[]  : buffer
  * @param[in]     uint16_t index_sector  : index of sector that need read
  * @retval        pointer to list element
  */
void print_sector_file(const uint8_t buffer[], const uint16_t index_sector);



/*******************************************************************************
* Global variable
*******************************************************************************/

uint8_t  start_fat  = 0;
uint8_t  start_root = 0;
uint8_t  start_data = 0;
uint32_t bytes_of_sector = 0;
uint8_t  sec_per_cluster = 0;
uint16_t fat_size   = 0;
//uint32_t numof_fat = 170*9;
//uint16_t fat[170*9];

/*******************************************************************************
* Code
*******************************************************************************/



/* INIT */
bool fat_init(uint8_t* file_name)
{
	bool relvar = true;
	if(hal_init(file_name))
	{
		update_byte_of_sector(512);
	    fat_read_boot();
		update_byte_of_sector(bytes_of_sector);

	    printf("-----------------------------------------------------------\n");
		relvar = true;
	}else{
		relvar = false;
	}

	return relvar;
}

/* BOOT */

void fat_read_boot()
{
    FILE*    ptr_file = NULL;
    uint16_t BPB_BytsPerSec = 0; //11 2 byte
    uint8_t  BPB_SecPerClus = 0; //13 1 byte
    uint16_t BPB_RsvdSecCnt = 0; //14 2 byte
    uint8_t  BPB_NumFATs    = 0; //16 1 byte
    uint16_t BPB_RootEntCnt = 0; //17 2 byte
    uint16_t BPB_FATSz16    = 0; //22 2 byte
    uint8_t  buffer[512];

    hal_read_sector(buffer, 0);


    BPB_BytsPerSec = buffer[OFFSET_BOOT_BPB_BytsPerSec] | \
	(((uint16_t)buffer[OFFSET_BOOT_BPB_BytsPerSec + 1] & 0X00FF) << 8);
	
    BPB_RootEntCnt = buffer[OFFSET_BOOT_BPB_RootEntCnt] | \
	(((uint16_t)buffer[OFFSET_BOOT_BPB_RootEntCnt + 1] & 0X00FF) << 8);
    
    BPB_RsvdSecCnt = buffer[OFFSET_BOOT_BPB_RsvdSecCnt] | \
	(((uint16_t)buffer[OFFSET_BOOT_BPB_RsvdSecCnt + 1] & 0X00FF) << 8);
	
    BPB_FATSz16    = buffer[OFFSET_BOOT_BPB_FATSz16] | \
	(((uint16_t)buffer[OFFSET_BOOT_BPB_FATSz16 + 1] & 0X00FF) << 8);
	
    BPB_SecPerClus = buffer[OFFSET_BOOT_BPB_SecPerClus];
    BPB_NumFATs    = buffer[OFFSET_BOOT_BPB_NumFATs];

    start_fat  = BPB_RsvdSecCnt;
    start_root = BPB_RsvdSecCnt + BPB_SecPerClus * BPB_NumFATs * BPB_FATSz16;
    start_data = BPB_RootEntCnt * 32 / BPB_BytsPerSec + start_root;
    bytes_of_sector = BPB_BytsPerSec;
    sec_per_cluster = BPB_SecPerClus;
    fat_size        = BPB_FATSz16;
}




/* ROOT */


node_entry_t*  fat_read_root()
{
    node_entry_t*           root = NULL;

	uint8_t                 buffer[bytes_of_sector];
	uint16_t                index = 0;
	entry_t                 entry_current;
	entry_extra_t           entry_extra_current;
	node_entry_t*           ptr_temp_main = NULL;
	uint8_t                 number_of_sector = 0;
	node_entry_extra_t*     head_entry_extra = NULL;
	node_entry_extra_t*     ptr_temp = NULL;
    uint8_t                 entry_buff[BYTESOF_ENTRY];
    uint8_t                 number_of_entry = 0;

    /* read sector begin root into buffer */
    hal_read_sector(buffer, start_root);
    number_of_sector = 0;

    while(buffer[0] != 0x00)
    {
    	creat_list_in_folder(buffer, &root);
	    number_of_sector++;
	    hal_read_sector(buffer, start_root + number_of_sector);
    }

	/* NOTE: function read_root return variable "root" */
	return root;
}

void creat_entry_extra_list(node_entry_extra_t**  head_entry_extra, uint8_t  entry_buff[])
{
	uint8_t index = 0;
	entry_extra_t  entry_extra_current;
	node_entry_extra_t*     ptr_temp = NULL;
	/* offset 1    5 character , 1 character = 2 byte*/
    for(index = OFFSET_LONG_NAME_1; index < OFFSET_LONG_NAME_1 + 10; index++)
    {
        if((index % 2) != 0)
        {
            entry_extra_current.name_1[(index-1)/2] = entry_buff[index];
        }
    }
    entry_extra_current.name_1[5] = '\0';

    /* offset 14    6 character , 1 character = 2 byte*/
    for(index = OFFSET_LONG_NAME_2; index < OFFSET_LONG_NAME_2 + 12; index++)
    {
        if((index % 2) == 0)
        {
            entry_extra_current.name_2[(index-14)/2] = entry_buff[index];
        }
    }
    entry_extra_current.name_2[6] = '\0';

    /* offset 28    2 character , 1 character = 2 byte*/
    for(index = OFFSET_LONG_NAME_3; index < OFFSET_LONG_NAME_3 + 4; index++)
    {
        if((index % 2) == 0)
        {
            entry_extra_current.name_3[(index-28)/2] = entry_buff[index];
        }
    }
    entry_extra_current.name_3[2] = '\0';

    /* add node entry extra character to list entry extra */
    node_entry_extra_t* entry_extra_ptr = NULL;
    entry_extra_ptr = (node_entry_extra_t*) malloc(sizeof(node_entry_extra_t));

    entry_extra_ptr->entry_extra = entry_extra_current;
    entry_extra_ptr->next = (*head_entry_extra);

    (*head_entry_extra) = entry_extra_ptr;
	
}

entry_t read_infor_entry(const uint8_t entry_buff[32])
{
    entry_t        entry_current;
    uint8_t  index  = 0;
    uint16_t temp   = 0;
    
    for(index = OFFSET_DIRECTORY_EXTENT; index < OFFSET_DIRECTORY_EXTENT + 3; index++)
    {
        entry_current.extent[index - OFFSET_DIRECTORY_EXTENT] = entry_buff[index];
    }
    entry_current.extent[3] = '\0';
    
    entry_current.status_attri = entry_buff[OFFSET_DIRECTORY_ATTRI];

    entry_current.size = entry_buff[OFFSET_DIRECTORY_SIZE] |\
	(entry_buff[OFFSET_DIRECTORY_SIZE + 1]<<8) | \
    (entry_buff[OFFSET_DIRECTORY_SIZE + 2]<<16) | \
	(entry_buff[OFFSET_DIRECTORY_SIZE + 3]<<24);

    entry_current.size  = entry_current.size;

    temp = entry_buff[OFFSET_DIRECTORY_YEAR] |\
	(entry_buff[OFFSET_DIRECTORY_YEAR + 1] << 8);
	
    entry_current.year  = ((temp & 0xFE00) >> 9) +1980;
    entry_current.month = (temp & 0x1E0) >> 5;
    entry_current.day = (temp & 0x1F);

    temp = entry_buff[OFFSET_DIRECTORY_HOUR] |\
	(entry_buff[OFFSET_DIRECTORY_HOUR + 1] << 8);
	
    entry_current.hour   = (temp & 0xF800) >> 11;
    entry_current.minute = (temp & 0x7E0) >> 5;
    entry_current.secord = (temp & 0x1F);

    if(entry_current.status_attri == 0x00)
    {
        entry_current.folder = false;
    }else{
        entry_current.folder = true;
    }

    entry_current.index_fir_cluster = entry_buff[OFFSET_FIR_CLUSTER] |\
	(entry_buff[OFFSET_FIR_CLUSTER + 1] << 8);

    return entry_current;
}


uint8_t* read_long_name(node_entry_extra_t* head_entry_extra)
{
    node_entry_extra_t*  ptr_temp = NULL;

    uint8_t  len_name = 0;
    static uint8_t  root_dir_name[100];
    uint8_t  index = 0;

    ptr_temp = head_entry_extra;

    while (ptr_temp != NULL) {
        index = 0;
        while(ptr_temp->entry_extra.name_1[index] != '\0')
        {
            if((ptr_temp->entry_extra.name_1[index] ) &&\
            (ptr_temp->entry_extra.name_1[index] < 0x7E))
            {
                root_dir_name[index + len_name] = ptr_temp->entry_extra.name_1[index];
            } else {
                len_name--;
            }

            if(root_dir_name[index + len_name] == '.')
            {
                root_dir_name[index + len_name] = '\0';
            }

            index++;
        }


        len_name = len_name + index;
        index = 0;
        while(ptr_temp->entry_extra.name_2[index] != '\0')
        {
            if((ptr_temp->entry_extra.name_2[index] ) &&\
            (ptr_temp->entry_extra.name_2[index] < 0x7E))
            {
                root_dir_name[index + len_name] = ptr_temp->entry_extra.name_2[index];
            }else {
                len_name--;
            }

            if(root_dir_name[index + len_name] == '.')
            {
                root_dir_name[index + len_name] = '\0';
            }

            index++;
        }

        len_name = len_name + index;
        index = 0;
        while(ptr_temp->entry_extra.name_3[index] != '\0')
        {
            if((ptr_temp->entry_extra.name_3[index] ) &&\
            (ptr_temp->entry_extra.name_3[index] < 0x7E))
            {
                root_dir_name[index + len_name] = ptr_temp->entry_extra.name_3[index];
            }else {
                len_name--;
            }

            if(root_dir_name[index + len_name] == '.')
            {
                root_dir_name[index + len_name] = '\0';
            }

            index++;
        }

        len_name = len_name + index;

        ptr_temp = ptr_temp->next;
    }
    root_dir_name[index + len_name] = '\0';

    return root_dir_name;
}


/* PRINT LIST ROOT */
void print_root_list(node_entry_t* root)
{
    node_entry_t* current = root;
    uint8_t index = 1;

    if(root != NULL)
    {
        while (current != NULL) {
        if(!current->entry.folder){
           printf("%-2d %-70s %-3s    %-8d     %2d/%2d/%4d   %2d:%2d:%2d",\
		   index,\
           current->entry.name, current->entry.extent, current->entry.size,\
           current->entry.month, current->entry.day, current->entry.year,\
           current->entry.hour, current->entry.minute, current->entry.secord);
           printf("\n");
        } else{
           printf("%-2d %-70s                     %2d/%2d/%4d   %2d:%2d:%2d",\
		   index,\
           current->entry.name,\
           current->entry.month, current->entry.day, current->entry.year, \
           current->entry.hour, current->entry.minute, current->entry.secord);
           printf("\n");
        }

        current = current->next;
        index++;
        }
        printf("-----------------------------------------------------------\n");
    } else{
        printf("head of list NULL");
    }


}




/* FOLDER */


void  fat_read_folder(node_entry_t**  head,const node_entry_t* current)
{
    uint16_t             fir_cluster     = 0;

    //printf("%s\n", current->entry.name);
    fir_cluster = current->entry.index_fir_cluster;
//    if((*head)->entry.index_fir_cluster != 0x03)
//    {
//        free(*head);
//    }
    *head = read_folder(fir_cluster);

}

node_entry_t* read_folder(const uint16_t fir_cluster)
{
    node_entry_t*   head    = NULL;
    uint8_t    buffer_fat[bytes_of_sector]; /* = 512 */
    uint8_t    buffer_data[bytes_of_sector * sec_per_cluster];
    uint16_t   index_byte      = 0;
    uint16_t   index_sector    = 0;
    uint16_t   current_cluster = fir_cluster;
	//uint16_t   fat[bytes_of_sector * fat_size];
	//creat_fat_table(fat);

    do
    {
		/* update index_sector is sector of data */
        index_sector = current_cluster + start_data -2;
        
        hal_read_multi_sector(buffer_data, index_sector, sec_per_cluster);

        creat_list_in_folder(buffer_data, &head);

		/* update next cluster */
        current_cluster = value_cluster(current_cluster);
        //current_cluster = fat[current_cluster];

    } while (current_cluster != 0xFFF);

    return head;
}


void print_list(node_entry_t* head)
{
    node_entry_t* current = head;
    uint8_t index = 0;
    index = 1;

    while (current != NULL) {
        if(!current->entry.folder){
           printf("%-2d %-70s %-3s    %-8d     %2d/%2d/%4d   %2d:%2d:%2d",\
		   index,\
           current->entry.name, current->entry.extent, current->entry.size,\
           current->entry.month, current->entry.day, current->entry.year,\
           current->entry.hour, current->entry.minute, current->entry.secord);
           printf("\n");
        } else{
           printf("%-2d %-70s                     %2d/%2d/%4d   %2d:%2d:%2d",\
		   index,\
           current->entry.name,\
           current->entry.month, current->entry.day, current->entry.year,\
           current->entry.hour, current->entry.minute, current->entry.secord);
           printf("\n");
        }

        current = current->next;
        index++;
    }
    printf("------------------------------------------------------\n");
}

void creat_list_in_folder(const uint8_t  buffer[],node_entry_t** head)
{
    entry_t             entry_current;
    uint8_t             number_of_entry = 0;
    uint8_t             entry_buff[BYTESOF_ENTRY];
    uint16_t            index = 0;
    node_entry_t*       entry_ptr = NULL;
    static node_entry_extra_t* head_entry_extra = NULL;
    
    uint8_t             name_up = 0;

    while(( (number_of_entry * BYTESOF_ENTRY) < (*(&buffer + 1) - buffer))\
	&& (buffer[number_of_entry * BYTESOF_ENTRY] != 0x00) )
    {
        for(index = number_of_entry * BYTESOF_ENTRY;\
		index < number_of_entry * BYTESOF_ENTRY + BYTESOF_ENTRY; index++)
        {
            entry_buff[index - number_of_entry * BYTESOF_ENTRY] = buffer[index];
        }
        number_of_entry++;
		if(entry_buff[OFFSET_DIRECTORY_ATTRI] != 0x0F)
		{
			/* READ INFORMATION WITHOUT NAME */
            entry_current = read_infor_entry(entry_buff);

            /* READ NAME */

            if(head_entry_extra == NULL) /* have zero entry extra before it*/
            {
                /* SHORST FILE NAME */

                if(entry_buff[OFFSET_NAME_LOW_OR_UP] == 0x08)
                {
                	name_up = 'a' - 'A';
				}

                for(index = 0; index < 8; index++)
                {
                	if(entry_buff[index] != 0x20)
                	{
                		entry_current.name[index] = entry_buff[index] + name_up;
					} else{
						entry_current.name[index] = entry_buff[index];
					}
                }

                entry_current.name[8] = '\0';

            } else {
               /* LONG FILE NAME */
                memcpy(entry_current.name, read_long_name(head_entry_extra), 100);
            }

            if(head_entry_extra != NULL)
            {
                free(head_entry_extra);
                head_entry_extra = NULL;
            }
            /* --------------------------------------------*/

            // Add entry to list
            if( NULL == *head )
	        {
	            (*head) = (node_entry_t*)malloc(sizeof(node_entry_t));
	            (*head)->entry = entry_current;
	            (*head)->next = NULL;

	        } else{
	            entry_ptr = (*head);
	            while (entry_ptr->next != NULL)
	            {
	                entry_ptr = entry_ptr->next;
	            }
	            entry_ptr->next = (node_entry_t*)malloc(sizeof(node_entry_t));

	            entry_ptr->next->entry = entry_current;
	            entry_ptr->next->next = NULL;

	        }
        } else {
            /* add an entry extra into list entry extra*/
			creat_entry_extra_list(&head_entry_extra, entry_buff);

        }
	}
}


uint16_t index_to_byte_cluster(const uint16_t index_cluster)
{
    uint16_t index_byte = 0;
    index_byte = (index_cluster + (index_cluster)/2) % bytes_of_sector;

    return index_byte;
}

uint16_t index_to_sector_cluster(const uint16_t index_cluster)
{

    uint16_t index_sector = 0;
    index_sector = (index_cluster + (index_cluster)/2) / bytes_of_sector + 1;

    return index_sector;
}

void print_sector(const uint8_t buffer[], const uint16_t index_sector)
{
    uint16_t index = 0;
    printf("\n %d th sector: \n", index_sector);
    for(index = 0; index < bytes_of_sector; index++)
    {
        if(index % 16 == 0)
        {
            printf("\n");
        }
        printf("%2x  ", buffer[index]);
    }
    printf("\n");
}






/* FILE */


uint8_t* fat_read_file(node_entry_t*  file, uint64_t*  size_buff)
{
    uint8_t    buff_fat[bytes_of_sector];
    uint8_t    buff_data[bytes_of_sector * sec_per_cluster];
    uint32_t   bytesof_cluster = bytes_of_sector * sec_per_cluster; /* 512 */
    uint16_t   fir_cluster  = 0;
    uint16_t   entry_fat = 0;
    uint8_t    numof_file   = 0;
    uint16_t   index_sector = 0;
    uint16_t   index_byte   = 0;
    uint64_t   index = 0;
    uint32_t   numof_sector = 0;
    uint32_t   bytes_cluster = 0;
    
    /* create fat table */
//    uint16_t   fat[bytes_of_sector * fat_size];
//	creat_fat_table(fat);

    uint64_t   size = 0;
    node_entry_t*   file_current;
    file_current = file;

    size = file_current->entry.size;
    *size_buff = size;
    /* return this variable */
    uint8_t*   buff_final = NULL;
    buff_final = (uint8_t*)malloc(size);
    
    if(buff_final != NULL)
    {
    	hal_read_sector(buff_fat, 1);

	    fir_cluster = file_current->entry.index_fir_cluster;
	    entry_fat = fir_cluster;

	    //printf("%s\n\n", file_current->entry.name);
	    do
	    {
	        /* update index sector of data */
	        index_sector = entry_fat + start_data -2;
	        hal_read_multi_sector(buff_data, index_sector, sec_per_cluster);

	        /* copy to buff final */
	        for(index = 0; index < bytesof_cluster; index++)
	        {
	            buff_final[index + numof_sector * bytesof_cluster] = buff_data[index];
	            if((index + numof_sector * bytesof_cluster +1) == size)
	            {
	                index = bytesof_cluster;  /* break for loop */
	            }
	        }
	        /* update next cluster */
			entry_fat = value_cluster(entry_fat);

	        //entry_fat = fat[entry_fat];
	        numof_sector++;

	    } while (entry_fat != 0xFFF);
	}

    return buff_final;
}

void creat_fat_table(uint16_t* fat)
{
    uint32_t size = 0;
    uint32_t index_byte = 0;
    uint32_t index_cluster = 0;
    uint16_t value = 0;

    size = bytes_of_sector * fat_size;
    uint8_t buff[size];

    hal_read_multi_sector(buff, 1, fat_size);

    for(index_cluster = 0; index_cluster < size; index_cluster++)
    {
        index_byte = index_cluster + index_cluster/2;
        if(index_cluster % 2 == 0)
        {
            value = buff[index_byte] | ((uint16_t)(buff[index_byte + 1] & 0x0F) << 8);
        } else{
            value = (buff[index_byte] >> 4) | (uint16_t)(buff[index_byte + 1] << 4);
        }

        fat[index_cluster] = value;
    }
}


void print_buff(const uint8_t buffer[], const uint64_t size)
{
    uint64_t index = 0;
    for(index = 0; index < size; index++)
    {
        if(index % 16 == 0)
        {
            printf("\n");
        }
        printf("%2x  ", buffer[index]);
    }
    printf("\n");
}

void print_buff_to_text(const uint8_t buffer[], const uint64_t size)
{
	uint64_t index = 0;

    for(index = 0; index < size; index++)
    {
	    printf("%c", buffer[index]);
    }
    printf("\n");
}

void print_sector_file(const uint8_t buffer[], const uint16_t index_sector)
{
    uint16_t index = 0;
    printf("\n %d th sector: \n", index_sector);
    for(index = 0; index < 512; index++)
    {
        if(index % 16 == 0)
        {
            printf("\n");
        }
        printf("%2x  ", buffer[index]);
    }
    printf("\n");
}

uint16_t value_cluster(const uint16_t index_cluster)
{
	uint8_t     buffer[bytes_of_sector];
	uint8_t     buff_next[bytes_of_sector];
    uint16_t    index_byte = 0;
    uint16_t    value = 0;
    uint16_t    index_byte_to_check = 0;
    uint16_t    index_sector = 0;
    index_byte      = (index_cluster + (index_cluster)/2) % bytes_of_sector;
    index_sector    = (index_cluster + (index_cluster)/2) / bytes_of_sector + 1;
    
	hal_read_sector(buffer, index_sector);
    
	if((index_byte % 511 == 0) && (index_sector  % 3 != 0) )
	{
		hal_read_sector(buff_next, index_sector + 1);
		if(index_sector  % 3 == 1)
		{
			value = (buffer[index_byte] >> 4) | (((uint16_t)buff_next[0]) << 4);
		} else{
			value = buffer[index_byte] | ((uint16_t)(buff_next[0] & 0x0F) << 8);
		}
	} else{
		if(index_cluster % 2 == 0)
	    {
	        value = buffer[index_byte] | ((uint16_t)(buffer[index_byte + 1] & 0x0F) << 8);
	    } else{
	        value = (buffer[index_byte] >> 4) | (((uint16_t)buffer[index_byte + 1]) << 4);
	    }
	}

    return value;
}

uint16_t index_to_byte_cluster_file(const uint16_t index_cluster)
{
    uint16_t index_byte = 0;
    index_byte = (index_cluster + (index_cluster)/2) % bytes_of_sector;

    return index_byte;
}

uint16_t index_to_sector_cluster_file(const uint16_t index_cluster)
{

    uint16_t index_sector = 0;
    index_sector = (index_cluster + (index_cluster)/2) / bytes_of_sector + 1;

    return index_sector;
}

/*******************************************************************************
* EOF
*******************************************************************************/








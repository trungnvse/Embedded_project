#ifndef _FAT_H_
#define _FAT_H_

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

typedef struct entry{
    uint8_t    name[100];
    uint8_t    extent[4];
    uint8_t    status_attri;
    int8_t     day;
    int8_t     month;
    uint16_t   year;
    int8_t     hour;
    int8_t     minute;
    int8_t     secord;
    uint32_t   size;
    bool       folder;
    uint16_t   index_fir_cluster;
} entry_t;

typedef struct node_entry{
    entry_t           entry;
    struct node_entry* next;
} node_entry_t;

/* Use for ROOT */
typedef struct entry_extra{
    uint8_t    name_1[6];
    uint8_t    name_2[7];
    uint8_t    name_3[3];
} entry_extra_t;

typedef struct node_entry_extra{
    entry_extra_t  entry_extra;
    struct node_entry_extra* next;
} node_entry_extra_t;


/* Use for FILE */
typedef struct node_cluster{
    uint8_t value;
    struct fir_cluster* next;
} node_cluster_t;





/*******************************************************************************
* API
*******************************************************************************/

/* INIT */
/**
  * @brief         open file
  * @param[in]     uint8_t* file_name  : pointer to string of name file
  * @retval        check true if open file success, if not return false
  */
bool fat_init(uint8_t* file_name);

/* ROOT */

/**
  * @brief         create list element of root directory
  * @retval        pointer to list element of root directory
  */
node_entry_t*  fat_read_root();

/**
  * @brief         print all element from root directory
  * @param[in]     node_entry_t* root  : pointer to head of list root
  * @retval        None
  */
void print_root_list(node_entry_t* root);



/* FOLDER */

/**
  * @brief         create new list have current element is first element
  * @param[in]     node_entry_t**  head  : pointer to pointer to list
  * @param[in]     node_entry_t* current : pointer to current element
  * @retval        pointer to pointer to new list
  */
void  fat_read_folder(node_entry_t**  head, const node_entry_t* current);

/**
  * @brief         print list element
  * @param[in]     node_entry_t* head  : pointer to list
  * @retval        None
  */
void print_list(node_entry_t* head);

/* Use for FILE */

/**
  * @brief         print all byte of file
  * @param[in]     uint8_t buffer[]  : buffer have all byte of file
  * @param[in]     uint64_t size     : size of file
  * @retval        None
  */
void print_buff(const uint8_t buffer[], const uint64_t size);

/**
  * @brief         print all byte of file follow ASCII
  * @param[in]     uint8_t buffer[]  : buffer have all byte of file
  * @param[in]     uint64_t size     : size of file
  * @retval        None
  */
void print_buff_to_text(const uint8_t buffer[], const uint64_t size);

/**
  * @brief         create pointer to adress where hold all byte of file
  * @param[in]     node_entry_t*   file  : pointer to current file
  * @param[in]     uint64_t*   size_buff : pointer to variable size of file
  * @retval        pointer to adress where hold all byte of file
  */
uint8_t* fat_read_file(node_entry_t*  file, uint64_t*   size_buff);

#endif /*_FAT_H_*/
/*******************************************************************************
* EOF
*******************************************************************************/

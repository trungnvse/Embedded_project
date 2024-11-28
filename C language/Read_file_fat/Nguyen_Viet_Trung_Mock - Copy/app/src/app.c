/*******************************************************************************
* Code
*******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "..\..\fat\inc\fat.h"

/*******************************************************************************
* Prototype
*******************************************************************************/

/**
  * @brief         Check input is number or not
  * @param[in]     const char *str     : pointer to the string input
  * @retval        true if input is number , if not return false
  */
bool isNumeric(const char *str);


/*******************************************************************************
* Code
*******************************************************************************/

int main()
{
    node_entry_t*           root = NULL;
    node_entry_t*           head = NULL;
    node_entry_t*           current = NULL;
    uint8_t                 index   = 0;
    uint64_t                choice  = 0;
    uint8_t*    buff_final;
    uint64_t    size_buff;
    uint8_t*    endptr;
    uint8_t  input[10];

    /* fat_init */
	if(fat_init("floppy.img"))
	{

		/* root */
		root = fat_read_root();
        print_root_list(root);
	    /* read folder or read file */
	    head = root;
	    do{
	    	do{
	    		current = head;
	    		do{
	    			printf("choice: \n");
			        fgets(input, sizeof(input), stdin);
			        if (input[strlen(input) - 1] == '\n')
					{
                        input[strlen(input) - 1] = '\0';
                    }
				} while(!isNumeric(input));
		        choice = atoi(input);
		        system("cls");
		        for(index = 1; index < choice; index++)
		        {
		            current = current->next;
		            if(current == NULL)
		            {
		            	printf("don't have file/folder! enter again!\n");
		            	print_list(head);
		            	index = choice;
					}
		        }
			} while(current == NULL);


	        if(current->entry.index_fir_cluster == 0x00)
	        {
	            print_root_list(root);
	            free(head);
	            head = root;
	        } else{
	           if(current->entry.folder)
	            {
	                /* read folder */
	                fat_read_folder(&head, current);
	                print_list(head);
	            } else{
	                /* read file */
	                buff_final = fat_read_file(current, &size_buff);
	                if(buff_final != NULL)
	                {
	                	printf("content of file:\n");
						print_buff(buff_final, size_buff);
						if(strcmp(current->entry.extent,"TXT") == 0)
						{
							print_buff_to_text(buff_final, size_buff);
						}
		                printf("-----------------------------------------------------------\n");
		                free(buff_final);
					} else{
						printf("read file false!\n");
					}
					print_list(head);
	            }
	        }

	    } while(true);
	} else{
		printf("cant open file");
	}

    return 0;
}


bool isNumeric(const char *str)
{
	bool relvar  = true;
	uint8_t index = 0;
    int len = strlen(str);
    for (index = 0; index < len; index++) {
        if (!isdigit(str[index]))
		{
            relvar  = false; /* Return false if any character is not a digit */
        }
    }
    return relvar; /* Return true if all characters are digits */
}

/*******************************************************************************
* EOF
*******************************************************************************/

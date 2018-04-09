/****************************************************************************
 * dictionary.c
 * Implements a dictionary's functionality.
 ***************************************************************************/

#include <stdbool.h>
#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "dictionary.h"

//create hash table
struct node* head[LENGTH][26];

//size of dictionary in words
int DictSize = 0;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    //get hash index of word
    int index = strlen(word);
    int index2 = (int)tolower(word[0]);
    
    node* curr = head[index][index2];    
    
    while(curr != NULL)    
    {
    	if (strcasecmp(curr->word, word) == 0)
    		return true;
    	else curr = curr->next;   
    }    
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */

bool load(const char* dictionary)
{
	//open dictionary
    FILE* dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        printf("Could not open %s during load\n", dictionary);
        return false;
    }
    
    //initialize heads of hash table as NULL
    for (int i = 0; i< LENGTH; i++)
	{
		for (int j = 0; j < 26; j++)
		{
			head[i][j] = NULL;
		}
	}

    
    //hash indexes
    int index = 0;
    int index2 = 0;
    
    //hold read string
    char str[LENGTH+1];
    
	for (int c = fgetc(dict); c != EOF; c = fgetc(dict))
	{
    	if (isalpha(c) || (c == '\'' && index > 0))
    	{
    		str[index] = c;
    		index++;    	
    	}  
    	  		
        else if (index > 0)
        {
            // terminate current word
            str[index] = '\0';
            index2 = (int)str[0];
            
            //create new node and insert into linked list as first
    		node* new_node = malloc(sizeof(node));
    		if (new_node == NULL)
    		{
    			printf("out of memory\n");
        		return false;
       		}
       		for (int i = 0; i < index +1; i++)
       		{
    		new_node->word[i] = str[i];
    		}
    		
    		if (head[index][index2] == NULL)
    		{
        		head[index][index2] = new_node;
        		new_node->next = NULL;
    		}
    	
    		else
    		{
    			new_node->next = head[index][index2];
    			head[index][index2] = new_node;
    		}
    		DictSize++;
    		index = 0;
                        
        }

        
	}

	fclose(dict);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return DictSize;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
	for (int index = 0; index < LENGTH; index++)
	{
		for (int index2 = 0; index2 < 26; index2++)
		{
    		if (head[index][index2] != NULL)
    		{
        		node* next = head[index][index2]->next;
        		free(head[index][index2]);
        
        		while (next != NULL)
        		{            
            		head[index][index2] = next;
            		next = head[index][index2]->next;
            		free(head[index][index2]);
       			}
        
    		}
    	}
    }
    return true;
}

#include "inverted_search.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int get_index(char *word)
{
    if (word[0] >= 'A' && word[0] <= 'Z')
        return word[0] - 'A';

    if (word[0] >= 'a' && word[0] <= 'z')
        return word[0] - 'a';

    if (word[0] >= '0' && word[0] <= '9')
        return 26;

    return 27;
}

// Insert file at first position
int insert_at_first(filenames_t **head, char *filename)
{
    // Allocate memory for new node
    filenames_t *new = malloc(sizeof(filenames_t));

    if(new == NULL)
    {
        return FAILURE;
    }

    // Copy file name into new node
    strcpy(new->filename, filename);

    // Link new node with old first node
    new->link = *head;

    // Make new node as first node
    *head = new;

    return SUCCESS;
}

// Check whether file is already present
int check_duplicate(filenames_t *head, char *filename)
{
    // Traverse the file list
    while(head != NULL)
    {
        // Compare current file name with given file name
        if(strcmp(head->filename, filename) == 0)
        {
            return SUCCESS;
        }

        head = head->link;
    }

    return FAILURE;
}


// Check whether file has .txt extension
int check_file_extension(char *filename)
{
    char *ext;

    // Find the last dot in file name
    ext = strrchr(filename, '.');

    // Check whether extension is .txt
    if(ext != NULL && strcmp(ext, ".txt") == 0)
    {
        return SUCCESS;
    }

    return FAILURE;
}


// Insert file at last position
int insert_at_last(filenames_t **head, char *filename)
{
    filenames_t *new;
    filenames_t *temp;

    // Allocate memory for new node
    new = malloc(sizeof(filenames_t));

    if(new == NULL)
    {
        return FAILURE;
    }

    // Copy file name into new node
    strcpy(new->filename, filename);

    // New node will be the last node
    new->link = NULL;

    // If list is empty
    if(*head == NULL)
    {
        *head = new;
        return SUCCESS;
    }

    // Start from first node
    temp = *head;

    // Go to last node
    while(temp->link != NULL)
    {
        temp = temp->link;
    }

    // Connect new node at last
    temp->link = new;

    return SUCCESS;
}

// Initialize hash table
void create_hashtable(mainnode_t *hash_table[])
{
    int i;

    // Initialize all hash table indexes to NULL
    for(i = 0; i < 28; i++)
    {
        hash_table[i] = NULL;
    }
}

int create_database(filenames_t *head, mainnode_t *hash_table[])
{
    FILE *fptr;
    char buff[WORD_SIZE];
    int index;

    // Traverse all files
    while(head != NULL)
    {
        // Open current file
        fptr = fopen(head->filename, "r");

        if(fptr == NULL)
        {
            printf("%s : File not opened\n", head->filename);
            head = head->link;
            continue;
        }

        // Read words from file
        while(fscanf(fptr, "%19s", buff) == 1)
        {
            // Find hash index
            if(buff[0] >= 'A' && buff[0] <= 'Z')
            {
                index = buff[0] - 'A';
            }
            else if(buff[0] >= 'a' && buff[0] <= 'z')
            {
                index = buff[0] - 'a';
            }
            else if(buff[0] >= '0' && buff[0] <= '9')
            {
                index = 26;
            }
            else
            {
                index = 27;
            }

            // Search for word in main node
            mainnode_t *main_temp = hash_table[index];

            while(main_temp != NULL)
            {
                if(strcmp(main_temp->word, buff) == 0)
                {
                    break;
                }

                main_temp = main_temp->link;
            }

            // Word is not present, create new main node
            if(main_temp == NULL)
            {
                main_temp = malloc(sizeof(mainnode_t));

                if(main_temp == NULL)
                {
                    fclose(fptr);
                    return FAILURE;
                }

                strcpy(main_temp->word, buff);
                main_temp->file_count = 1;
                main_temp->sub_link = NULL;

                // Insert main node at first
                main_temp->link = hash_table[index];
                hash_table[index] = main_temp;

                // Create first subnode
                subnode_t *sub_new = malloc(sizeof(subnode_t));

                if(sub_new == NULL)
                {
                    fclose(fptr);
                    return FAILURE;
                }

                sub_new->word_count = 1;
                strcpy(sub_new->f_name, head->filename);

                // Insert subnode at first
                sub_new->link = main_temp->sub_link;
                main_temp->sub_link = sub_new;
            }
            else
            {
                // Search word in subnode list
                subnode_t *sub_temp = main_temp->sub_link;

                while(sub_temp != NULL)
                {
                    if(strcmp(sub_temp->f_name, head->filename) == 0)
                    {
                        break;
                    }

                    sub_temp = sub_temp->link;
                }

                // Word already exists in same file
                if(sub_temp != NULL)
                {
                    sub_temp->word_count++;
                }
                else
                {
                    // Word found but this is a new file
                    subnode_t *sub_new = malloc(sizeof(subnode_t));

                    if(sub_new == NULL)
                    {
                        fclose(fptr);
                        return FAILURE;
                    }

                    sub_new->word_count = 1;
                    strcpy(sub_new->f_name, head->filename);

                    // Insert subnode at first
                    sub_new->link = main_temp->sub_link;
                    main_temp->sub_link = sub_new;

                    // Increase file count
                    main_temp->file_count++;
                }
            }
        }

        // Close current file
        fclose(fptr);

        // Move to next file
        head = head->link;
    }

    return SUCCESS;
}

int display_database(mainnode_t *hash_table[])
{
    int i;
    mainnode_t *main_temp;
    subnode_t *sub_temp;

    // Traverse all 28 hash indexes
    for(i = 0; i < 28; i++)
    {
        main_temp = hash_table[i];

        // Traverse main nodes
        while(main_temp != NULL)
        {
            printf("[%d] [%s] : %d file(s)",i,main_temp->word,main_temp->file_count);

            // Get first subnode
            sub_temp = main_temp->sub_link;

            // Traverse subnodes
            while(sub_temp != NULL)
            {
                printf(" -> %s : %d",sub_temp->f_name,sub_temp->word_count);
                sub_temp = sub_temp->link;
            }

            printf("\n");

            // Move to next main node
            main_temp = main_temp->link;
        }
    }

    return SUCCESS;
}

int search_database(mainnode_t *hash_table[], char *word)
{
    int index;
    mainnode_t *main_temp;
    subnode_t *sub_temp;

    index = get_index(word);

    // Search word in main node
    main_temp = hash_table[index];

    while(main_temp != NULL)
    {
        if(strcmp(main_temp->word, word) == 0)
        {
            break;
        }

        main_temp = main_temp->link;
    }

    // Word not found
    if(main_temp == NULL)
    {
        printf("Word not found in database\n");
        return DATA_NOT_FOUND;
    }

    // Display word information
    printf("\nWord: %s\n", main_temp->word);
    printf("File count: %d\n", main_temp->file_count);

    // Traverse subnodes
    sub_temp = main_temp->sub_link;

    while(sub_temp != NULL)
    {
        printf("%s : %d\n", sub_temp->f_name, sub_temp->word_count);
        sub_temp = sub_temp->link;
    }

    return SUCCESS;
}

int save_database(mainnode_t *hash_table[], char *filename)
{
    FILE *fptr;
    mainnode_t *main_temp;
    subnode_t *sub_temp;
    int i;

    // Open file for writing
    fptr = fopen(filename, "w");

    if(fptr == NULL)
    {
        printf("File not opened\n");
        return FAILURE;
    }

    // Traverse all hash table indexes
    for(i = 0; i < 28; i++)
    {
        main_temp = hash_table[i];

        // Traverse main nodes
        while(main_temp != NULL)
        {
            fprintf(fptr, "#%d;%s;%d;",i,main_temp->word,main_temp->file_count);

            // Traverse subnodes
            sub_temp = main_temp->sub_link;

            while(sub_temp != NULL)
            {
                fprintf(fptr, "%s;%d;",sub_temp->f_name,sub_temp->word_count);
                sub_temp = sub_temp->link;
            }

            fprintf(fptr, "#\n");

            main_temp = main_temp->link;
        }
    }

    // Close file
    fclose(fptr);

    printf("Database saved successfully\n");

    return SUCCESS;
}

int update_database(filenames_t **head,mainnode_t *hash_table[],char *filename)
{
    FILE *fptr;
    char buff[WORD_SIZE];
    int index;

    mainnode_t *main_temp;
    mainnode_t *new_main;
    subnode_t *sub_temp;
    subnode_t *new_sub;

    // Check file extension
    if (check_file_extension(filename) == FAILURE)
    {
        printf("Invalid file extension\n");
        return FAILURE;
    }

    // Check duplicate file
    if (check_duplicate(*head, filename) == SUCCESS)
    {
        printf("File already exists\n");
        return FAILURE;
    }

    // Open new file
    fptr = fopen(filename, "r");

    if (fptr == NULL)
    {
        printf("File not found or cannot be opened\n");
        return FAILURE;
    }

    // Read words from the new file
    while (fscanf(fptr, "%19s", buff) == 1)
    {
        index = get_index(buff);
        main_temp = hash_table[index];

        // Search for the word
        while (main_temp != NULL)
        {
            if (strcmp(main_temp->word, buff) == 0)
                break;

            main_temp = main_temp->link;
        }

        if (main_temp != NULL)
        {
            sub_temp = main_temp->sub_link;

            // Search for the file in subnodes
            while (sub_temp != NULL)
            {
                if (strcmp(sub_temp->f_name, filename) == 0)
                    break;

                sub_temp = sub_temp->link;
            }

            if (sub_temp != NULL)
            {
                sub_temp->word_count++;
            }
            else
            {
                // Add a new subnode
                new_sub = malloc(sizeof(subnode_t));

                if (new_sub == NULL)
                {
                    fclose(fptr);
                    return FAILURE;
                }

                strcpy(new_sub->f_name, filename);
                new_sub->word_count = 1;

                new_sub->link = main_temp->sub_link;
                main_temp->sub_link = new_sub;

                main_temp->file_count++;
            }
        }
        else
        {
            // Create a new main node
            new_main = malloc(sizeof(mainnode_t));

            if (new_main == NULL)
            {
                fclose(fptr);
                return FAILURE;
            }

            strcpy(new_main->word, buff);
            new_main->file_count = 1;
            new_main->link = hash_table[index];
            new_main->sub_link = NULL;

            // Create first subnode
            new_sub = malloc(sizeof(subnode_t));

            if (new_sub == NULL)
            {
                free(new_main);
                fclose(fptr);
                return FAILURE;
            }

            strcpy(new_sub->f_name, filename);
            new_sub->word_count = 1;
            new_sub->link = NULL;

            new_main->sub_link = new_sub;
            hash_table[index] = new_main;
        }
    }

    fclose(fptr);

    // Add filename to the file list
    if (insert_at_last(head, filename) == FAILURE)
        return FAILURE;

    return SUCCESS;
}
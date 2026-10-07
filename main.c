#include "inverted_search.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    filenames_t *head = NULL;
    mainnode_t *hash_table[28];

    int i;

    // Check whether files are passed through command line
    if(argc < 2)
    {
        printf("Error: Pass file names through command line\n");
        return FAILURE;
    }

    // Read all command line file names
    for(i = 1; i < argc; i++)
    {
        // Check whether file has .txt extension
        if(check_file_extension(argv[i]) == FAILURE)
        {
            printf("%s : Invalid file extension\n", argv[i]);
            continue;
        }

        // Check whether file is already present
        if(check_duplicate(head, argv[i]) == SUCCESS)
        {
            printf("%s : Duplicate file\n", argv[i]);
            continue;
        }

        // Insert valid file at last
        if(insert_at_last(&head, argv[i]) == FAILURE)
        {
            printf("File insertion failed\n");
            return FAILURE;
        }
    }

    // Print the file list
    filenames_t *temp = head;

    printf("\nFile List:\n");

    while(temp != NULL)
    {
        printf("%s -> ", temp->filename);
        temp = temp->link;
    }

    printf("NULL\n");

    // Create and initialize hash table
    create_hashtable(hash_table);

    // Display menu
    int choice;
    while(1)
    {
        printf("\n1. Create Database\n");
        printf("2. Display Database\n");
        printf("3. Search Database\n");
        printf("4. Save Database\n");
        printf("5. Update Database\n");
        printf("6. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);
    
        switch(choice)
        {
            case 1:
                // Create database from input files
                printf("\nCreating Database...\n");

                if(create_database(head, hash_table) == SUCCESS)
                {
                    printf("Database created successfully\n");
                }
                else
                {
                    printf("Database creation failed\n");
                }
            break;
            
            case 2:
                // Display database
                printf("\nDatabase:\n");

                if(display_database(hash_table) == SUCCESS)
                {
                    printf("\n");
                }
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                return 0;
            default:
                printf("Choose a valid option..!!\n");
                break;
        }
    }
}
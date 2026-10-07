#ifndef MAIN_H
#define MAIN_H

#define SUCCESS 0
#define FAILURE -1
#define DATA_NOT_FOUND -2

#define FNAME_SIZE 30
#define WORD_SIZE 20

typedef struct sub
{
    int word_count;
    char f_name[FNAME_SIZE];
    struct sub *link;
} subnode_t;

typedef struct file
{
    char filename[FNAME_SIZE];
    struct file *link;
} filenames_t;

typedef struct mainnode
{
    int file_count;
    char word[WORD_SIZE];
    struct mainnode *link;
    struct sub *sub_link;
} mainnode_t;

#endif

int check_duplicate(filenames_t *head, char *filename);

int insert_at_first(filenames_t **head, char *filename);

int check_file_extension(char *filename);

int insert_at_last(filenames_t **head, char *filename);

void create_hashtable(mainnode_t *hash_table[]);

int create_database(filenames_t *head, mainnode_t *hash_table[]);

int display_database(mainnode_t *hash_table[]);

int search_database(mainnode_t *hash_table[], char *word);

int save_database(mainnode_t *hash_table[], char *filename);

int update_database(mainnode_t *hash_table[], char *filename);
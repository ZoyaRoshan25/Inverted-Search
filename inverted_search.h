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
}subnode_t;

typedef struct file
{
    char filename[WORD_SIZE];
    struct file *link;
}filenames_t;

typedef struct mainnode
{
    int file_count;
    char word[WORD_SIZE];
    struct mainnode *link;
    struct sub *sub_link;
}mainnode_t;

#endif
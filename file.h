#ifndef __FILE_H__
#define __FILE_H__

typedef struct Directory {
char *name;
struct list_dirs *list_d;
struct list_files *list_f;
struct Directory *parentDir;
} Directory;
typedef struct list_dirs {
Directory *dir;
struct list_dirs *next;
} list_dirs;


typedef struct File {
char *name;
int size;
char *data;
struct Directory *dir;
} File;
typedef struct list_files {
File *file;
struct list_files *next;
} list_files;
void tree(Directory *director_curent, int k, int k2);
void afisare_f_tree(list_files *lista, int k);
void afisare_d_tree(list_dirs *lista, int k, int k2);

#endif /* __FILE_H__ */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct file
{
    int size;
    char *name;
};

struct dir
{
    int pos;
    int dir_size;
    int file_size;
    char *name;
    struct file *files;
    struct dir *dirs;
    int bytes;
};


void init_dir(struct dir *dir, char *name, int position)
{
    dir->pos = position;
    dir->bytes = 0;
    dir->dir_size = 0;
    dir->file_size = 0;
    dir->name = name;
    dir->dirs = malloc(100 * sizeof(struct dir));
    dir->files = malloc(100 * sizeof(struct file));
}

void init_file(struct file *file, char *name, int size)
{
    file->size = size;
    file->name = name;
}

char line[30];
char dir_name[25];
char file_name[25];

int main()
{
    FILE *input;
    input = fopen("day7.txt", "r");

    struct dir *tree = malloc(100 * sizeof(struct dir));
    int pos = 0;

    fgets(line, 30, input);
    init_dir(&tree[pos], "/", pos);

    while (fgets(line, 30, input) != NULL)
    {
        //TODO not sure, jestli musim tuhle kontrolu delat a nekam ukladat aktualni slozku
        //kontrola, kde vlastne jsme (aktualni slozka)
        /*tree[pos][i];*/

        if (line[0] == '$')
        {
            //je to prikaz
            if (line[2] == 'c' && line[3] == 'd')
            {
                //je to cd
                if (line[5] == '.' && line[6] == '.')
                {
                    //je to cd ..
                    //TODO ulozit pocet bytu do diru, abych o to neprisel
                    pos--;
                    continue;
                }
                else
                {
                    //je to cd DIR
                    for (int i = 5; i < strlen(line); i++)
                    {
                        dir_name[i - 5] = line[i];
                        dir_name[i - 5 + 1] = '\0';
                    }
                    
                    int cnt = 0;
                    for (int i = 0; i < tree[pos].dir_size; i++)
                    {
                        if (!strcmp(dir_name, tree[pos].dirs[i].name))
                        {
                            //mame ten dir, ted do nej prejit
                            pos++;
                            init_dir(&tree[pos], dir_name, pos);
                            break;
                        }
                    }
                    continue;
                }
            }
        }
        else
        {
            //je to vypis
            if (line[0] == 'd' && line[1] == 'i' && line[2] == 'r')
            {
                //je to dir
                for (int i = 4; i < strlen(line); i++)
                {
                    dir_name[i - 4] = line[i];
                    dir_name[i - 4 + 1] = '\0';
                }
                init_dir(&tree[pos].dirs[tree[pos].dir_size], dir_name, pos + 1);
                tree[pos].dir_size++;
            }
            else
            {
                //je to file
                struct file tmp_file;
                for (int i = 0; i < strlen(line); i++)
                {
                    file_name[i] = line[i];
                    file_name[i + 1] = '\0';
                }

                int size = atoi(line);
                init_file(&tmp_file, file_name, size);

                tree[pos].files[tree[pos].file_size] = tmp_file;
                tree[pos].file_size++;
            }
        }
    }
    //TODO nejaky konecny vypis a podminka
    fclose(input);
    return 0;
}
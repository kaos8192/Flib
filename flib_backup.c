#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <dirent.h> 
#include "flib.h"

/* Code by Geir Anderson
 * Recieved help from Alex Sedgwick
 * Uses dcurses, unistd, stat, and string
 *
 *
 * TODO:
 * ==================================================================
 *      1. lists
 *
 *      2. identity tags
 *
 *      3. size
 *
 *      4. date
 *
 *      5. sorts
 *
 *      6. misc.
 *
 * ==================================================================
 */

//return a pointer to an array of c strings, the last element should be zero
//this will list the name of each file in the path
char** fl_list(const char* path)
{
    char ** return_arr = fl_list_filter(path, "", "");
    return return_arr;
}

//same as fl_list except the files returned must start with prefix
//and end with suffix
char** fl_list_filter(const char* path, const char* prefix, const char* suffix)
{
    DIR* direct_ptr = opendir(path);
    struct dirent *cur_fptr;

    int i = 0;

    while(cur_fptr = readdir(direct_ptr))
    {
        if()
        {
            i++;
        }
        else
        {
        }
    }

    char** result_arr;
    result_arr = (char **)malloc(i * 256);
    rewinddir(direct_ptr);

    i = 0;

    while(cur_fptr = readdir(direct_ptr))
    {
        int lurng = strlen(cur_fptr->d_name);
        char* s = malloc(lurng * sizeof(char));
        strncpy(s, cur_fptr->d_name, lurng);
        s[lurng] = 0;
        result_arr[i] = malloc(lurng * sizeof(char));
        strncpy(result_arr[i], s, lurng);
        i++;
    }

    char *n = "\0";
    result_arr[i] = (char*)malloc(strlen(n));
    result_arr[i] = n;

    closedir(direct_ptr);
    return result_arr;
}

//count the number of file names in files
int fl_count(char** files)
{

}

//is the file a directory
int fl_isDir(const char* name)
{

}

//is the file an executable
int fl_isExe(const char* name)
{

}

//is the file a symbolic link
int fl_isSym(const char* name)
{

}

//what is the file's size in bytes
unsigned int fl_size(const char* name)
{

}

//same as fl_size except if the file is a directory then it recursively finds
//the size of directory and all files it contains
unsigned int fl_size_r(const char* name)
{

}

//the last modification time of the file
unsigned int fl_date(const char* name)
{

}

//sort the files descending by fl_size
void fl_sort_bySize(char** files)
{

}

//sort the files descending by fl_date
void fl_sort_byDate(char** files)
{

}

//free all of the memory associated with files
void fl_delete(char** files)
{

}

//format the date and store in str
//Mar 09 13:49
void fl_formatDate(const char* filename, char* str)
{

}

//XXX.XB where the last can be B, K, M, G 
void fl_readableSize(unsigned int size,char* str)
{

}

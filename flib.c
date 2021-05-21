#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <dirent.h> 
#include "flib.h"

/* Code by Geir Anderson
 * Recieved help from Alex Sedgwick
 * Uses unistd, stat, and string
 * size for buffer in strings idea from (found in format date and readable size): 
 * https://www.geeksforgeeks.org/how-to-find-size-of-array-in-cc-without-using-sizeof-operator/
 *
 * TODO:
 * ==================================================================
 *      1. lists
 *          [✓] fl_list
 *          [✓] fl_list_filter
 *          [✓] fl_count
 *
 *      2. identity tags
 *          [✓] fl_isDir
 *          [✓] fl_isExe
 *          [✓] fl_isSym
 *
 *      3. size
 *          [✓] fl_size
 *          [✓] fl_size_r
 *
 *      4. date
 *          [✓] fl_date
 *          [✓] fl_formatDate
 *
 *      5. sorts
 *          [✓] fl_sort_bySize
 *          [✓] fl_sort_byDate
 *
 *      6. misc.
 *          [✓] fl_delete
 *          [✓] fl_readableSize
 *
 * ==================================================================
 */

//return a pointer to an array of c strings, the last element should be zero
//this will list the name of each file in the path
char** fl_list(const char* path)
{
    return fl_list_filter(path, "", "");
}

//same as fl_list except the files returned must start with prefix
//and end with suffix
char** fl_list_filter(const char* path, const char* prefix, const char* suffix)
{
    DIR* direct_ptr = opendir(path);
    struct dirent *cur_fptr;

    int i = 0;
    int prefl = strlen(prefix);
    int suffl = strlen(suffix);
    int prefbool = 0;
    int suffbool = 0;

    while(cur_fptr = readdir(direct_ptr))
    {
        int comparnum = strlen(cur_fptr->d_name);

        if(comparnum >= prefl)
        {
            prefbool = strncmp(prefix, cur_fptr->d_name, prefl);
        }

        else
        {
            prefbool = -1;
        }

        if(comparnum >= suffl)
        {
            char *last_ones = &(cur_fptr->d_name)[comparnum - suffl];

            suffbool = strncmp(suffix, last_ones, suffl);
        }

        else
        {
            suffbool = -1;
        }

        if(prefbool >= 0 && suffbool >= 0)
        {
            i++;
        }
    }

    char** result_arr;
    result_arr = malloc(sizeof(char*)*(i+1));

    rewinddir(direct_ptr);

    i = 0;

    while(cur_fptr = readdir(direct_ptr))
    {
        int lurng = strlen(cur_fptr->d_name);
        char *last_part = &(cur_fptr->d_name)[lurng - suffl];

        if((prefbool = strncmp(prefix, cur_fptr->d_name, prefl)) == 0
                && 
                (suffbool = strncmp(suffix, last_part, suffl)) == 0)
        {
            char* s = cur_fptr->d_name;
            strcat(s,"\0");
            result_arr[i] = malloc(lurng+1);
            strcpy(result_arr[i], s);
            i++;
        }
    }

    result_arr[i] = 0;

    closedir(direct_ptr);

    return result_arr;
}

//count the number of file names in files
int fl_count(char** files)
{
    int result = 0;

    while(files[result] != 0)
    {
        result++;
    }

    return result;
}

//is the file a directory
int fl_isDir(const char* name)
{
    struct stat file_here;

    lstat(name, &file_here);

    return S_ISDIR(file_here.st_mode);
}

//is the file an executable
int fl_isExe(const char* name)
{
    struct stat file_here;

    lstat(name, &file_here);

    if(S_ISREG(file_here.st_mode) 
            && file_here.st_mode & 0111)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//is the file a symbolic link
int fl_isSym(const char* name)
{
    struct stat file_here;

    lstat(name, &file_here);

    return S_ISLNK(file_here.st_mode);
}

//what is the file's size in bytes
unsigned int fl_size(const char* name)
{
    struct stat file_here;

    lstat(name, &file_here);

    return (unsigned int)file_here.st_size;
}

//same as fl_size except if the file is a directory then it recursively finds
//the size of directory and all files it contains
unsigned int fl_size_r(const char* name)
{
    unsigned int total = 0;
    char str[4096];

    if(strcmp(name, ".") == 0
            || strcmp(name, "..") == 0
            || fl_isDir(name) == 0)
    {
        total = fl_size(name);
    }

    else
    {
        char** files = fl_list(name);

        for(int i = 0; i < fl_count(files); i++)
        {
            if(strcmp(files[i], ".") == 0
                    || strcmp(files[i], "..") == 0)
            {
                continue;
            }
            else
            {
                sprintf(str, "%s/%s", name, files[i]);
                if(fl_isDir(str) == 0)
                {
                    total += fl_size(str);
                }
                else
                {
                    total += fl_size_r(str);
                }
            }
        }
    }
    return total;
}

//the last modification time of the file
unsigned int fl_date(const char* name)
{
    struct stat file_stats;

    lstat(name, &file_stats);

    return (unsigned int)file_stats.st_mtime;
}

//Helper for sort size
int fl_sort_bySize_help(const void* param1, const void* param2)
{
    return fl_size(*(char**)param1)
        - fl_size(*(char**)param2);
    //return fl_size_r(*(char**)param1) - fl_size_r(*(char**)param2);
}
//sort the files descending by fl_size
void fl_sort_bySize(char** files)
{
    qsort(files, fl_count(files),
            sizeof(files[0]),
            fl_sort_bySize_help);
}

//Helper for sort date
int fl_sort_byDate_help(const void* param1, const void* param2)
{
    return fl_date(*(char**)param1)
        - fl_date(*(char**)param2);
}

//sort the files descending by fl_date
void fl_sort_byDate(char** files)
{
    qsort(files, fl_count(files), 
            sizeof(files[0]),
            fl_sort_byDate_help);
}

//free all of the memory associated with files
void fl_delete(char** files)
{
    int i = 0;

    while(files[i] != 0)
    {
        free(files[i]);
        i++;
    }

    free(files);
}

//format the date and store in str
//Mar 09 13:49
void fl_formatDate(const char* filename, char* str)
{
    struct stat file_stats;

    lstat(filename, &file_stats);

    strftime(str, (*(&str + 1) - str),
            "%b %d %R",
            localtime(&file_stats.st_mtime));
}

//XXX.XB where the last can be B, K, M, G 
void fl_readableSize(unsigned int size,char* str)
{
    const char *pref[] = {"B", "K", "M", "G"};
    int i = 0;
    float scale = size;

    while(scale >= 1000)
    {
        scale = scale/1000;
        i++;
    }

    snprintf(str, (*(&str + 1) - str),
            "%.1f%s", scale, pref[i]); 
}

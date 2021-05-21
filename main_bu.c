#include "flib.h"
#include "dcurses.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    char* a = "\0";
    char* b = "\0";
    char* c = "\0";
    int list_mode = 0;
    int sort_mode = 0;
    int count_mode = 0;
    char ** arrayit;
    int i = 0;
    unsigned int ui = 0;
    unsigned int date_t = 0;
    char date_str[13];
    char space_str[7];
    int setting_switcher = 0;
    while((setting_switcher = getopt(argc, argv, "f:p:s:mdr")) != -1)
    {
        switch(setting_switcher)
        {
            case'f':
                a = optarg;
                list_mode = 0;
                break;
            case'p':
                b = optarg;
                list_mode = 1;
                break;
            case's':
                c = optarg;
                list_mode = 1;
                break;
            case'm':
                sort_mode = 1;
                break;
            case'd':
                sort_mode = 2;
                break;
            case'r':
                count_mode = 1;
                break;
            default:
                list_mode = 0;
                a = ".";
                b = "";
                c = "";
                break;
        }
    }

    if(a == "\0")
    {
        a = ".";
    }
    if(b == "\0")
    {
        b = "";
    }
    if(c == "\0")
    {
        c = "";
    }

    if(list_mode == 0)
    {
        arrayit = fl_list(a);
    }
    else if(list_mode == 1)
    {
        arrayit = fl_list_filter(a, b, c);
    }
    else
    {
        return -1;
    }

    if(sort_mode == 1)
    {
        fl_sort_bySize(arrayit);
    }
    else if(sort_mode == 2)
    {
        fl_sort_byDate(arrayit);
    }
    else
    {
    }
    printf("\n");
    while(arrayit[i] != NULL)
    {
        if(count_mode == 1)
        {
            ui = fl_size_r(arrayit[i]);
        }
        else
        {
            ui = fl_size(arrayit[i]);
        }
        fl_readableSize(ui, space_str);
        fl_formatDate(arrayit[i], date_str);

        dcurses_fg(DCURSES_COLOR_GREEN);
        printf("%7s ", space_str);
        dcurses_fg(DCURSES_COLOR_WHITE|DCURSES_COLOR_BRIGHT);
        printf("%s", date_str);
        if(fl_isDir(arrayit[i]) == 1)
        {
            dcurses_fg(DCURSES_COLOR_BLUE);
            printf("    [dir]   ");
            dcurses_fg(DCURSES_COLOR_BLUE);
            printf("%s", arrayit[i]);
        }
        else if(fl_isExe(arrayit[i]) == 1)
        {
            dcurses_fg(DCURSES_COLOR_GREEN | DCURSES_COLOR_GREEN);
            printf("    [exe]   ");
            dcurses_fg(DCURSES_COLOR_GREEN | DCURSES_COLOR_BRIGHT);
            printf("%s", arrayit[i]);
        }
        else if(fl_isSym(arrayit[i]) == 1)
        {
            dcurses_fg(DCURSES_COLOR_RED | DCURSES_COLOR_BRIGHT);
            printf("    [sym]   ");
            dcurses_fg(DCURSES_COLOR_RED | DCURSES_COLOR_BRIGHT);
            printf("%s", arrayit[i]);
        }
        else
        {
            printf("            ");
            printf("%s", arrayit[i]);
        }
        printf("\n");
        dcurses_reset();
        i++;
    }
    printf("\t\n");
    dcurses_reset();
    printf("_____________________________________________________\n\n");
    int entries = fl_count(arrayit);
    printf(" %d Entries\n\n", entries);

    fl_delete(arrayit);

    return 0;
}

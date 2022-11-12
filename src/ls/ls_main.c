/*
 * File: ls_main.c
 *
 *
 * NAME
 *            ls - list directory contents
 *
 * SYNOPSIS
 *            ls [OPTION]... [FILE]...
 *
 *
 * DESCRIPTION
 *            List  information  about  the FILEs (the current directory by default).
 *
 * OPTIONS
 *           -a   do not ignore entries starting with .
 *           -i   print the index number of each file
 *	         -l   use a long listing format
 *
 *
 * Created: 2020-02-21
 * Author:  Mohamed_Moawad
 *
 */

/************************************************************************/
/*				                INCLUDES            			        */
/************************************************************************/

#include "ls.h"

/************************************************************************/
/*				        Global / Static Variables                       */
/************************************************************************/

int g_Printing_Option = NO_OPTION;
int g_Sorting_Option = NO_OPTION;

/************************************************************************/
/*				                MAIN FUNCTION	    	                */
/************************************************************************/

int main(int argc, char **argv)
{

    int Number_of_options = 0;
    int Number_of_Separated_options = 0;
    char options[MAX_OPTIONS], path[MAX_PATH];
    struct stat file_status;

    /*********************** Options_Parsing **************************/
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            for (int j = 1; j < strlen(argv[i]); j++)
            {
                options[Number_of_options] = argv[i][j];
                Number_of_options++;
            }
            Number_of_Separated_options++;
        }
    }
    options[Number_of_options] = '\0';

    for (int options_index = 0; options_index < Number_of_options; options_index++)
    {
        switch (options[options_index])
        {
        case 'a':
            g_Printing_Option |= OPTION_a;
            break;
        case 'i':
            g_Printing_Option |= OPTION_i;
            break;
        case 'l':
            g_Printing_Option |= OPTION_l;
            break;
        default:
            printf("my_ls:invalid option -%c\n", options[options_index]);
            exit(0);
            break;
        }
    }

    /**************************************************************/

    /*********************** Path Parsing**************************/

    if (Number_of_Separated_options + 1 == argc)
    {
        strcpy(path, ".");
        Display_Dir_Content(path);
        printf("\n");
        return 0;
    }
    else
    {
        // Number_of_paths = argc - Number_of_Separated_options - 1;
        for (int path_parsing_index = 1; path_parsing_index < argc; path_parsing_index++)
        {

            if (argv[path_parsing_index][0] == '-')
            {
                continue;
            }
            else
            {
                strcpy(path, argv[path_parsing_index]);

                if (stat(path, &file_status) == -1)
                {
                    Report_Error("stat");
                }
                else if (S_ISDIR(file_status.st_mode))
                {

                    Display_Dir_Content(path);
                }
                else
                {
                    Display_File(path);
                }
            }
        }
    }

    /**************************************************************/

    printf("\n");

    return 0;
}

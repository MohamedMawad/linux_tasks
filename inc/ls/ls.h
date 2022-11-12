/*
 * File: ls.h
 *
 *
 * NAME
 *         ls - list directory contents
 *
 *SYNOPSIS
 *         ls [OPTION]... [FILE]...
 *
 *
 * Created: 2020-02-21
 * Author:  Mohamed_Moawad
 *
 */

#ifndef _LS_H
#define _LS_H

/************************************************************************/
/*				                    INCLUDES	        		        */
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <errno.h>
#include <sys/ioctl.h>

/************************************************************************/
/*				                   DEFINES	            		        */
/************************************************************************/

#define MAX_NAME 256
#define MAX_OPTIONS 10
#define MAX_PATH 4096
// #define TERMINAL_ROWLEN 80
#define MAXLEN_INODE 8

#define NO_OPTION 0
/********** Printing_based options *************/
#define OPTION_a 1 /* do not ignore entries starting with . */
#define OPTION_i 2 /* print the index number of each file */
#define OPTION_l 4 /* use a long listing format */

/********** Sorting_based options *************/
#define OPTION_S 1 /* Sorting by size */
#define OPTION_n 2 /* Sorting by name */
#define OPTION_r 4 /* Reverse while Sorting */

/************************************************************************/
/*		     Shared global variables	       		        */
/************************************************************************/

extern int g_Printing_Option;
extern int g_Sorting_Option;

/************************************************************************/
/*		                  ls FUNCTIONS' PROTOTYPES	        	        */
/************************************************************************/

void Display_Dir_Content(char *path);
int Insert_Filenames_in_List(char *path, char ***List_of_Filenames);
void Display_File(char *Filename);

void Print_Colored_File(struct stat *file_info, char *Filename);
void Print_Inode_Number(struct stat *file_info);
void Print_Permitions(struct stat *file_info);
void Print_User_Name(struct stat *file_info);
void Print_Group_Name(struct stat *file_info);
void Print_User_ID(struct stat *file_info);
void Print_Group_ID(struct stat *file_info);
void Print_File_Size(struct stat *file_info);
void Print_File_Type(struct stat *file_info);
void Print_Last_Modification_Time(struct stat *file_info);
void Print_Last_Access_Time(struct stat *file_info);
void Print_Number_Of_Links(struct stat *file_info);
void Report_Error(const char *err_string);

/********* Colors *********/

void Red(void);
void Green(void);
void Yellow(void);
void Blue(void);
void Cyan(void);
void reset_color(void);

/**************************/

#endif /* ls.h  */

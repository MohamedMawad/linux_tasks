/*
 * File: ls.c
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

/************************************************************************/
/*				 					INCLUDES					        */
/************************************************************************/

#include "ls.h"

/************************************************************************/
/*				 		Global / Static Variables       		        */
/************************************************************************/

static int g_maxlen_of_file_name;
static int cursor_position = 0;

/************************************************************************/
/*		         		ls FUNCTIONS' implementation		         	*/
/************************************************************************/

void Display_Dir_Content(char *path)
{
	char **List_of_Filenames = NULL;
	char **h = (char **)malloc(40);
	int Number_of_Files = 0;

	/*start of Insert_Filenames_in_List fuction section*/

	DIR *ptr_to_dir;
	struct dirent *ptr_to_directory_entry;

	/*** Count number of files in the passed path ***/

	ptr_to_dir = opendir(path);
	if (NULL == ptr_to_dir)
	{
		Report_Error("opendir");
	}
	else
	{
		g_maxlen_of_file_name = 0;
		while ((ptr_to_directory_entry = readdir(ptr_to_dir)) != NULL)
		{
			if (g_maxlen_of_file_name < strlen(ptr_to_directory_entry->d_name))
				g_maxlen_of_file_name = strlen(ptr_to_directory_entry->d_name);

			Number_of_Files++;
		}
		closedir(ptr_to_dir);
	}

	/*** Allocate space in Heap memory based on the number of files ***/

	List_of_Filenames = (char **)malloc(sizeof(char *) * (Number_of_Files));
	for (int i = 0; i < Number_of_Files; i++)
	{
		List_of_Filenames[i] = (char *)malloc(sizeof(char) * MAX_NAME); // MAX_NAME = 256
	}

	/*** Insert file names in this allocated area ***/
	ptr_to_dir = opendir(path);

	for (int i = 0; i < Number_of_Files; i++)
	{
		ptr_to_directory_entry = readdir(ptr_to_dir);
		if (NULL == ptr_to_directory_entry)
		{
			Report_Error("opendir");
		}
		else
		{
			strcpy(List_of_Filenames[i], ptr_to_directory_entry->d_name);
		}
	}

	closedir(ptr_to_dir);

	/*end of Insert_Filenames_in_List fuction section*/

	if (chdir(path) < 0)
	{
		Report_Error("chdir");
	}

	for (int list_index = 0; list_index < Number_of_Files; list_index++)
	{
		Display_File(List_of_Filenames[list_index]);
	}

	/*** Free the allocated space ***/

	for (int list_index = 0; list_index < Number_of_Files; list_index++)
	{
		free(List_of_Filenames[list_index]);
	}
	free(List_of_Filenames);
}


void Display_File(char *Filename)
{
	struct stat file_status;

	if (stat(Filename, &file_status) == -1)
	{
		Report_Error("stat");
	}
	else
	{

		switch (g_Printing_Option)
		{
		case NO_OPTION:
			if (Filename[0] != '.')
			{
				if (g_maxlen_of_file_name > TERMINAL_ROWLEN - cursor_position)
				{
					printf("\n");
					cursor_position = 0;
				}
				Print_Colored_File(&file_status, Filename);
				cursor_position += g_maxlen_of_file_name;
			}
			break;
		case OPTION_a:
			if (g_maxlen_of_file_name > TERMINAL_ROWLEN - cursor_position)
			{
				printf("\n");
				cursor_position = 0;
			}
			Print_Colored_File(&file_status, Filename);
			cursor_position += g_maxlen_of_file_name;
			break;
		case OPTION_i:
			if (Filename[0] != '.')
			{
				if (g_maxlen_of_file_name + MAXLEN_INODE > TERMINAL_ROWLEN - cursor_position)
				{
					printf("\n");
					cursor_position = 0;
				}
				Print_Inode_Number(&file_status);
				Print_Colored_File(&file_status, Filename);
			}
			cursor_position += g_maxlen_of_file_name + MAXLEN_INODE;
			break;
		case OPTION_a + OPTION_i:
			if (g_maxlen_of_file_name + MAXLEN_INODE > TERMINAL_ROWLEN - cursor_position)
			{
				printf("\n");
				cursor_position = 0;
			}
			Print_Inode_Number(&file_status);
			Print_Colored_File(&file_status, Filename);
			cursor_position += g_maxlen_of_file_name + MAXLEN_INODE;
			break;
		case OPTION_l:
			if (Filename[0] != '.')
			{
				Print_File_Type(&file_status);
				Print_Permitions(&file_status);
				Print_Number_Of_Links(&file_status);
				Print_User_Name(&file_status);
				Print_Group_Name(&file_status);
				Print_File_Size(&file_status);
				Print_Last_Modification_Time(&file_status);
				Print_Colored_File(&file_status, Filename);
				printf("\n");
			}
			break;
		case OPTION_l + OPTION_a:
			Print_File_Type(&file_status);
			Print_Permitions(&file_status);
			Print_Number_Of_Links(&file_status);
			Print_User_Name(&file_status);
			Print_Group_Name(&file_status);
			Print_File_Size(&file_status);
			Print_Last_Modification_Time(&file_status);
			Print_Colored_File(&file_status, Filename);
			printf("\n");
			break;
		case OPTION_l + OPTION_i:
			if (Filename[0] != '.')
			{
				Print_Inode_Number(&file_status);
				Print_File_Type(&file_status);
				Print_Permitions(&file_status);
				Print_Number_Of_Links(&file_status);
				Print_User_Name(&file_status);
				Print_Group_Name(&file_status);
				Print_File_Size(&file_status);
				Print_Last_Modification_Time(&file_status);
				Print_Colored_File(&file_status, Filename);
				printf("\n");
			}
			break;
		case OPTION_l + OPTION_i + OPTION_a:
			Print_Inode_Number(&file_status);
			Print_File_Type(&file_status);
			Print_Permitions(&file_status);
			Print_Number_Of_Links(&file_status);
			Print_User_Name(&file_status);
			Print_Group_Name(&file_status);
			Print_File_Size(&file_status);
			Print_Last_Modification_Time(&file_status);
			Print_Colored_File(&file_status, Filename);
			printf("\n");
			break;

		default:
			printf("Unknown option!!!\n");
			break;
		}

	} /* end of else block*/
}

void Print_Colored_File(struct stat *file_info, char *Filename)
{
	switch (file_info->st_mode & S_IFMT)
	{
	case S_IFREG:
	case S_IFBLK:
	case S_IFCHR:
		if (file_info->st_mode & S_IXUSR || file_info->st_mode & S_IXGRP || file_info->st_mode & S_IXOTH)
		{
			Green();
		}
		break;
	case S_IFDIR:
		Blue();
		break;
	case S_IFIFO:
		Yellow();
		break;
	case S_IFLNK:
		Cyan();
		break;
	case S_IFSOCK:
		Red();
		break;
	default:
		printf("Unknown!!!");
		break;
	}

	printf("%-*s  ", g_maxlen_of_file_name, Filename);
	reset_color();
}
/*************Colors*****************/
/*
CODE	COLOR
[0;31m	Red
[1;31m	Bold Red
[0;32m	Green
[1;32m	Bold Green
[0;33m	Yellow
[01;33m	Bold Yellow
[0;34m	Blue
[1;34m	Bold Blue
[0;35m	Magenta
[1;35m	Bold Magenta
[0;36m	Cyan
[1;36m	Bold Cyan
[0m	Reset
*/
void Red(void)
{
	printf("\033[1;31m");
}

void Green(void)
{
	printf("\033[1;32m");
}

void Yellow(void)
{
	printf("\033[1;33m");
}
void Blue(void)
{
	printf("\033[1;34m");
}
void Cyan(void)
{
	printf("\033[0;36m");
}

void reset_color(void)
{
	printf("\033[0m");
}

/******************************/

void Print_Permitions(struct stat *file_info)
{
	// Owner permissions
	if (file_info->st_mode & S_IRUSR)
		printf("r");
	else
		printf("-");
	if (file_info->st_mode & S_IWUSR)
		printf("w");
	else
		printf("-");
	if (file_info->st_mode & S_IXUSR)
		printf("x");
	else
		printf("-");

	// Group permissions
	if (file_info->st_mode & S_IRGRP)
		printf("r");
	else
		printf("-");
	if (file_info->st_mode & S_IWGRP)
		printf("w");
	else
		printf("-");
	if (file_info->st_mode & S_IXGRP)
		printf("x");
	else
		printf("-");

	// Others permissions
	if (file_info->st_mode & S_IROTH)
		printf("r");
	else
		printf("-");
	if (file_info->st_mode & S_IWOTH)
		printf("w");
	else
		printf("-");
	if (file_info->st_mode & S_IXOTH)
		printf("x");
	else
		printf("-");

	// printf(" ");
}

void Print_Inode_Number(struct stat *file_info)
{
	printf("%7ld ", (long)file_info->st_ino);
}

void Print_User_Name(struct stat *file_info)
{
	struct passwd *psd; // Receive the user name of the file owner from this structure

	psd = getpwuid(file_info->st_uid);

	printf("%#s ", psd->pw_name);
}

void Print_Group_Name(struct stat *file_info)
{
	struct group *grp; // Get group name

	grp = getgrgid(file_info->st_gid);

	printf("%#s ", grp->gr_name);
}

void Print_User_ID(struct stat *file_info)
{
	printf("%#ld ", (long)file_info->st_uid);
}

void Print_Group_ID(struct stat *file_info)
{
	printf("%#ld ", (long)file_info->st_gid);
}

void Print_File_Size(struct stat *file_info)
{

	printf("%6lld ", (long long)file_info->st_size);
}

void Print_File_Type(struct stat *file_info)
{
	switch (file_info->st_mode & S_IFMT)
	{
	case S_IFBLK:
		printf("b");
		break;
	case S_IFCHR:
		printf("c");
		break;
	case S_IFDIR:
		printf("d");
		break;
	case S_IFIFO:
		printf("f");
		break;
	case S_IFLNK:
		printf("l");
		break;
	case S_IFREG:
		printf("-");
		break;
	case S_IFSOCK:
		printf("s");
		break;
	default:
		printf("?");
		break;
	}
}

void Print_Last_Modification_Time(struct stat *file_info)
{
	char buff_time[32];
	strcpy(buff_time, ctime(&file_info->st_mtime));
	buff_time[strlen(buff_time) - 1] = '\0';
	printf("%#s ", buff_time);
}

void Print_Last_Access_Time(struct stat *file_info)
{
	char buff_time[32];

	strcpy(buff_time, ctime(&file_info->st_atime));
	buff_time[strlen(buff_time) - 1] = '\0';
	printf("%#s ", buff_time);
}

void Print_Number_Of_Links(struct stat *file_info)
{
	printf("%3ld ", (long)file_info->st_nlink);
}

void Report_Error(const char *err_string)
{
	perror(err_string);
	exit(1);
}

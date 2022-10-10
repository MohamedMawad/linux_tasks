#include <femtoShell.h>

// printf color codes
void red(void)
{
    printf("\033[1;31m");
}
void green(void)
{
    printf("\033[1;32m");
}
void yellow(void)
{
    printf("\033[1;33m");
}
void blue(void)
{
    printf("\033[1;34m");
}
void reset(void)
{
    printf("\033[0m");
}

void printPrompt(void)
{
    char username[NAME_MAX + 1];
    char hostname[_SC_HOST_NAME_MAX + 1];
    char fullPath[PATH_MAX + 1];

    getlogin_r(username, NAME_MAX + 1);
    gethostname(hostname, _SC_HOST_NAME_MAX + 1);
    getcwd(fullPath, PATH_MAX + 1);

    green();
    printf("%s@%s", username, hostname);
    reset();
    printf(":");
    blue();
    printf("%s", fullPath);
    reset();
    printf("# ");
}

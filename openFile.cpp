#include <sys/types.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include <stdint.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>
#include <ctype.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <iostream>
using namespace std;

int main()

{
    string filename,str= "xdg-open ";

    printf("Enter the filename to be opened \n");

    getline(cin,filename);

    str.append(filename);
    //str = filename;
    char file[str.length()];

    for(int i=0;i<str.length();i++)
        file[i] = str[i];
    //file[str.length()+1] = NULL;
    /*  open the file for reading */
    system(file);

    //if(!fork())
    //execl(file, NULL);

    return 0;

}
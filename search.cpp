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
#include <linux/kernel.h>
#include <linux/fs.h>
#include <bits/stdc++.h>

using namespace std;

char pathname[MAXPATHLEN];

//std::size_t found = str.find(str2);
  //if (found!=std::string::npos)
    //std::cout << "first 'needle' found at: " << found << '\n';



void search(vector<pair<string, int>> directory,string file){
  
	for(int i=0;i<directory.size();i++){
		//cout<<file<<" "<<directory[i].first<<"\n";	
		size_t found = directory[i].first.find(file);
		if(found!=std::string::npos){
			cout<<"\n"<<directory[i].first<<"\n";
		}
	}
}


/**
 * Lists all files and sub-directories recursively 
 * considering path as base path.
 */

void listFilesRecursively(char *basePath,vector<pair<string, int>> &directory)
{
    char path[PATH_MAX];
    string temp;
    int flag=1;
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    if(dir){
    	temp = string(basePath);
    	//directory.push_back(temp);
    	directory.push_back(pair<string, int> (temp,1));

    while ((dp = readdir(dir)) != NULL)
    {
    	string str;

    	if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            //strcpy(path, basePath);
            //strcat(path, "/");
            strcpy(path, dp->d_name);
            //printf("%s\n",path );
            str = string(basePath)+"/"+string(path);
    		directory.push_back(pair<string, int> (str,0));
    	}
    }
    	closedir(dir);
	}
}


int main(int argc, char *argv[])
{
    // Directory path to list files
    char path[PATH_MAX];
    vector<pair<string,int>> directory;

    getcwd(path, sizeof(pathname));

    //cout<<string(path)<<"\n";

    listFilesRecursively(path,directory);
    int i=1;
    while(i<directory.size()){

    	strcpy(path, (directory[i].first).c_str());
    	//cout<<string(path)<<"\n";
    	if(directory[i].second==0)
    	listFilesRecursively(path,directory);
    	i++;
    }

    cout<<directory.size()<<"\n";

    search(directory,string(argv[1]));
   // createDump(directory);
    return 0;
}



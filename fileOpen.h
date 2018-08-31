
#ifndef FILEOPEN_H
#define FILEOPEN_H

using namespace std;

void openFile(string filename)

{
    char file[filename.length()+1];

    for(int i=0;i<filename.length();i++)
        file[i] = filename[i];

    file[filename.length()+1] = NULL;

    int pid = fork();

    if (pid == 0) {
      execl("/usr/bin/xdg-open", "xdg-open", file, (char *)0);
      exit(1);
    }

}


#endif
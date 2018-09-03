#ifndef CREATEFILE_H
#define CREATEFILE_H

using namespace std;

void createFile(char* fileName,char* directory){
  char* pathFile;
  string str(directory);
  str = str +"/"+fileName;
  pathFile = (char*)str.c_str();
  int filedescriptor = open(pathFile, O_WRONLY | O_APPEND | O_CREAT);   
  chmod(pathFile, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);
}

#endif
#include "headerFiles.h"

using namespace std;

void copyFile(char source[], char destination[]) {

    int src_fd, dst_fd, n, err;
    unsigned char buffer[4096];
    char * src_path, *dst_path;

    src_path = source;
    dst_path = destination;


    
    src_fd = open(src_path, O_RDONLY);
    dst_fd = open(dst_path, O_CREAT | O_WRONLY);

    while (1) {
        err = read(src_fd, buffer, 4096);
        if (err == -1) {
            printf("Error reading file.\n");
            exit(1);
        }
        n = err;

        if (n == 0) break;

        err = write(dst_fd, buffer, n);
        if (err == -1) {
            printf("Error writing to file.\n");
            exit(1);
        }
    }
    chmod(dst_path, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);
    close(src_fd);
    close(dst_fd);
    
}

string SplitFilename (string& str)
{
  //cout << "Splitting: " << str << '\n';
  size_t found = str.find_last_of("/\\");
  cout << " path: " << str.substr(0,found) << '\n';
  cout << " file: " << str.substr(found+1) << '\n';
  return str.substr(found+1);
}


int main()
{
    string source,target;
    getline(cin,source);
    getline(cin,target);

    string temp = SplitFilename(source);
    
    cout<<"temp: "<<temp<<"\n";
    
    target = target+"/"+temp;
    cout<<"target: "<<target<<"\n";

    char s[source.length()+1];
    strcpy(s,source.c_str());
    
    printf("%s\n", s);
    char d[target.length()+1];
    strcpy(d,target.c_str());
    printf("%s\n", d);
    copyFile(s,d);
    return 0;
}

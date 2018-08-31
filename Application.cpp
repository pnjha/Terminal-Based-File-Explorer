#include "headerFiles.h"

using namespace std;

int INDEX = 0;


string homeDirectory;
string workingDirectory;
struct winsize size,CURR_ROW;

vector<pair<string,string>> presentDirectoryInfo;
vector<pair<string,int>> presentDirectory;
stack<string> traverseLeft;
stack<string> traverseRight;

void incrementIndex();
void decrementIndex();
void reset();
void displayStatusBar(int);
int isUpAllowed();
int isDownAllowed();
void setCursorTopRow();
void setCursorBottomRow();
void listDirectories();
int setCommandMode();
void displayStatusBar(int);
int setNormalMode();
void initializeVariables();
void gotoDirectory(string);

//////////////////////////////////////////////////////////// READ AND STORE DIRECTORY INFO //////////////////////////////////////////////////////////////////

static char perms_buff[30];


const char *get_perms(mode_t mode)
{
  char ftype = '?';

  if (S_ISREG(mode)) ftype = '-';
  if (S_ISLNK(mode)) ftype = 'l';
  if (S_ISDIR(mode)) ftype = 'd';
  if (S_ISBLK(mode)) ftype = 'b';
  if (S_ISCHR(mode)) ftype = 'c';
  if (S_ISFIFO(mode)) ftype = '|';

  sprintf(perms_buff, "%c%c%c%c%c%c%c%c%c%c %c%c%c", ftype,
  mode & S_IRUSR ? 'r' : '-',
  mode & S_IWUSR ? 'w' : '-',
  mode & S_IXUSR ? 'x' : '-',
  mode & S_IRGRP ? 'r' : '-',
  mode & S_IWGRP ? 'w' : '-',
  mode & S_IXGRP ? 'x' : '-',
  mode & S_IROTH ? 'r' : '-',
  mode & S_IWOTH ? 'w' : '-',
  mode & S_IXOTH ? 'x' : '-',
  mode & S_ISUID ? 'U' : '-',
  mode & S_ISGID ? 'G' : '-',
  mode & S_ISVTX ? 'S' : '-');

  return (const char *)perms_buff;
}

//char path_name[MAXPATHLEN];

void die(char *msg)
{
  perror(msg);
  exit(0);
}

static int
one (const struct dirent *unused)
{
  return 1;
}



int listDirectories(string currentDirectory){

  int count,i;
  struct dirent **files;
  struct stat statbuf;
  char datestring[256];
  struct passwd pwent;
  struct passwd *pwentp;
  struct group grp;
  struct group *grpt;
  struct tm time;
  char buf[1024];
  string temp;
  char path_name[currentDirectory.length()+1];

  strcpy(path_name,currentDirectory.c_str());


  count = scandir(path_name, &files, one, alphasort);

  if(count > 0)
  {
    //printf("total %d\n",count);

    for (i=0; i<count; ++i)
    {
      	stat(files[i]->d_name, &statbuf);
      
        /* Print out type, permissions, and number of links. */
      	string status = get_perms(statbuf.st_mode);
      	if(status[0]=='d'&&i>=2){
      		//sprintf(temp,"%s/%s",currentDirectory,files[i]->d_name);
      		temp = currentDirectory+"/"+string(files[i]->d_name);
      		presentDirectoryInfo.push_back(pair<string, string> (temp,currentDirectory));
			presentDirectory.push_back(pair<string, int> (temp,1));
      	}else if(status[0]!='d'&&i>=2){

      		//sprintf(temp,"%s/%s",currentDirectory,files[i]->d_name);
      		temp = currentDirectory+"/"+string(files[i]->d_name);
      		presentDirectoryInfo.push_back(pair<string, string> (temp,currentDirectory));
			presentDirectory.push_back(pair<string, int> (temp,0));

      	}

        printf(" %10.10s", get_perms(statbuf.st_mode));
        printf("\t%d", statbuf.st_nlink);

        if (!getpwuid_r(statbuf.st_uid, &pwent, buf, sizeof(buf), &pwentp))
          printf("\t%s", pwent.pw_name);
        else
          printf("\t%d", statbuf.st_uid);

        if (!getgrgid_r (statbuf.st_gid, &grp, buf, sizeof(buf), &grpt))
          printf("\t%s", grp.gr_name);
        else
          printf("\t%d", statbuf.st_gid);

        /* Print size of file. */
        printf("\t%5dKB\t", (int)statbuf.st_size/1024);

        localtime_r(&statbuf.st_mtime, &time);
        /* Get localized date string. */
        strftime(datestring, sizeof(datestring), "%F %T", &time);

        printf("\t%s\t%s\n", datestring, files[i]->d_name);
      

      free (files[i]);
    }

    free(files);
  }

}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////// SET NORMAL MODE /////////////////////////////////////////////////////////////////////////////////

int setNormalMode(){
  
  int c;
  system("clear");

  if (terminal_init()) {
    if (errno == ENOTTY)
        fprintf(stderr, "This program requires a terminal.\n");
    else
        fprintf(stderr, "Cannot initialize terminal: %s.\n", strerror(errno));
  return -1;
  }
  else{
  	reset();
    listDirectories(workingDirectory);
    displayStatusBar(NORMALMODE);
    setCursorTopRow();
    
  }

  //  printf("\033[1;1H");
  
  while ((c = getchar()) != EOF) {
    
    if (c == ':'){
      terminal_done();
      setCommandMode(); 
      return 0;
    }
    if(c == BACKSPACE){
    	string uplevelPath;

    	for(int i=0;i<presentDirectoryInfo.size();i++){
    		if(presentDirectoryInfo[i].first==workingDirectory){
    			uplevelPath = presentDirectoryInfo[i].second;
    		}
    	}
		cout<<"problem: "<<uplevelPath<<"\n";
    	presentDirectoryInfo.clear();
		presentDirectory.clear();
		traverseLeft.push(uplevelPath);
		reset();
		listDirectories(uplevelPath);
		
		displayStatusBar(NORMALMODE);
    }
    if ( c == HOME){
    	traverseLeft.push(homeDirectory);
    	workingDirectory = homeDirectory;
    	presentDirectoryInfo.clear();
		presentDirectory.clear();
		reset();
		listDirectories(homeDirectory);
		displayStatusBar(NORMALMODE);
    }
    if ( c == ARROWRIGHT) {
      //printf ( "\033[C");//cursor right
		if(!traverseRight.empty()){
			workingDirectory = traverseRight.top();
			traverseLeft.push(traverseRight.top());
			string targetPath = traverseRight.top();
			presentDirectoryInfo.clear();
			presentDirectory.clear();
			reset();	
			listDirectories(targetPath);
			traverseRight.pop();
		}

    }
    if ( c == ARROWLEFT) {
        //printf ( "\033[D");//cursor left
		if(!traverseLeft.empty()){
			traverseLeft.pop();
			workingDirectory = traverseLeft.top();
			traverseRight.push(traverseLeft.top());
			string targetPath = traverseLeft.top();
			presentDirectoryInfo.clear();
			presentDirectory.clear();
			reset();	
			listDirectories(targetPath);	
		}        
    }
    if ( c == ARROWUP) {
		if(isUpAllowed()){
			moveCursorUp();
			decrementIndex();
			//printf ( "\033[A");
		}
    }
    if ( c == ARROWDOWN) {
		if(isDownAllowed()){
			moveCursorDown();
			incrementIndex();
		//printf ( "\033[B");
		}
    }
    if (c == ENTER){
		if(presentDirectory[INDEX].second==1){
			workingDirectory = presentDirectoryInfo[INDEX].first;
			traverseLeft.push(presentDirectoryInfo[INDEX].first);
			string targetPath = presentDirectoryInfo[INDEX].first;
			presentDirectoryInfo.clear();
			presentDirectory.clear();
			reset();
			listDirectories(targetPath);
		}
		else if(presentDirectory[INDEX].second==0){
			openFile(presentDirectory[INDEX].first);
		}
	}
  }
  return 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////// COMMAND MODE ///////////////////////////////////////////////////////////////////////////////////


int setCommandMode(){

	setCursorBottomRow();
    displayStatusBar(COMMANDMODE);
    

    cout<<"\n\n";

    int i=0,count=0;
	string str,temp;
    
    while(getline(cin,str)){
        
        if(str[0]==ESC){
            setNormalMode();
            return 0;
        }
        else if(str.find("copy_file")==0){
            vector<string> tokens;
            char s = ' ';
            int l=0,len=0,size = str.size();
            for(int i=0;i<size;i++){
               if(str[i]==s){
                  tokens.push_back(str.substr(l,len));
                  len=0;
                  l= i+1;  
               }else{
                len++;
               } 
            }
            tokens.push_back(str.substr(l,str.size()));
                  
            //for(int i=0;i<tokens.size();i++)
             // cout<<tokens[i]<<"\n";  
            for(int i=1;i<tokens.size()-1;i++){
              copyFile(tokens[i],tokens[tokens.size()-1]);
            }

        }
        else if(str.find("copy_dir")==0){
            
        }
        else if(str.find("move_file")==0){
            
        }
        else if(str.find("move_dir")==0){
            
        }
        else if(str.find("rename")==0){
            i=0;
            while(str[i]!=' '){
                i++;
            }
            i++;
            temp = str.substr(i);
            i=0;
            count = 0;
            while(temp[i]!=' '){
                i++;
                count++;
            }
            char oldname[count];
            i=0;
            while(temp[i]!=' '){
                oldname[i] = temp[i];
                i++;
            }
            oldname[i] = '\0';
            i++;
            temp = temp.substr(i);
            char newname[temp.length()];
            i=0;
            while(i<temp.length()){
                newname[i] = temp[i];
                //cout<<c[i];
                i++;
            }
            newname[i]='\0';
            renameFile(oldname,newname);
        }
        else if(str.find("create_dir")==0){
            i=0;
            while(str[i]!=' '){
                i++;
            }
            i++;
            temp = str.substr(i);
            
            createDirectory(temp);
        }
        else if(str.find("create_file")==0){

            i=0;
            while(str[i]!=' '){
                i++;
            }
            i++;
            temp = str.substr(i);
            i=0;
            count = 0;
            while(temp[i]!=' '){
                i++;
                count++;
            }
            char file[count];
            i=0;
            while(temp[i]!=' '){
                file[i] = temp[i];
                i++;
            }
            file[i] = '\0';
            string fileName(file);
            i++;
            temp = temp.substr(i);
            char dire[temp.length()];
            i=0;
            while(i<temp.length()){
                dire[i] = temp[i];
                //cout<<c[i];
                i++;
            }
            dire[i] = '\0';
            string directory(dire);

            createFile(file,dire);
        }
        else if(str.find("delete_file")==0){
            i=0;
            while(str[i]!=' '){
                i++;
            }
            i++;   
            temp = str.substr(i);
            char c[temp.length()];
            //cout<<"temp.length()"<<temp.length()<<"\n";
            i=0;
            while(i<temp.length()){
                c[i] = temp[i];
                cout<<c[i];
                i++;
            }
            c[i]='\0';
            cout<<"\n"<<temp;
            deleteFile(c);
        }
        else if(str.find("delete_dir")==0){
            i=0;
            while(str[i]!=' '){
                i++;
            }
            i++;   
            temp = str.substr(i);
            char c[temp.length()];
            //cout<<"temp.length()"<<temp.length()<<"\n";
            i=0;
            while(i<temp.length()){
                c[i] = temp[i];
                cout<<c[i];
                i++;
            }
            c[i]='\0';
            cout<<"\n"<<temp;
            delete_folder_tree(c);
        }
        else if(str.find("goto")==0){
        	vector<string> tokens;
            char s = ' ';
            int l=0,len=0,size = str.size();
            for(int i=0;i<size;i++){
               if(str[i]==s){
                  tokens.push_back(str.substr(l,len));
                  len=0;
                  l= i+1;  
               }else{
                len++;
               } 
            }
            tokens.push_back(str.substr(l,str.size()));
            gotoDirectory(tokens[1]);  
        }
        else if(str.find("search")==0){

        }
        else if(str.find("snapshot")==0){
        	vector<string> tokens;
            char s = ' ';
            int l=0,len=0,size = str.size();
            for(int i=0;i<size;i++){
               if(str[i]==s){
                  tokens.push_back(str.substr(l,len));
                  len=0;
                  l= i+1;  
               }else{
                len++;
               } 
            }
            tokens.push_back(str.substr(l,str.size()));
            
            snapshot(tokens[1]);
        }
            
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////// MAIN FUNCTION //////////////////////////////////////////////////////////////////////////////////

int main(){
	initializeVariables();
	cout<<homeDirectory<<"\n";
	if(setNormalMode()==-1)
		printf("Some error happened\n");
	return 0;	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////// GOTO DIRECTORY /////////////////////////////////////////////////////////////////////////////

void gotoDirectory(string dir){
	reset();
	cout<<dir<<"\n";
	dir = homeDirectory+dir;
	workingDirectory = dir;
	setNormalMode();
	//cout<<dir<<"\n";
	traverseLeft.push(dir);
	presentDirectoryInfo.clear();
	presentDirectory.clear();
	//listDirectories(dir);
	SetCursor(1,0);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////// SET CURSOR BOTTOM ROW //////////////////////////////////////////////////////////////////////

void setCursorBottomRow(){

	CURR_ROW.ws_row=BOTTOMROW;
	CURR_ROW.ws_col=0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////// SET CURSOR TOP ROW //////////////////////////////////////////////////////////////////////

void setCursorTopRow(){

	CURR_ROW.ws_row=FIRSTROW;
	CURR_ROW.ws_col=0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////// RESET CURSOR //////////////////////////////////////////////////////////////////////////////


void reset()
{
	ClearScreen();
	INDEX = 0;
	SetCursor(0,0);
	CURR_ROW.ws_row=FIRSTROW;
	CURR_ROW.ws_col=0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////// INITIALIZE VARIABLE ///////////////////////////////////////////////////////////////////////

void initializeVariables(){

   char cwd[PATH_MAX];
   if (getcwd(cwd, sizeof(cwd)) != NULL) {
      // printf("Current working dir: %s\n", cwd);
   } else {
       perror("getcwd() error");
       
   }
   homeDirectory = string(cwd);
   workingDirectory = string(cwd);
   //cout<<homeDirectory<<"\n";
   INDEX = 0;
   //reset();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////// IS UP MOVEMENT ALLOWED INDEX ////////////////////////////////////////////////////////////////////

int isUpAllowed(){
	if(INDEX>MINROWINDEX)
		return TRUE;
	else
		return FALSE;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////// IS DOWN MOVEMENT ALLOWED INDEX //////////////////////////////////////////////////////////////////

int isDownAllowed(){
	if(INDEX<MAXROWINDEX)
		return TRUE;
	else
		return FALSE;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////// INCREMENT INDEX ////////////////////////////////////////////////////////////////////////////////

void incrementIndex(){
	INDEX++;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////// DECREMENT INDEX ////////////////////////////////////////////////////////////////////////////////

void decrementIndex(){
	INDEX--;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////// DISPLAY STATUS BAR //////////////////////////////////////////////////////////////////////////////

void displayStatusBar(int MODE){
	if(MODE==NORMALMODE){
		SetCursor(STATUSROW,0);
		//CURR_ROW.ws_row=STATUSROW;
		//CURR_ROW.ws_col=0;
		cout<<"**************************************************************************************************************************\n";
		cout<<"NORMAL MODE\n";
		cout<<"**************************************************************************************************************************\n";
		SetCursor(1,0);
		INDEX = 0;
	}else if(MODE == COMMANDMODE){
		reset();
		listDirectories(workingDirectory);
		SetCursor(STATUSROW,0);
		//CURR_ROW.ws_row=STATUSROW;
		//CURR_ROW.ws_col=0;
		cout<<"**************************************************************************************************************************\n";
		cout<<"COMMAND MODE\n";
		cout<<"**************************************************************************************************************************\n";
		SetCursor(STATUSROW+1,0);
		
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

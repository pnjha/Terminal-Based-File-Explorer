
#include<stdio.h>

main()
{
    FILE *fp;
    char filename[20];
    int status;
    printf("nEnter the file name:nn");
    scanf("%s",filename);
    
    fp = fopen(filename, "r");
    
    if(fp == NULL)
    {
        printf("Error:  file not found! check the directory!!nn");
    }
    fclose(fp);
    status = remove(filename);
    
    if( status == 0 )
    printf("%s file deleted successfully.n",filename);
    else
    {
        printf("Unable to delete the filen");
        perror("Error");
    }
    
    return 0;
}

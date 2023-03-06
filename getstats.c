/* getstats.c 
 *
 * CSC 360, Summer 2022
 *
 * - If run without an argument, dumps information about the PC to STDOUT.
 *
 * - If run with a process number created by the current user, 
 *   dumps information about that process to STDOUT.
 *
 * Please change the following before submission:
 *
 * Author: Bipasa Agrawal
 * Login:  bipasaagrawal@uvic.ca 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 * Note: You are permitted, and even encouraged, to add other
 * support functions in order to reduce duplication of code, or
 * to increase the clarity of your solution, or both.
 */


//Function print_detail() takes in the file descriptor(fd) and the name of the file (buf)
//It prints the name, threads and the context switches of a particular process;returns nothing

void print_detail(char *buf,FILE *fd) {
    int sum=0;
    int max=100;
    char filename[100];
    strncpy(filename,buf,100);
    while(fgets(buf,sizeof(char)*max, fd)) //reads info line-by-line until end of file
    {   
        if (strstr(buf, "Name")) {
            printf("%s", buf);
            }
        if (strstr(buf, "Threads")) {
            printf("Filename (if any): %s",filename);
            printf("%s", buf);
            }
        if (strstr(buf, "ctxt_switches")) {
            char str[100];
            int t;
            strncpy(str,buf,90);
            strtok(str, ":");
            t = atoi(strtok(NULL, ":"));
            sum=sum+t;
            }    
     }
    printf("Total context switches:  %d\n", sum);
}

//Function print_process_info takes in the process number
//It prints the relavant details of the particular process, returns nothing

void print_process_info(char * process_num) {
    FILE *fd;
    char name_status[20];
    char name_cmdline[20];
    //Here, strncpy and strcat are used to form the name of the files that needs to be opened since the process number depends on the user input
    strncpy(name_status,"/proc/",10);
    strcat(name_status,process_num); 
    strncpy(name_cmdline,name_status,20);
    strcat(name_cmdline,"/comm");
    strcat(name_status,"/status");
    
    fd = fopen(name_cmdline, "r");
    //error handling if the particular process does not exist
    if (fd == NULL) {
        printf("Process number %s not found.\n",process_num);
        return ;
    }
    char buf[100];
    
    fgets(buf,sizeof(char)*100, fd);  //here, fgets() reads info from a file line-by-line
    fclose(fd);
    
    fd = fopen(name_status, "r");
    //error handling if the particular process does not exist
    if (fd == NULL) {
        printf("Process number %s not found.\n",process_num);
        return ;
    }
    printf("Process number: %s\n", process_num);
    
    print_detail(buf,fd);
    fclose(fd);       
} 

// Function print_uptime takes in the first line from the uptime file
//It calculates the uptime in days, hrs, mins and secs and prints it
void print_uptime(char *buf) {
    int up = atoi(strtok(buf, " ")); //to extract the first part of the line and convert to int
    int days = up/ (24*3600);
    up =  up%(24*3600);
    int hrs = up/3600;
    up = up%3600;
    int mins = up/60;
    up=up%60;
    int secs=up;
    printf("Uptime:   %d days, %d hours, %d minutes, %d seconds\n",days,hrs,mins,secs);
}

//Function print_full_info is called when there is no specified process number
//It prints the relavent information of cpu, version and memory info 
void print_full_info() {
    FILE *fd;
    char buf[100];
    //Here, fopen() is used to open the filename provided. Error handling is not provided because the filename is in the code. 
    fd = fopen("/proc/cpuinfo", "r");
    int max=100;
    while(fgets(buf,sizeof(char)*max, fd))
    {
         if (strstr(buf, "model name")) {
            printf("%s", buf);
            }
        if (strstr(buf, "cpu cores")) {
            printf("%s", buf);
            break;
            }
    }
    fclose(fd); //used to close the file
    
    fd = fopen("/proc/version", "r");
    while(fgets(buf,sizeof(char)*max, fd))
    {
        printf("%s", buf);
    }
    fclose(fd);
    
    fd = fopen("/proc/meminfo", "r");
    fgets(buf,sizeof(char)*max, fd); //here, fgets() reads info from a file line-by-line
    printf("%s", buf);
    fclose(fd);
    
    fd = fopen("/proc/uptime", "r");
    fgets(buf,sizeof(char)*max, fd);
    print_uptime(buf);
    fclose(fd); 
}

//main function
int main(int argc, char ** argv) {  
    if (argc == 1) {
        print_full_info(); //called when there is no particular process number
    } else {
        print_process_info(argv[1]); //called to print info on a specific process
    }
}

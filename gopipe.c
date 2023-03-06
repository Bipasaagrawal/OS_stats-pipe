/* gopipe.c
 *
 * CSC 360, Summer 2022
 *
 * Execute up to four instructions, piping the output of each into the
 * input of the next.
 *
 * Please change the following before submission:
 *
 * Author: Bipasa Agrawal
 * Login:  bipasaagrawal@uvic.ca 
 */


/* Note: The following are the **ONLY** header files you are
 * permitted to use for this assignment! */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <wait.h>

#define MAX_LEN_LINE 280
int line_len=0;
char lines[MAX_LEN_LINE][MAX_LEN_LINE];
char *token[4][MAX_LEN_LINE];


//Function make_tokens convert array of input lines to array of words in each line and stores it in token variable
void make_tokens()
{
    char *t2;
    int num_tokens = 0;
    
    for(int k=0;k<line_len;k++)
    {
       num_tokens=0;
       t2 = strtok(lines[k], " "); 
        while (t2 != NULL && num_tokens < 10) {
            token[k][num_tokens] = t2;

            num_tokens++;
            t2 = strtok(NULL, " ");
        }
    }
}


//Function make_pipe() creates multiple pipe. It then checks how many commands have been entered and pipe the output of each cmd to the next one. It displays the results after all the cmds entered have been executed. 
void make_pipe()
{
    char *envp[] = { 0 }; //environment variable
    int status, pid_head,pid_sec,pid_third,pid_last; //different pids for the child processes,stored for the waitpid at the end
    int fd[2],fd2[2],fd3[2]; //file-descriptors
    //Since there can be a maximum of 4 commands, 3 pipes are opened. 
    pipe(fd);
    pipe(fd2);
    pipe(fd3);
    
    if(line_len>0) //Checking to see if any commands have been entered or its just empty
    {
        if ((pid_head = fork()) == 0) {

            if(line_len>1) //checks to see if output needs to be rerouted to next process; if only one command has been provided, output will need to go to console. 
            {
                dup2(fd[1], 1); //writing to fd[1] instead of console
            }
            else 
            {
                close(fd[1]);
            }
            //unused pipes are closed
            close(fd[0]);
            close(fd2[0]);
            close(fd2[1]);
            close(fd3[1]);
            close(fd3[0]);

            execve(token[0][0], token[0], envp);
            write(1,"First command is incorrect",26); //error handling to check if user input for first command is valid; if not, displays error message and returns without printing output
            return;
        }
        if(line_len>1)
        {
            if ((pid_sec = fork()) == 0) {
                dup2(fd[0], 0); //reads input from previous cmd pipe
                if(line_len>2)
                {
                    dup2(fd2[1], 1); //writes output to pipe to next process
                }
                else 
                {
                    close(fd2[1]);
                }
                close(fd[1]);
                close(fd2[0]);
                close(fd3[0]);
                close(fd3[1]);
                execve(token[1][0], token[1], envp); 
                write(1,"Second command is incorrect",27); //error handling to check if user input for second command is valid
               return;
            }
            if(line_len>2)
            {
                if ((pid_third = fork()) == 0) {                 
                    dup2(fd2[0], 0);//reads input from previous cmd pipe
                    if(line_len>3)
                    {
                        dup2(fd3[1], 1); //writes output to next process, if there are more than 3 cmds given
                    }
                    else {
                        close(fd3[1]);
                    }
                    close(fd[0]);
                    close(fd[1]);
                    close(fd2[1]);
                    close(fd3[0]);
                    execve(token[2][0], token[2], envp);
                    write(1,"Third command is incorrect",26);//error handling to check if user input for third command is valid
                    return;
                }
                if(line_len>3)
                { 
                    if ((pid_last = fork()) == 0) {
                        dup2(fd3[0], 0); //reads input from the previous process
                        close(fd[0]);
                        close(fd[1]);
                        close(fd2[1]);
                        close(fd2[0]);
                        close(fd3[1]);

                        execve(token[3][0], token[3], envp); //outputs the result if all four cmds have been given as input 
                        write(1,"Fourth command is incorrect",27); //error handling to check if user input for last command is valid
                        return;
                    }
                }
            }
        }
    }
    //Parent process
    
    //All the open ends of the pipe are closed for the parent process
    close(fd[0]);
    close(fd[1]);
    close(fd2[0]);
    close(fd2[1]);
    close(fd3[0]);
    close(fd3[1]);
    
    //Here, waitpid is used 4 times; Each one waits for the child process specified by the pid name to end. 
    waitpid(pid_head, &status, 0);
    waitpid(pid_sec, &status, 0); 
    waitpid(pid_third, &status, 0); 
    waitpid(pid_last, &status, 0);
}

//main function
int main() {
    char input[MAX_LEN_LINE];
    int t =read(0, input, MAX_LEN_LINE); //takes input from the user
    strncpy(lines[line_len],input,t-1);
    if(t>1) //checks to see if line entered is empty or not
    {
        line_len++; //line_len is used to count the number of cmds provided by user ie no. of lines
    }
    else {
        write(1,"No commands entered!",20); //when there is no input from user (only enter)
    }
    //checks to see if enter(\n)has been pressed and no of lines do not exceed 4
    while (input[strlen(input) -1] == '\n' && line_len<4  && t>1) {
        
        t = read(0, input, MAX_LEN_LINE);
        strncpy(lines[line_len],input,t-1); //copies each line into a new entry of arrray
        if(t>1)
        {
            line_len++;
        }
    }
    input[strlen(input) - 1] = '\0'; //end of input from user
    
    make_tokens(); //tokenizes each line array into an array of array (with each word as token)
    make_pipe();  //uses above array of array to pipe each cmd output into next one
}

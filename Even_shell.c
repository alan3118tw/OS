/*
 * Simple shell interface program.
 *
 * Operating System Concepts - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



#define MAX_LINE 80 /* 80 chars per line, per command */


void Strsol(char a[],char* args[],int *run_in_bg);

int main(void)
{
	char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
	char a[MAX_LINE];
   	int should_run = 1,run_in_bg,i;
	printf("Please input the command currently, wrong or empty input may cause error.\n");
		
    while (should_run)
	{  	
		run_in_bg = 0;
	    printf("osh>");
	    fflush(stdout);
		Strsol(a, args, &run_in_bg);

		pid_t pid_1 = fork();
		if(pid_1 < 0)			//if fork failed the fork() will return a negative number 
		{
		    printf("Child process fork failed.\n");
		}
		else if(pid_1 == 0 && args[0]!=NULL)		//child process
		{
			execvp(args[0],args);		//use execvp function to replace the child process		
		}	
		//only the parent process will run  will run the following code
		if(args[1]==NULL)
		{
			if(strcmp(args[0],"exit") == 0)  	//exit
				should_run = 0;
		}
		if(run_in_bg == 0)			//determine whether or not run concurrently
			wait(NULL);	
    }
	return 0;
}

void Strsol(char a[],char* args[],int *run_in_bg)
{
	int count=0,length,start_address=-1,i;
	length = read(STDIN_FILENO, a, MAX_LINE);//length of total command user was input
				
	if(length<=0)
		printf("Error\n");
	
	//start string processing
	else if(length > 0)
	{
		for(i=0;i<length;i++)
		{
			switch(a[i])
			{
				case'\t':		//when we encounter a tab or space
				case' ':
					if(start_address!=-1)
					{
						args[count]=&a[start_address];		//fill args[i] as the ith command
						count++;			//move to next args
					}
					start_address=-1;		//reset the start_address state
					a[i]='\0';
					break;

				case'\n':		//End of the input(Enter)
					if(start_address!=-1)
					{
						args[count]=&a[start_address];
						count++;			//move to next args
					}
					a[i]='\0';
					args[count] = NULL;		
			        break;

				case'&':		//fork a child to run concurrently
					*run_in_bg = 1;
         			a[i]='\0';
					break;

				default:		//record the commands segment's first address position
					if(start_address==-1)
					{	
						start_address = i;
					}
			}
		}	
		args[count] = NULL;		//add the last command as NULL
	}
}


















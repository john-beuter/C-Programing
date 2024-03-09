#include<stdio.h> 
#include <stdlib.h> //Allows for EXIT_SUCCESS or EXIT_FAILURE useful for returning errors
#include<unistd.h>
#include <errno.h>
#include <string.h> 
#include <sys/wait.h> 
#include <signal.h>



int main(int argc, char *argv[]) //if the second arguement == -p we are changing the prompt
{
	char prompt[30] = "";
	if(argc == 3 && strcmp("-p", argv[1]) == 0)  //strcmp string compare  
		strcat(prompt, argv[2]); //Strcat to add the input to the string
	else
		strcat(prompt, "308>");

	char input[30];//Making a char buffer to accept strings   //dynamically set this with malloc
	char * token;
    char * tokenHolder[30]; //Why a char star? I want to dynamically allocate this.
	char background[10];
	
	int pos;
	int pos2;

	int status;
	int status_normal;

	int backChild;
	int backChild_State;

	while(1)
	{
		prompt:
		while(backChild_State = waitpid(-1, &status, WNOHANG) > 0) // Waits for a child process to finish befor doing stuff.
		{	
			if(backChild_State > 0 && WIFEXITED(status)) //If it is the pid of the child
        	{
				printf("Child [%d] %s Exit %d\n", backChild, background, WEXITSTATUS(status));
			}
			else if(backChild_State > 0 && WIFSIGNALED(status))
			{
				printf("Child [%d] %s Killed (%d)\n", backChild, background, SIGTERM);
			}
		}

		
		printf("%s ", prompt);
		fgets(input, sizeof(input), stdin); 							
		input[strcspn(input, "\n")] = 0; 			//fgets does not remove new lines
		
		if(strcmp(input, "") == 0)
		{
			goto prompt;
		}
		else
		{
			token = strtok(input, " ");
			pos = 0;

			while(token != NULL)
    		{
    		    tokenHolder[pos++] = token;
    		    token = strtok(NULL, " ");
    		}
			tokenHolder[pos++] = '\0';  //Null terminates the next element. This helps prevent and residual values from being in the list

    		if(tokenHolder[0] == NULL)
    		{
    		    printf("It's NULL");
    		}
			char * strip; 
			strip = strtok(input, "/");
			char * stripHolder[30];
			pos2 = 0;

			while(strip != NULL)
    		{
				stripHolder[pos2++] = strip;
    		    strip = strtok(NULL, "/");
    		}
				
			stripHolder[pos2++] = '\0';  //Null terminates the next element. This helps prevent and residual values from being in the list
			if(pos2 > 2)  //If we have an actual token
    		{
				strcpy(tokenHolder[0], stripHolder[1]);
    		}
		}
		
		if(strcmp(tokenHolder[0], "exit") == 0)
		{
			exit(EXIT_SUCCESS);
		}
		else if(strcmp(tokenHolder[0], "cd") == 0)   // Look at the first value in the token list if it is cd then do cd stuff. Simple as that
		{
			if(tokenHolder[1] != NULL) // If null for 2 place do thisi
			{
				if (chdir(tokenHolder[1]) != 0)
				{
					perror("chdir() to token failed");
				} 
			}
			else 
			{
				chdir(getenv("HOME"));
			}
		}
		else if(strcmp(tokenHolder[0], "pid") == 0)
		{
			printf("%d\n", getpid());
		}

		else if(strcmp(tokenHolder[0], "ppid") == 0)
		{
			printf("%d\n", getppid());
		}
		else if(strcmp(tokenHolder[0], "pwd") == 0)
		{
			system("pwd"); 
		}
		else if(strcmp(tokenHolder[0], "clear") == 0) 
		{
			system("clear"); 
		}
		else
		{
			if((tokenHolder[1] != NULL && tokenHolder[2] != NULL) && strcmp(tokenHolder[2], "&") == 0) //Need to check if there is something first. If I don't check if it's null then I get a seg fault because i am accessing a random addres
			{
            	int pid = fork(); // create a child process
            	if (pid < 0) 
            	{ 
            	    fprintf(stderr, "fork failed\n");
            	    exit(1);
            	} 
            	else if (pid > 0) 
            	{
					backChild = pid;
					strcpy(background, tokenHolder[0]);
					printf("I am the background pid %d\n", backChild);
            		usleep(500);
            	} 
            	else 
            	{ 
            	    printf("Child [%d] %s Back\n", (int) getpid(), tokenHolder[0]);

					char* command = tokenHolder[0];
            		char* argument_list[] = {command, tokenHolder[1], NULL};
            		if(tokenHolder[1] == NULL)
                	{
                     	char* argument_list[] = {command, NULL};
                	}
            		int status_code = execvp(command, argument_list); 
            		if (status_code == -1) 
            		{
            		    perror("execvp");
            		    return 1;  //Are you needed?
            		}
            	}
        	}
        	else
        	{
            	int pid_t = fork(); // create a child process 

        		if (pid_t < 0) 
        		{ 
        		    fprintf(stderr, "fork failed\n");
        		    exit(1);
        		} 
        		else if (pid_t > 0) 
        		{ // parent runs specified execl command
        		    waitpid(pid_t, &status_normal, 0);

        		    if(WIFEXITED(status_normal))
        		    {
        		        printf("Child [%d] %s Exit %d\n", pid_t, tokenHolder[0], WEXITSTATUS(status_normal)); //How does the pid worK?
        		    }
        		} 
        		else 
        		{ // child goes down this path (main)
        		    printf("Child [%d] %s\n", (int) getpid(), tokenHolder[0]);
        		    char* command = tokenHolder[0];
        		    char* argument_list[] = {command, tokenHolder[1], NULL};
        		    if(tokenHolder[1] == NULL)
						{
        		            char* argument_list[] = {command, NULL};		
        		        }

        		    int status_code = execvp(command, argument_list);  //Argue ment list is the whole thing. I need to take the first part and then the whole thing. 

        		    if (status_code == -1) 
        		    {
        		        perror("execvp");
        		        return 1;
        		    }  
        		}
        	}	
	 	}
	}
	exit(EXIT_SUCCESS);
}
		
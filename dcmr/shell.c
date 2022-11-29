#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "func.h"

#define MAX_LINE 80 /* 80 chars per line, per command */

int main(int argc, char *argv[])
{
    char *args[MAX_LINE/2 + 1];	/* command line has max of 40 arguments */
    char *parsed_commands[MAX_LINE]; /* parsed commands by ; */
    char last_command[MAX_LINE]; /* var to save last command */
    int should_run = 1;		/* flag to help exit program*/
	int style = -1; /* 1 = sequential style // 2 = parallel style // 3 = batchfile*/
    char user_input[MAX_LINE];
    int count_commands;
    int last_command_exists = 0;

    if (argc > 2)
    {
        printf("Invalid number of arguments");
        exit(1);
    }

    if (argc == 1)
    {
        style = 1;
    }

    if (argc == 2)
    {
        style = 3;
    }
    
    while (should_run) {
        if (style == 1)
        {
            printf("dcmr seq> ");
            while (!fgets(user_input, MAX_LINE, stdin))
            {
                exit(1);
            }
            
            if (strcspn(user_input, "\n") == MAX_LINE - 1)
            {
                printf("Line exceeds character limit\n");
                for(int ch=getchar(); ch != '\n' && ch != EOF; ch=getchar());
                continue;
            }
            
            user_input[strlen(user_input) - 1] = '\0';
            
            style = verify_style_parallel(style, user_input);

            if (style == 2)
            {
                continue;
            }
            
            if (verifyHistory(user_input) == 0)
            {
                memcpy(last_command, user_input, sizeof(last_command));
                last_command_exists = 1;
            }
                 
            count_commands = 0;
            count_commands = parse_input(user_input, parsed_commands);
            
            for (int i = 0; i < count_commands; i++)
            {
                if (verifyHistory(parsed_commands[i]) == 1)
                {
                    if (last_command_exists == 1)
                    {
                        execHistory(last_command, style);
                        continue;
                    }
                    else
                    {
                        printf("No commands\n");
                        break;
                    }
                    
                }

                if (pipeCheck(parsed_commands[i]) == 1)
                {
                    execPipeSequential(parsed_commands[i]);
                    continue;
                }
                
                parse_command_by_space(args, parsed_commands[i]);
                verify_exit(args[0]);
                exec_commands_sequential(args);
            }
            
        }  

        if (style == 2)
        {
            printf("dcmr par> ");
            while (!fgets(user_input, MAX_LINE, stdin))
            {
                exit(1);
            }

            if (strcspn(user_input, "\n") == MAX_LINE - 1)
            {
                printf("Line exceeds character limit\n");
                for(int ch=getchar(); ch != '\n' && ch != EOF; ch=getchar());
                continue;
            }
            
            user_input[strlen(user_input) - 1] = '\0';

            style = verify_style_sequential(style, user_input);   
            
            if (style == 1)
            {
                continue;
            }   
            
            if (verifyHistory(user_input) == 0)
            {
                memcpy(last_command, user_input, sizeof(last_command));
                 last_command_exists = 1;
            }

            count_commands = 0;
            count_commands = parse_input(user_input, parsed_commands);

            for (int i = 0; i < count_commands; i++)
            {
                if (verifyHistory(parsed_commands[i]) == 1)
                {
                    if (last_command_exists == 1)
                    {
                        execHistory(last_command, style);
                        continue;
                    }
                    else
                    {
                        printf("No commands\n");
                        break;
                    }
                    
                }

                if (pipeCheck(parsed_commands[i]) == 1)
                {
                    execPipeParallel(parsed_commands[i]);
                    continue;
                }

                parse_command_by_space(args, parsed_commands[i]);
                verify_exit(args[0]);

                exec_commands_parallel(args);
                                   
            }
           
            for (int i = 0; i < count_commands + 1 ; i++)
            {
                wait(NULL);
            }     
        }    
                 
        if (style == 3)
        {
            runBatch(argv[1]);
            /*int style_batch = 1; 

            FILE *file, *file_print;
            
            file = fopen(argv[1], "r");
            file_print = fopen(argv[1], "r");

            if (file == NULL && file_print == NULL)
            {
                printf("Could not open file\n");
                exit(EXIT_FAILURE);

            }
            else
            {
                char print_batch[80];
                while (fgets(print_batch, MAX_LINE / 2 + 1, file_print) != NULL)
                {
                    printf("%s", print_batch);
                }
                
                printf("\n");
                        
            }
          
          fclose(file_print); */

        } 

    } 
    
    return 0;            
}
        
        
	

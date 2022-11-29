#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "func.h"


#define MAX_LINE 80 /* 80 chars per line, per command */

void parse_command_by_space(char **args, char *command)
{
    args[0] = strtok(command, " ");

    int count = 0;

    while (args[count] != NULL)
    {
        args[++count] = strtok(NULL, " ");
    }
    
}

void exec_commands_parallel(char **args)
{
    pid_t pid;

    pid = fork();
    
    if (pid == 0)
    {
        if (execvp(args[0], args) == -1)
        {
            fprintf(stderr, "command not found\n");
        }
        exit(1);
    }
    else if (pid < 0)
    {
        fprintf(stderr, "fork failed");      
    } 
    
}

void exec_commands_sequential(char **args)
{
    pid_t pid, wait_pid;

    pid = fork();
    
    if (pid == 0)
    {
        if (execvp(args[0], args) == -1)
        {
            fprintf(stderr, "command not found\n");
            exit(EXIT_FAILURE);
        }
        exit(1);
    }
    else if (pid < 0)
    {
        fprintf(stderr, "fork failed");   
        exit(EXIT_FAILURE);  
    }
    else
    {
        wait_pid = wait(NULL);
    }    
    
}

int verify_command_exists(char *command, char *commands[], int index)
{
    int status;

    for (int i = 0; i < 2; i++)
    {
        if (strncmp(command, commands[i], strlen(commands[i])) == 0)
        {
            status = 1; // command exists
        }
        else
        {
            status == 0;
        }
            
    }
    
    return status;
    
}

int parse_input(char *text_input, char *commands[])
{
    char* token = strtok(text_input, ";");
    int i = 0;
    int count = 0;

    while (token != NULL)
    {
        commands[i] = token;
        ++i;
        token = strtok(NULL, ";");
        count++;

    }
    return count;
}

void parse_by_pipe(char *input, char **commands){

    char* token = strtok(input, "|");
    int i = 0;
    int count = 0;

    while (token != NULL)
    {
        commands[i] = token;
        ++i;
        token = strtok(NULL, "|");
        count++;

    }
}

int verify_style_parallel(int style, char *input)
{
    int style_flag  = strncmp(input, "style parallel", strlen("style parallel"));

    if (style_flag == 0)
    {
        return style = 2;
    }
    else
    {
        return style = 1;
    }
    
}

int verify_style_sequential(int style, char *input)
{
    int style_flag  = strncmp(input, "style sequential", strlen("style sequential"));

    if (style_flag == 0)
    {
        return style = 1;
    }
    else
    {
        return style = 2;
    }
    
}

int verify_style_batchfile(int style, char *input)
{
    int style_flag = strncmp(strtok(input, " "), "./shell", strlen("./shell"));

    if (style_flag == 0)
    {
        return 3;
    }
    else
    {
        return 1;
    } 

}

void verify_exit(char *input)
{
    if (strncmp(input, "exit", strlen("exit")) == 0)
    {
        exit(EXIT_SUCCESS);
    }
    

}


int pipeCheck(char *user_input){

    int status = 0;
 
    if(strchr(user_input, '|') != NULL)
    {
        status = 1;
    } 
    
    return status;
}

void execPipeSequential(char *user_input){

    pid_t pid1, pid2;
    char *args1[MAX_LINE];
    char *args2[MAX_LINE];
    char *command[MAX_LINE];
    parse_by_pipe(user_input, command);
    char *pipe_arg_1 = strdup(command[0]);
    char *pipe_arg_2 = strdup(command[1]);

    parse_command_by_space(args1, pipe_arg_1);
    parse_command_by_space(args2, pipe_arg_2);

    int file_desc[2];

    if (pipe(file_desc) == -1)
    {
        fprintf(stderr, "Pipe error");
    }

    pid1 = fork(); 

    if (pid1 == 0)
    {
        close(file_desc[0]);
        dup2(file_desc[1], STDOUT_FILENO);

        if (execvp(args1[0], args1) == -1)
        {
            fprintf(stderr, "command not found\n");
        }
        exit(1);
        
    }
    else if (pid1 < 0)
    {
        fprintf(stderr, "fork failed");   
        exit(EXIT_FAILURE); 
    }

    pid2 = fork(); 

    if (pid2 == 0)
    {
        close(file_desc[1]);
        dup2(file_desc[0], STDIN_FILENO);

        if (execvp(args2[0], args2) == -1)
        {
            fprintf(stderr, "command not found\n");
        }
        exit(1);
        
    }
    else if (pid1 < 0)
    {
        fprintf(stderr, "fork failed");   
        exit(EXIT_FAILURE);   
    }

    close(file_desc[0]);
    close(file_desc[1]);

    if (pid1 >= 0)
    {
        wait(NULL);
        
        if(pid2 >= 0)
        {
            wait(NULL);
        }
    }
}

void execPipeParallel(char *user_input){

    pid_t pid1, pid2;
    char *args1[MAX_LINE];
    char *args2[MAX_LINE];
    char *command[MAX_LINE];
    parse_by_pipe(user_input, command);
    char *pipe_arg_1 = strdup(command[0]);
    char *pipe_arg_2 = strdup(command[1]);

    parse_command_by_space(args1, pipe_arg_1);
    parse_command_by_space(args2, pipe_arg_2);

    int file_desc[2];

    if (pipe(file_desc) == -1)
    {
        fprintf(stderr, "Pipe error");
    }

    pid1 = fork(); 

    if (pid1 == 0)
    {
        close(file_desc[0]);
        dup2(file_desc[1], STDOUT_FILENO);

        if (execvp(args1[0], args1) == -1)
        {
            fprintf(stderr, "command not found\n");
        }
        exit(1);
        
    }
    else if (pid1 < 0)
    {
        fprintf(stderr, "fork failed");   
        exit(EXIT_FAILURE); 
    }

    pid2 = fork(); 

    if (pid2 == 0)
    {
        close(file_desc[1]);
        dup2(file_desc[0], STDIN_FILENO);

        if (execvp(args2[0], args2) == -1)
        {
            fprintf(stderr, "command not found\n");
        }
        exit(1);
        
    }
    else if (pid1 < 0)
    {
        fprintf(stderr, "fork failed");   
        exit(EXIT_FAILURE);   
    }

    close(file_desc[0]);
    close(file_desc[1]);
}

void execRedirect(){

}

int verifyHistory(char *user_input){
    
    if (strncmp(user_input, "!!", strlen("!!")) == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
    
}

void execHistory(char *last_command, int style){     
    char *args[MAX_LINE/2 + 1];
    char *parsed_commands[MAX_LINE];
    char command[MAX_LINE];
    
    memcpy(command, last_command, sizeof(command));
    if (style == 1)
    {
        int count_commands = parse_input(command, parsed_commands);
            
        for (int i = 0; i < count_commands; i++)
        {
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
        int count_commands = parse_input(command, parsed_commands);
            
        for (int i = 0; i < count_commands; i++)
        {
            if (pipeCheck(parsed_commands[i]) == 1)
            {
                execPipeParallel(parsed_commands[i]);
                continue;
            }
            
            parse_command_by_space(args, parsed_commands[i]);
            verify_exit(args[0]);
        
            exec_commands_parallel(args);
        }

        for (int i = 0; i < count_commands; i++)
        {
            wait(NULL);
        }
            
    }
    
    

}

void copy_last_command(char **parsed_commands, char **last_command){
    memcpy(last_command, parsed_commands, sizeof(last_command));
}

void runBatch(char *argv){
    
    char *args[MAX_LINE/2 + 1];	/* command line has max of 40 arguments */
    char *parsed_commands[MAX_LINE]; /* parsed commands by ; */
    char last_command[MAX_LINE]; /* var to save last command */
    int should_run = 1;		/* flag to help exit program*/
	int style = 1; /* 1 = sequential style // 2 = parallel style*/
    char user_input[MAX_LINE];
    int count_commands;
    int last_command_exists = 0;

    char print_batch[80];
    FILE *file, *file_print;

    file_print = fopen(argv, "r");
    file = fopen(argv, "r");

    if (file == NULL && file_print == NULL)
    {
        printf("Could not open file\n");
        exit(EXIT_FAILURE);

    }   

    printf("Read commands: \n");

    while (fgets(print_batch, MAX_LINE / 2 + 1, file_print) != NULL)
    {
        if (strncmp(print_batch, "\n", strlen("\n")) != 0)
        {
            printf("%s", print_batch);
        }
        else
        {
            continue;
        }
        
    }
    
    printf("\nCommand execution: \n");

    while (should_run) {
        if (style == 1)
        {
            while (!fgets(user_input, MAX_LINE, file))
            {
                exit(1);
            }
            
            if (strcspn(user_input, "\n") == MAX_LINE - 1)
            {
                printf("Line exceeds character limit\n");
                for(int ch=getchar(); ch != '\n' && ch != EOF; ch=getchar());
                continue;
            }
            
            if (user_input[strlen(user_input) - 1] == '\n')
            {
                user_input[strlen(user_input) - 1] = '\0';
            }
            
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
            while (!fgets(user_input, MAX_LINE, file))
            {
                exit(1);
            }

            if (strcspn(user_input, "\n") == MAX_LINE - 1)
            {
                printf("Line exceeds character limit\n");
                for(int ch=getchar(); ch != '\n' && ch != EOF; ch=getchar());
                continue;
            }
            
            if (user_input[strlen(user_input) - 1] == '\n')
            {
                user_input[strlen(user_input) - 1] = '\0';
            }

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

    // while (fgets(user_input, MAX_LINE / 2 + 1, file) != NULL)
    // {
    //     if (style == 1)
    //     {
    //         if (user_input[strlen(user_input) - 1] == '\n')
    //         {
    //             user_input[strlen(user_input) - 1] = '\0';
    //         }

    //         if (strcspn(user_input, "\n") == MAX_LINE - 1)
    //         {
    //             printf("Line exceeds character limit\n");
    //             for(int ch=getchar(); ch != '\n' && ch != EOF; ch=getchar());
    //             continue;
    //         }
            
    //         user_input[strlen(user_input) - 1] = '\0';
            
    //         style = verify_style_parallel(style, user_input);

    //         if (style == 2)
    //         {
    //             continue;
    //         }
            
    //         if (verifyHistory(user_input) == 0)
    //         {
    //             memcpy(last_command, user_input, sizeof(last_command));
    //             last_command_exists = 1;
    //         }
                 
    //         count_commands = 0;
    //         count_commands = parse_input(user_input, parsed_commands);
            
    //         for (int i = 0; i < count_commands; i++)
    //         {
    //             if (verifyHistory(parsed_commands[i]) == 1)
    //             {
    //                 if (last_command_exists == 1)
    //                 {
    //                     execHistory(last_command, style);
    //                     continue;
    //                 }
    //                 else
    //                 {
    //                     printf("No commands\n");
    //                     break;
    //                 }
                    
    //             }

    //             if (pipeCheck(parsed_commands[i]) == 1)
    //             {
    //                 execPipeSequential(parsed_commands[i]);
    //                 continue;
    //             }
                
    //             parse_command_by_space(args, parsed_commands[i]);
    //             verify_exit(args[0]);
    //             exec_commands_sequential(args);
    //         }
            
    //     }        
    //}
    }
    fclose(file_print);
    fclose(file);
    exit(EXIT_SUCCESS); 
}
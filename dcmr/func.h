void clean(char **args, char **parsed_commands);

void parse_command_by_space(char **args, char *command);

void exec_commands_parallel(char **args);

void exec_commands_sequential(char **args);

int verify_command_exists(char *command, char *commands[], int index);

int parse_input(char *text_input, char *commands[]);

int verify_style_parallel(int style, char *input);

int verify_style_sequential(int style, char *input);

int verify_style_batchfile(int style, char *input);

void verify_exit(char *input);

int verify_history(char *input, int ls_exists, char *last_command, int style);

void copy_array(char **array1, char **array2);

int pipeCheck(char *user_input);

void execPipeSequential(char *user_input);

void execPipeParallel(char *user_input);

void parse_by_pipe(char *input, char **commands);

int verifyHistory(char *user_input);

void execHistory(char *last_command, int style, int count_commands);
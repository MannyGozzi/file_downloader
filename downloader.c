#define _GNU_SOURCE
#include "array_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define FILENAME_POS 1
#define MAX_PROCESS_POS 2

void validate_args(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Please enter at least 2 arguments: <filename> <num_processes>\n");
        exit(1);
    }
}

FILE *get_file(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Opening file");
        exit(1);
    }
    return file;
}

void wait_and_print_pid_status(struct array_list *pids, struct array_list *line_indicies)
{
    int status;
    pid_t pid = wait(&status);
    int line_num = get_element(line_indicies, find_element(pids, pid));
    if (WEXITSTATUS(status) == EXIT_SUCCESS)
    {  
        printf("Process \033[33m#%d\033[0m \033[32mCompleted\033[0m downloading for line \033[36m%d\033[0m\n", pid, line_num);
    }
    else if (pid != -1)
    {
        printf("Process \033[33m#%d\033[0m \033[31mFailed\033[0m to download for line \033[36m%d\033[0m\n", pid, line_num);
    } else {
        printf("An unknown process \033[31mFailed\033[0m to correctly\n");
    }
}

void download_from_file(FILE *file, int max_processes)
{
    char *line = NULL;
    size_t num_chars = 0;
    int fork_cnt = 0;
    pid_t pid;
    int line_num = 0;
    struct array_list *pids = new_array_list();
    struct array_list *line_indicies = new_array_list();

    while (getline(&line, &num_chars, file) > 0)
    {
        ++line_num;
        char *name = strtok(line, " \n");
        char *url = strtok(NULL, " \n");
        char *time = strtok(NULL, " \n");
        if (fork_cnt >= max_processes)
        {
            wait_and_print_pid_status(pids, line_indicies);
            fork_cnt--;
        }
        fork_cnt++;
        pid = fork();
        array_list_add_to_end(pids, pid);
        array_list_add_to_end(line_indicies, line_num);
        if (pid != 0)
        {
            printf("Process \033[33m#%d\033[0m \033[34mDownloading\033[0m [\033[33m%s\033[0m]", pid, name);
            printf(" for line \033[36m%d\033[0m\n", line_num);
        }
        if (pid == 0)
        {
            int success = 0;
            if (time == NULL) {
                success = execlp("curl", "curl", "-o", name, "-s", url, (char *)NULL);
            } else {
                success = execlp("curl", "curl", "-m", time, "-o", name, "-s", url, (char *)NULL);
            }
            if (success) {
                exit(EXIT_SUCCESS);
            }
            exit(EXIT_FAILURE);
        }
    }
    free(line);
    while (fork_cnt != 0)
    {
        wait_and_print_pid_status(pids, line_indicies);
        fork_cnt--;
    }
    // cleanup 
    destroy_array_list(pids);
    destroy_array_list(line_indicies);
}

int main(int argc, char *argv[])
{
    validate_args(argc, argv);
    FILE *file = get_file(argv[FILENAME_POS]);
    int MAX_PROCESSES = atoi(argv[MAX_PROCESS_POS]);
    download_from_file(file, MAX_PROCESSES);
    fclose(file);
    return 0;
}

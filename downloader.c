#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define FILENAME_POS 1
#define MAX_PROCESS_POS 2

void validate_args(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Please enter at least 2 arguments: <filename> <num_processes>\n");
        exit(1);
    } 
}

FILE* get_file(char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Opening file");
        exit(1);
    }
    return file;
}

void wait_and_print_pid_status() {
    int status;
    pid_t pid = wait(&status);
    if (pid != -1 && WIFEXITED(status)) {
            printf("Download for process \033[33m#%d\033[0m \033[32mcompleted\033[0m\n", pid);
    } else {
        printf("A process \033[31mfailed\033[0m\n");
    }
}

void download_from_file(FILE* file, int max_processes) {
    char* line = NULL;
    size_t num_chars = 0;
    int fork_cnt = 0;
    pid_t pid;
    int line_num = 0;
    while (getline(&line, &num_chars, file) > 0) {
        ++line_num;
        char* name = strtok(line, " \n");
        char* url = strtok(NULL, " \n");
        char* time = strtok(NULL, " \n");
        if (time == NULL) time = "10";
        if (fork_cnt >= max_processes) {
            wait_and_print_pid_status();
            fork_cnt--;
        }
        fork_cnt++;
        pid = fork();
        if (pid != 0) {
            printf("=> Starting download for \033[33m%s\033[0m", name);
            printf(" under process \033[33m#%d\033[0m for line \033[36m%d\033[0m\n", pid, line_num);
        }
        if (pid == 0) {
            if (execlp("curl", "curl", "-m", time, "-o", name, "-s", url, (char*) NULL) == -1) {
                perror("Download execlp");
                exit(1);
            }
        }
    }
    free(line);
    while(fork_cnt != 0) {
        wait_and_print_pid_status();
        fork_cnt--;
    }
}

int main(int argc, char *argv[])
{
    validate_args(argc, argv);
    FILE* file = get_file(argv[FILENAME_POS]);
    int MAX_PROCESSES = atoi(argv[MAX_PROCESS_POS]);
    download_from_file(file, MAX_PROCESSES);


    fclose(file);
    return 0;
}

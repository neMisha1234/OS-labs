#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#define BUFFER_SIZE 256


int read_filename(char *out_buf, int max_len) {
    int bytes_read = 0;
    char ch;

    while (bytes_read < max_len - 1) {
        int res = read(STDIN_FILENO, &ch, 1);

        if (res <= 0) {
            break;
        }

        if (ch == '\n') {
            break;
        }
        out_buf[bytes_read] = ch;
        bytes_read++;
    }

    out_buf[bytes_read] = '\0';
    return bytes_read;
}

pid_t born_child(int pipe_read_fd, const char *filename) {
    pid_t pid = fork();

    if (pid < 0) {
        write(STDERR_FILENO, "Failed to fork\n", 15);
        return -1;
    }

    if (pid == 0) {
        int file_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (file_fd < 0) {
            write(STDERR_FILENO, "Failed to open file\n", 20);
            _exit(1);
        }

        dup2(pipe_read_fd, STDIN_FILENO);
        dup2(file_fd, STDOUT_FILENO);

        close(pipe_read_fd);
        close(file_fd);

        char *const args[] = {"./child", NULL};
        char *const envp[] = {NULL};
        execve("./child", args, envp);

        write(STDERR_FILENO, "Failed to execve\n", 17);
        _exit(1);
    }
    return pid;
}

int main() {
    char file1[BUFFER_SIZE];
    char file2[BUFFER_SIZE];

    int len1 = read_filename(file1, BUFFER_SIZE);
    int len2 = read_filename(file2, BUFFER_SIZE);

    int pipe1[2], pipe2[2];
    pipe(pipe1);
    pipe(pipe2);
    pid_t child1 = born_child(pipe1[0], file1);
    pid_t child2 = born_child(pipe2[0], file2);
    close(pipe1[0]);
    close(pipe2[0]);

    char buffer[BUFFER_SIZE];
    int bytes_read;
    
    while ((bytes_read = read_filename(buffer, BUFFER_SIZE)) > 0) {
        buffer[bytes_read] = '\n';
        int total_bytes = bytes_read + 1;

        if (bytes_read > 10) {
            write(pipe2[1], buffer, total_bytes);
        } else {
            write(pipe1[1], buffer, total_bytes);
        }
    }

    close(pipe1[1]);
    close(pipe2[1]); 

    waitpid(child1, NULL, 0);
    waitpid(child2, NULL, 0);

    return 0;
}
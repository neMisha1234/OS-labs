#include <stdbool.h>
#include <unistd.h>

#define BUFFER_SIZE 256

static const char VERBS[] = {'a', 'e', 'i', 'o', 'u', 'y', 
                             'A', 'E', 'I', 'O', 'U', 'Y'};
static const int VERBS_COUNT = sizeof(VERBS) / sizeof(VERBS[0]);

bool is_verb(char c) {
    for (int i = 0; i < VERBS_COUNT; i++) {
        if (c == VERBS[i]) {
            return true;
        }
    }
    return false;
}

int transform(char *str, int len) {
    int write_index = 0;

    for (int read_index = 0; read_index < len; read_index++) {
        if (!is_verb(str[read_index])) {
            str[write_index] = str[read_index];
            write_index++;
        }
    }

    return write_index; 
}

int main() {
    char buffer[BUFFER_SIZE];
    int bytes_read;

    while ((bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0) {
        int new_length = transform(buffer, bytes_read);
        write(STDOUT_FILENO, buffer, new_length);
    }
    return 0;
}
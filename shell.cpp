#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <fcntl.h>
#define MAX_ARGS 64

void parse(char* input, char** args) {
    int i = 0; 
    char* token = strtok(input, " ");
    while (token != nullptr) {
        args[i++] = token; 
        token = strtok(nullptr, " ");
    }
    args[i] = nullptr; 
}

void execute(char** args) {
    pid_t pid = fork(); 
    //Child process will execute the terminal command 
    if (pid == 0) { 
        execvp(args[0], args);
        std::cerr << "mysh: command not found: " << args[0] << std::endl; 
        exit(1);
    } else if (pid > 0) {
        wait(nullptr);
    } else { 
        std::cerr << "fork failed" << std::endl;
    }
}

bool handle_builtin(char** args) {
    //exit case 
    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    }
    //cd case
    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == nullptr) {
            chdir(getenv("HOME"));
        } else {
            if (chdir(args[1]) != 0) {
                std::cerr << "mysh: cd: no such file or directory: " << args[1] << std::endl; 
            }
        }
        return true; 
    }
    return false; 
}

void execute_pipe(char** args1, char** args2) {
    int fd[2];
    pipe(fd);
    //fd[0] -> read end 
    //fd[1] -> write end 

    //fork child 1 - LHS of pipe 
    pid_t pid1 = fork(); 
    if (pid1 == 0) {
        dup2(fd[1], 1); //Overwrite fd 1 to point to pipe 
        close(fd[0]);   //Don't need write end of pipe 
        close(fd[1]);   //Close fd[1] -> can only have one fd pointing to endpoint
        execvp(args1[0], args1);
        std::cerr << "mysh: command not found: " << args1[0] << std:: endl; 
        exit(1);
    }

    //fork child 2 - RHS of pipe 
    pid_t pid2 = fork();
    if (pid2 == 0) {
        dup2(fd[0], 0);
        close(fd[1]);
        close(fd[0]);
        execvp(args2[0], args2);
        std::cerr << "mysh: command not found: " << args2[0] << std::endl;
        exit(1);
    }
    close(fd[0]);
    close(fd[1]);
    wait(nullptr);
    wait(nullptr);
}

void execute_redirect(char** args, char* filename, bool is_output) {
    pid_t pid = fork(); 
    if (pid == 0) {
        if (is_output) { //Writing to file 
            int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644); 
            if (fd < 0) {
                std::cerr << "mysh: cannot open file: " << filename << std::endl; 
                exit(1);
            }
            dup2(fd, 1);
            close(fd);
        } else {         //Reading from file 
            int fd = open(filename, O_RDONLY); 
            if (fd < 0) {
                std::cerr << "mysh: cannot open file: " << filename << std::endl;
                exit(1);
            }
            dup2(fd, 0); 
            close(fd);
        }
        execvp(args[0], args);
        std::cerr << "mysh: cannot open file: " << filename << std::endl;
        exit(1);
    } else if (pid > 0) {
        wait(nullptr);
    } else { 
        std::cerr << "fork failed" << std::endl; 
    }
}

int main() {
    std::string input; 

    while (true) {
        std::cout << "mysh> " << std::flush;
        if (!std::getline(std::cin, input)) {
            break; 
        }
        if (input.empty()) {
            continue; 
        }

        //Copy input into char array since parse will modify the string
        char buf[1024];
        strcpy(buf, input.c_str());

        //Check for redirection and pipe 
        char* out_pos = strchr(buf, '>');
        char* in_pos = strchr(buf, '<');
        char* pipe_pos = strchr(buf, '|');
        if (out_pos != nullptr) {
            *out_pos = '\0';
            char* filename = out_pos + 1; 
            while (*filename == ' ') filename++; 
            char* args[MAX_ARGS];
            parse(buf, args);
            execute_redirect(args, filename, true);
        } else if (in_pos != nullptr) {
            *in_pos = '\0';
            char* filename = in_pos + 1; 
            while (*filename == ' ') filename++; 
            char* args[MAX_ARGS];
            parse(buf, args);
            execute_redirect(args, filename, false);
        } else if (pipe_pos != nullptr) {
            *pipe_pos = '\0';
            char* left = buf; 
            char* right = pipe_pos + 1; 
            char* args1[MAX_ARGS];
            char* args2[MAX_ARGS];
            parse(left, args1);
            parse(right, args2);
            execute_pipe(args1, args2);
        } else {
            char* args[MAX_ARGS]; 
            parse(buf, args);
            if (!handle_builtin(args)) {
                execute(args);
            }
        }
    }
    std::cout << "goodbye\n"; 
    return 0; 
}
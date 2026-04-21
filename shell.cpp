#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

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
        char* args[MAX_ARGS]; 
        parse(buf, args);
        if (!handle_builtin(args)) {
            execute(args);
        }
    }
    std::cout << "goodbye\n"; 
    return 0; 
}
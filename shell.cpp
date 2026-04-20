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

        for (int i = 0; args[i] != nullptr; i++) {
            std::cout << "arg[" << i << "] = " << args[i] << std::endl;
        } 
    }
    std::cout << "goodbye\n"; 
    return 0; 
}
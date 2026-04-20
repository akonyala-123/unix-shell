#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

#define MAX_ARGS 64

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
        std::cout << "got: " << input << std::endl; 
    }
    std::cout << "goodbye\n"; 
    return 0; 
}
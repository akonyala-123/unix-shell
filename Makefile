CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

shell: shell.cpp
	$(CXX) $(CXXFLAGS) -o shell shell.cpp

clean:
	rm -f shell
#@authour Adiessa Bell and Nathanael Pelling
#@date 10/12/2021
#@info cource COP4634

#The compiler to use.
CXX = g++
#The flags to use.
CXXFLAGS = -g -std=c++11 -Wall -lpthread -Werror=return-type -Werror=uninitialized

#The list of object files of the classes.
SOURCES = Timer.o

#Names the executable mt-collatz and compiles the file.
mt-collatz: mt-collatz.cpp $(SOURCES)
	   $(CXX) $(CXXFLAGS) -o $@ $^

#Removes the executable.
clean:
	$(RM) *.o mt-collatz

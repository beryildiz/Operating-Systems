BIN_SIMPLE = main
INCDIR     = ./include/
LIBDIR     = ./lib/
SRCDIR     = ./
CFLAGS     = -Wall -Werror -Wpedantic -g -I$(INCDIR)
CPPFLAGS   = $(CFLAGS) -std=c++11 

# executables
all: $(OBJFILES) | $(BIN_SIMPLE)

$(BIN_SIMPLE): $(SRCDIR)/main.o  
	g++ -o $@ $^

# object files
%.o: %.c
	gcc -c -o $@ $(CFLAGS) $^

%.o: %.cpp
	g++ -c -o $@ $(CPPFLAGS) $^

.PHONY: clean
clean:
	$(RM) $(BIN_SIMPLE)
	$(RM) $(wildcard $(SRCDIR)*.o)
	$(RM) $(wildcard $(LIBDIR)*.o)
	

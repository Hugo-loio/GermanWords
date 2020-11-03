.POSIX:

CC 		= gcc 
CFLAGS 		= -W -O

GTK_FLAGS	= `pkg-config --cflags gtk+-3.0`
GTK_LIB		= `pkg-config --libs gtk+-3.0`

INCLUDE 	= -I include/

SRC1 		= $(notdir $(wildcard main/*.c)) 

EXE 		= $(addprefix bin/, $(SRC1:.c=.exe))

SRC2 		= $(notdir $(wildcard src/*.c))

OBJS 		= $(addprefix bin/, $(SRC2:.c=.o))

PROTECT 	= $(EXE:.exe=.o)

VPATH 		= src main

.PRECIOUS: $(OBJS) $(PROTECT)

all: bin $(EXE)

bin/%.exe: bin/%.o $(OBJS)
	$(CC) $(CFLAGS) $(GTK_FLAGS) $^ $(INCLUDE) $(GTK_LIB) -o $@

bin/%.o: %.c
	$(CC) -c $(CFLAGS) $(GTK_FLAGS) $^ $(INCLUDE) $(GTK_LIB) -o $@

bin:
	mkdir bin

clean:
	rm -fv bin/*

test:
	@echo "Testing..."
	@echo "\nEXE:"
	@echo $(EXE) 
	@echo "\nOBJS:"
	@echo $(OBJS)
	@echo "\nPROTECT:"
	@echo $(PROTECT)	

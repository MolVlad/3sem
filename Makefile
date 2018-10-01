PROJECT = homework2

###########
#Sources

SOURCES_S =
SOURCES_C = src/main.c src/split.c

SOURCES = $(SOURCES_S) $(SOURCES_C)
OBJS = $(SOURCES_S:.s = .o) $(SOURCES_C:.c=.o)

INCLUDES += src
INCLUDES += includes

DEFINES =

CFLAGS = -Wall -Wextra
CFLAGS += $(INCLUDES) $(DEFINES)

RM = rm -f

.PHONY: all clean

all: clean $(PROJECT) #$(PROJECT)
	gcc -o $(PROJECT) $(PROJECT).o

$(PROJECT): $(OBJS)

%.o:
	gcc $(CFLAGS) -c $<

clean:
	$(RM) $(OBJS)

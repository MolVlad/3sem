PROJECT = exe

PRECOMP = includes/precomp.h
SOURCES = src/main.c src/list.c src/read_from_file.c src/sem.c src/shm.c
INCLUDES += -I includes
DEFINES = -DPARSE_COMMAND_LINE -DNDEBUG #-DCONSOLE
CFLAGS = -Wall -Wextra
LIBS = -lpthread
COMP = gcc

PRECOMP_OBJ = $(PRECOMP:.h=.h.gch)
OBJS = $(SOURCES:.c=.o)

CFLAGS += $(DEFINES) $(INCLUDES) $(DEFINES)

RM = rm -rf

.PHONY: all clean go tags

link: $(OBJS)
	$(COMP) -o $(PROJECT) $(OBJS) $(LIBS)

all: clean link

clean:
	$(RM) $(OBJS)

%.o: %.c
	$(COMP) -c $(CFLAGS) $(@:.o=.c) $(LIBS) -o $@

tags:
	ctags -R

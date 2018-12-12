PROJECT = server

SOURCES = src/main.c ../libs/src/my_string.c src/user_communication.c src/handler_communication.c src/zombie.c src/state.c ../libs/src/fifo.c ../libs/src/msg.c src/sem.c
INCLUDES += -I includes -I ../libs/includes
DEFINES =
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
	$(RM) $(OBJS) $(PROJECT)

%.o: %.c
	$(COMP) -c $(CFLAGS) $(@:.o=.c) $(LIBS) -o $@

tags:
	ctags -R

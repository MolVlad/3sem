PROJECT = process_handler

SOURCES = src/main.c ../libs/src/my_string.c src/user_communication.c src/database_communication.c src/fight_zombie_process.c src/server_state_machines.c ../libs/src/fifo_config.c ../libs/src/msg_queue_config.c ../libs/src/sem_config.c src/signal_handlers.c
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

clean_obj:
	$(RM) $(OBJS)

%.o: %.c
	$(COMP) -c $(CFLAGS) $(@:.o=.c) $(LIBS) -o $@

tags:
	ctags -R

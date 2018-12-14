PROJECT = database

SOURCES = src/main.c src/htable.c src/btree.c ../libs/src/my_string.c ../libs/src/sem_config.c ../libs/src/fifo_config.c ../libs/src/msg_queue_config.c src/config_signal.c src/handler_state_machines.c src/sending_instruction_by_handler.c
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

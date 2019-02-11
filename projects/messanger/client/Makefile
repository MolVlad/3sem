PROJECT = client

SOURCES = src/main.c src/state_machines.c src/server_connection.c src/server_feedback.c src/server_api.c src/my_string.c src/sem_config.c src/threads.c src/signal_handlers.c
INCLUDES += -I includes
DEFINES =
CFLAGS = -Wall -Wextra
LIBS = -lpthread
COMP = gcc

PRECOMP_OBJ = $(PRECOMP:.h=.h.gch)
OBJS = $(SOURCES:.c=.o)

CFLAGS += $(DEFINES) $(INCLUDES) $(DEFINES)

RM = rm -rf

.PHONY: all clean go tags clean_obj

all: clean link clean_obj

link: $(OBJS)
	$(COMP) -o $(PROJECT) $(OBJS) $(LIBS)

clean:
	$(RM) $(OBJS) $(PROJECT)

clean_obj:
	$(RM) $(OBJS)

%.o: %.c
	$(COMP) -c $(CFLAGS) $(@:.o=.c) $(LIBS) -o $@

tags:
	ctags -R

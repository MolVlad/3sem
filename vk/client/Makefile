PROJECT = client

SOURCES = src/main.c src/console.c src/print.c src/menu.c ../libs/src/my_string.c src/sem.c
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

PROJECT = homework2

SOURCES = src/main.c src/split.c
OBJS = $(SOURCES:.c=.o)

INCLUDES += -I src
INCLUDES += -I includes

DEFINES = -DNDEBUG #-DCONSOLE

CFLAGS = -Wall -Wextra
CFLAGS += $(DEFINES) $(INCLUDES) $(DEFINES)

COMP = gcc
RM = rm -rf

.PHONY: all clean go tags

all: link

clean:
	$(RM) $(OBJS)

link: $(OBJS)
	$(COMP) -o $(PROJECT) $(OBJS)

%.o:
	gcc -c $(CFLAGS) $(@:.o=.c) -o $@

tags:
	ctags -R

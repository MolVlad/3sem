PROJECT = exe

PRECOMP = includes/precomp.h
SOURCES = src/main.c src/split.c
INCLUDES += -I includes
DEFINES = -DPARSE_COMMAND_LINE -DNDEBUG #-DCONSOLE
CFLAGS = -Wall -Wextra
COMP = gcc

PRECOMP_OBJ = $(PRECOMP:.h=.h.gch)
OBJS = $(SOURCES:.c=.o)

CFLAGS += $(DEFINES) $(INCLUDES) $(DEFINES)

RM = rm -rf

.PHONY: all clean go tags

link: $(PRECOMP_OBJ) $(OBJS)
	$(COMP) -o $(PROJECT) $(OBJS)

all: clean link

clean:
	$(RM) $(OBJS)

clean_all: clean
	$(RM) $(PRECOMP_OBJ)

$(PRECOMP_OBJ):
	$(COMP) -c $(CFLAGS) $(PRECOMP)

%.o: %.c
	$(COMP) -c $(CFLAGS) $(@:.o=.c) -include $(PRECOMP) -o $@

tags:
	ctags -R

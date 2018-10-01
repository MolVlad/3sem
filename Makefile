PROJECT = homework2

SOURCES = src/main.c src/split.c

INCLUDES += -I includes

DEFINES = -DCONSOLE

CFLAGS = -Wall -Wextra
CFLAGS += $(DEFINES) $(INCLUDES) $(DEFINES)

RM = rm -f

.PHONY: all clean go

all: clean go

go:
	gcc $(CFLAGS) $(SOURCES) -o $(PROJECT)

clean:
	$(RM) *.o

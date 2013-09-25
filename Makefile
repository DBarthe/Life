
CC ?= gcc

INCPATH = ./includes/

CFLAGS += -Wall -Wextra -I $(INCPATH)

OFLAGS = -O3

LDFLAGS	= -lSDL -lSDL_ttf -lSDL_image

TARGET = life

SRC =	src/main.c \
	src/life.c \
	src/get_next_line.c \
	src/options.c \
	src/console.c \
	src/graphic.c \
	src/import.c \
	src/map.c

OBJ = $(SRC:.c=.o)

all: CFLAGS+= $(OFLAGS)
all: compile

debug: CFLAGS+= -DDEBUG -g
debug: compile

compile: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(TARGET)

re:	fclean all

re-debug: fclean debug
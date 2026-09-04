CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99
TARGET = main

SRCS = main.c busca_largura.c busca_profundidade.c ../pratica01/grafo_lista.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET) $(OBJS)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
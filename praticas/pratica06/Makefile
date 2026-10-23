CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = main
OBJS = main.o dijkstra.o
 
all: $(TARGET)
 
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)
 
%.o: %.c dijkstra.h
	$(CC) $(CFLAGS) -c $< -o $@
 
clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe
 
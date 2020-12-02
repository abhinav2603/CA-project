CC = gcc
CFLAGS = -lpthread

READER = string_reader
GENERATOR = string_generator
RELIABLE = reliability

all: $(READER) $(GENERATOR) $(RELIABLE)

$(READER): string_reader.c meltdown.c
	$(CC) string_reader.c meltdown.c $(CFLAGS) -w -o $(READER)

$(GENERATOR): string_generator.c
	$(CC) string_generator.c meltdown.c $(CFLAGS) -w -o $(GENERATOR)

$(RELIABLE): reliability.c
	$(CC) reliability.c meltdown.c $(CFLAGS) -w -o $(RELIABLE)

clean:
	rm -f $(READER)
	rm -f $(GENERATOR)
	rm -f $(RELIABLE)

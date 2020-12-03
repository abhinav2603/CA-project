CC = gcc
CFLAGS = -lpthread

READER = string_reader
GENERATOR = string_generator
RELIABLE = reliability
RELIABLE2 = reliability2

all: $(READER) $(GENERATOR) $(RELIABLE) $(RELIABLE2)

$(READER): string_reader.c meltdown.c
	$(CC) string_reader.c meltdown.c $(CFLAGS) -w -o $(READER)

$(GENERATOR): string_generator.c
	$(CC) string_generator.c meltdown.c $(CFLAGS) -w -o $(GENERATOR)

$(RELIABLE): reliability.c
	$(CC) reliability.c meltdown.c $(CFLAGS) -w -o $(RELIABLE)

$(RELIABLE2): reliability2.c
	$(CC) reliability2.c meltdown.c $(CFLAGS) -w -o $(RELIABLE2)

clean:
	rm -f $(READER)
	rm -f $(GENERATOR)
	rm -f $(RELIABLE)
	rm -f $(RELIABLE2)

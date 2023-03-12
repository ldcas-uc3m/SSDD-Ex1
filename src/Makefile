BIN_FILES  = cliente servidor
VPATH = lib test  # path with source files not found in current dir
CC = gcc

CFLAGS = -g -I$(INSTALL_PATH)/include -Wall

LDFLAGS = -L$(INSTALL_PATH)
LDLIBS = -lpthread -lm -lrt


all: $(BIN_FILES)
.PHONY : all

cliente: cliente.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $^ -o $@
	
servidor: servidor.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $^ -o $@
	
test: test.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $^ -o $@
	
test_client: test_client.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $^ -o $@
	
test_server: test_server.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $^ -o $@
	
linked_list: linked_list.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $^ -o $@

# TODO: libclaves.so

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

clean:
	rm -f $(BIN_FILES) *.o *.so

.SUFFIXES:
.PHONY : clean
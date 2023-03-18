BIN_FILES  = cliente servidor
TEST_FILES = test_seq test_conc test_linked_list
VPATH = lib tests  # path with source files not found in current dir
CC = gcc

CFLAGS = -g -I$(INSTALL_PATH)/include -Wall -fPIC
LDFLAGS = -L$(INSTALL_PATH)/lib/
LDLIBS = -pthread -lm -lrt


all: $(BIN_FILES)
.PHONY : all


cliente: cliente.o lib/libclaves.so
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -L$(INSTALL_PATH) -lclaves $^ -o $@
	
servidor: servidor.o server_impl.o linked_list.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $^ -o $@
	
test_linked_list: test_linked_list.o linked_list.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $^ -o $@

test_conc: test_conc.o lib/libclaves.so
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -L$(INSTALL_PATH) -lclaves $^ -o $@
	
test_seq: test_seq.o lib/libclaves.so
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -L$(INSTALL_PATH) -lclaves $^ -o $@


lib/libclaves.so: claves.o
	$(CC) -shared $(CFLAGS) $(LDFLAGS) $(LDLIBS) $^ -o $@


%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<


tests: $(TEST_FILES)
.SUFFIXES:
.PHONY : tests


clean:
	rm -f $(BIN_FILES) $(TEST_FILES) *.o *.so lib/*.so

.SUFFIXES:
.PHONY : clean
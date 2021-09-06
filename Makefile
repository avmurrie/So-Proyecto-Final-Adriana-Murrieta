DEPS = common.h
CFLAGS = -Wall -c

LDFLAGS = -L/usr/local/lib -I/usr/lib
INCLUDE = -I/usr/local/include -I/usr/include -Iinclude
LDLIBS = -lcurl


.PHONY: all debug sanitize clean
all: server client

server: server.o common.o $(DEPS)
	gcc -o $@ server.o common.o $(DFLAGS) -lpthread

client: client.o common.o $(DEPS)
	gcc -o $@ client.o common.o $(DFLAGS)
	
twilio: c_sms.c twilio.c
	gcc -o c_sms $(INCLUDE) $(LDFLAGS) $(DFLAGS) $(SOURCES) $(LDLIBS)
	

%.o: %.c $(DEPS)
	gcc $(CFLAGS) $< $(DFLAGS)

# Compila usando la opción -g para facilitar la depuración con gdb.
debug: DFLAGS = -g
debug: clean all

# Compila habilitando la herramienta AddressSanitizer para
# facilitar la depuración en tiempo de ejecución.
sanitize: DFLAGS = -fsanitize=address,undefined
sanitize: clean all

clean:
	rm -rf server client *.o

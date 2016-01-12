CPPFLAGS ?= -O0 -Wall
LIBS = -losmocore
all: qxdm-log

qxdm-log: framing.o serial.o qxdm-log.o config.o
	$(CC) $(CPPFLAGS) -o $@ $^ $(LIBS)

clean:
	@rm -f qxdm-log *.o

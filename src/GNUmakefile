CPPFLAGS ?= -O0 -Wall
all: qxdm-log

qxdm-log: framing.o serial.o qxdm-log.o
	$(CC) $(CPPFLAGS) -o $@ $^ $(LIBS)

clean:
	@rm -f qxdm-log *.o

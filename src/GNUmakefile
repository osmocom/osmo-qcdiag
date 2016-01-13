CPPFLAGS ?= -g -O0 -Wall
LIBS = -losmocore
all: qxdm-log

qxdm-log: framing.o serial.o qxdm-log.o config.o gprs_rlc.o gprs_mac.o diag_gsm.o protocol.o
	$(CC) $(CPPFLAGS) -o $@ $^ $(LIBS)

clean:
	@rm -f qxdm-log *.o

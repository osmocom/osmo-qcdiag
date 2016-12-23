CPPFLAGS ?= -g -O0 -Wall `pkg-config --cflags qmi-glib`
LIBS ?= -losmocore `pkg-config --libs qmi-glib`
all: qxdm-log

qxdm-log: framing.o qxdm-log.o config.o gprs_rlc.o gprs_mac.o diag_gsm.o protocol.o qmi_decode.o
	$(CC) $(CPPFLAGS) -o $@ $^ $(LIBS)

clean:
	@rm -f qxdm-log *.o

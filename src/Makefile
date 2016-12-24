CPPFLAGS ?= -g -O0 -Wall `pkg-config --cflags qmi-glib`
LIBS ?= -losmocore `pkg-config --libs qmi-glib`
all: qxdm-log

MODS_LOG = gprs_rlc.o gprs_mac.o diag_gsm.o diag_log.o diag_log_gsm.o diag_log_gprs.o diag_log_umts.o diag_log_qmi.o

qxdm-log: diagchar_hdlc.o diag_io.o qxdm-log.o diag_msg.o protocol.o $(MODS_LOG)
	$(CC) $(CPPFLAGS) -o $@ $^ $(LIBS)

clean:
	@rm -f qxdm-log *.o

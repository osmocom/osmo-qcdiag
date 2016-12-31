CPPFLAGS ?= -g -O0 -Wall `pkg-config --cflags libosmocore` `pkg-config --cflags qmi-glib`
LIBS ?= `pkg-config --libs libosmocore` `pkg-config --libs qmi-glib`
all: osmo-qcdiag-log

MODS_LOG = gprs_rlc.o gprs_mac.o diag_gsm.o diag_log.o diag_log_gsm.o diag_log_gprs.o diag_log_umts.o diag_log_qmi.o diag_dpl.o

osmo-qcdiag-log: diagchar_hdlc.o diag_io.o osmo-qcdiag-log.o diag_msg.o protocol.o $(MODS_LOG)
	$(CC) $(CPPFLAGS) -o $@ $^ $(LIBS)

clean:
	@rm -f osmo-qcdiag-log *.o

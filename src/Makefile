CFLAGS ?= -g -O0 -Wall
LIBS = libosmocore qmi-glib

all: osmo-qcdiag-log

MODS_LOG = gprs_l1.o gprs_rlc.o gprs_mac.o diag_gsm.o diag_log.o diag_log_gsm.o diag_log_gprs.o diag_log_wcdma.o diag_log_umts.o diag_log_qmi.o diag_dpl.o diag_log_simcard.o diag_event.o

%.o: %.c
	$(CC) $(shell pkg-config --cflags $(LIBS)) $(CFLAGS) -c -o $@ $^

osmo-qcdiag-log: diagchar_hdlc.o diag_io.o osmo-qcdiag-log.o diag_msg.o protocol.o diag_cmd.o $(MODS_LOG)
	$(CC) $(shell pkg-config --libs $(LIBS)) -o $@ $^

clean:
	$(RM) osmo-qcdiag-log *.o

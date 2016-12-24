#include <stdint.h>
#include <stdio.h>

#include <libqmi-glib.h>

#include "diag_log.h"
#include "log_codes_qmi.h"

/* A small wrapper around libqmi-glib to give us a human-readable string
 * representation of QMI messages that we receive from DIAG */
static int dump_qmi_msg(const uint8_t *data, unsigned int len)
{
	GByteArray *buffer;
	GError *error = NULL;
	QmiMessage *message;
	gchar *printable;

	buffer = g_byte_array_sized_new(len);
	g_byte_array_append(buffer, data, len);

	message = qmi_message_new_from_raw(buffer, &error);
	if (!message) {
		fprintf(stderr, "qmi_message_new_from_raw() returned NULL\n");
		return -1;
	}

	printable = qmi_message_get_printable(message, "QMI ");
	fputs(printable, stdout);
	g_free(printable);

	return 0;
}

static void handle_qmi_msg(struct log_hdr *lh, struct msgb *msg)
{
	dump_qmi_msg(lh->data, lh->len);
}

#define CORE(x)	(0x1000 + x)

/* Tx and Rx might be switched */
#define LOG_QMI_PORT_RX(i)	CORE(LOG_QMI_RESERVED_CODES_BASE_C+(i*2))
#define LOG_QMI_PORT_TX(i)	CORE(LOG_QMI_RESERVED_CODES_BASE_C+(i*2)+1)

static const struct diag_log_dispatch_tbl log_tbl[] = {
	{ CORE(LOG_QMI_CALL_FLOW_C), handle_qmi_msg },
	{ CORE(LOG_QMI_SUPPORTED_INTERFACES_C), handle_qmi_msg },
	{ LOG_QMI_PORT_RX(0), handle_qmi_msg },
	{ LOG_QMI_PORT_RX(1), handle_qmi_msg },
	{ LOG_QMI_PORT_RX(2), handle_qmi_msg },
	{ LOG_QMI_PORT_RX(3), handle_qmi_msg },
	{ LOG_QMI_PORT_RX(4), handle_qmi_msg },
	{ LOG_QMI_PORT_RX(5), handle_qmi_msg },
	{ LOG_QMI_PORT_RX(6), handle_qmi_msg },
	{ LOG_QMI_PORT_RX(7), handle_qmi_msg },
	{ LOG_QMI_PORT_RX(8), handle_qmi_msg },
	{ LOG_QMI_PORT_RX(9), handle_qmi_msg },
	{ LOG_QMI_PORT_RX(10), handle_qmi_msg },
	{ LOG_QMI_PORT_RX(11), handle_qmi_msg },
	{ LOG_QMI_PORT_RX(12), handle_qmi_msg },
	{ LOG_QMI_PORT_RX(13), handle_qmi_msg },
	{ LOG_QMI_PORT_RX(14), handle_qmi_msg },
	{ LOG_QMI_PORT_RX(15), handle_qmi_msg },
	{ LOG_QMI_PORT_TX(0), handle_qmi_msg },
	{ LOG_QMI_PORT_TX(1), handle_qmi_msg },
	{ LOG_QMI_PORT_TX(2), handle_qmi_msg },
	{ LOG_QMI_PORT_TX(3), handle_qmi_msg },
	{ LOG_QMI_PORT_TX(4), handle_qmi_msg },
	{ LOG_QMI_PORT_TX(5), handle_qmi_msg },
	{ LOG_QMI_PORT_TX(6), handle_qmi_msg },
	{ LOG_QMI_PORT_TX(7), handle_qmi_msg },
	{ LOG_QMI_PORT_TX(8), handle_qmi_msg },
	{ LOG_QMI_PORT_TX(9), handle_qmi_msg },
	{ LOG_QMI_PORT_TX(10), handle_qmi_msg },
	{ LOG_QMI_PORT_TX(11), handle_qmi_msg },
	{ LOG_QMI_PORT_TX(12), handle_qmi_msg },
	{ LOG_QMI_PORT_TX(13), handle_qmi_msg },
	{ LOG_QMI_PORT_TX(14), handle_qmi_msg },
	{ LOG_QMI_PORT_TX(15), handle_qmi_msg },
};

static __attribute__((constructor)) void on_dso_load_qmi(void)
{
	diag_log_reg_dispatch(log_tbl, ARRAY_SIZE(log_tbl));
}

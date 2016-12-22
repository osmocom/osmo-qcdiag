#include <stdint.h>
#include <stdio.h>

#include <libqmi-glib.h>

int dump_qmi_msg(const uint8_t *data, unsigned int len)
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

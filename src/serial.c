#include <termios.h>
#include <stdio.h>
#include <errno.h>

int serial_configure(int fd)
{
	struct termios term;
	int rc;

	rc = tcgetattr(fd, &term);
	if (rc != 0) {
		printf("Error with tcgetattr: %d\n", errno);
		return -1;
	}

	/* taken from libqcdm but I doubt that is copyrightable */
	term.c_cflag &= ~(CBAUD | CSIZE | CSTOPB | CLOCAL | PARENB);
	term.c_iflag &= ~(HUPCL | IUTF8 | IUCLC | ISTRIP | IXON | IXOFF | IXANY | ICRNL);
	term.c_oflag &= ~(OPOST | OCRNL | ONLCR | OLCUC | ONLRET);
	term.c_lflag &= ~(ICANON | ISIG | IEXTEN | ECHO | ECHOE | ECHOK | ECHONL);
 	term.c_lflag &= ~(NOFLSH | XCASE | TOSTOP | ECHOPRT | ECHOCTL | ECHOKE);
	term.c_cc[VMIN] = 1;
 	term.c_cc[VTIME] = 0;
 	term.c_cc[VEOF] = 1;
	term.c_cflag |= (B115200 | CS8 | CREAD | 0 | 0);

	rc = tcsetattr(fd, TCSANOW, &term);
	if (rc != 0) {
		printf("Failed to set new attributes: %d\n", errno);
		return -2;
	}

	return 0;
}

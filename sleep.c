#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main (int n, char *  a[])
{
	if (n > 1) {
		// suspend without locking
		if (0 == strcmp("nolock", a[1]))
			return execl("/lib/systemd/systemd-sleep", "systemd-sleep", "suspend", NULL);
	} else {
		// suspend with lock
		return execlp("systemctl", "systemctl", "suspend", NULL);
	}
}

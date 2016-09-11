#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

char * slug = "/dev/rtc0";
char name[128];
char out[512];
struct tm mytime;
struct rtc_time rt;
int fd;

void copy(struct rtc_wkalrm x)
{
	mytime.tm_sec = x.time.tm_sec;
	mytime.tm_min = x.time.tm_min;
	mytime.tm_hour = x.time.tm_hour;
	mytime.tm_mday = x.time.tm_mday;
	mytime.tm_mon = x.time.tm_mon;
	mytime.tm_year = x.time.tm_year;
}

void _db(char c, long call)
{
	struct rtc_wkalrm x;
	if (call == RTC_WKALM_RD) {
		ioctl(fd, call, &x);
	}
	else {
		ioctl(fd, call, &x.time);
	}
	printf("%d %d %d %d %d %d %d %d\n", 
		x.enabled,
		x.pending,
		x.time.tm_sec,
	    x.time.tm_min,
	    x.time.tm_hour,
	    x.time.tm_mday,
	    x.time.tm_mon,
	    x.time.tm_year
		);
	copy(x);
	time_t t = mktime(&mytime);
	printf("%s", asctime(&mytime));
	printf("%s", asctime(localtime(&t)));
}

void rtcget()
{
	int fd = open("/dev/rtc", O_RDONLY);
	ioctl(fd, RTC_RD_TIME, &rt);
	printf(" %d %d %d %d %d %d\n", 
		rt.tm_sec,
	    rt.tm_min,
	    rt.tm_hour,
	    rt.tm_mday,
	    rt.tm_mon,
	    rt.tm_year
		);
	close(fd);
}

void debug(char c)
{
	name[8] = c;
	printf("\n%s\n", name);
	fd = open(name, O_RDONLY);
	_db(c, RTC_WKALM_RD);
	_db(c, RTC_ALM_READ);
	// _db(c, RTC_RD_TIME);
	// rtcget();
	close(fd);
}

int main (int n, char * a[])
{
	strcpy( &name[0], slug);
	debug('0');
	debug('1');
	debug(0)	;
	return 0;
}
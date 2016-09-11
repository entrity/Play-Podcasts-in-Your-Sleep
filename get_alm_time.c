#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

#define THRESHOLD_SEC 10
#define RTC_DEV_PATH "/dev/rtc0"

int fd;

void debug_rtc(struct rtc_time * t)
{
	printf("sec %d min %d hr %d \nd %d m %d y %d wd %d yd %d dst %d\n",
		t->tm_sec,
		t->tm_min,
		t->tm_hour,
		t->tm_mday,
		t->tm_mon,
		t->tm_year,
		t->tm_wday,
		t->tm_yday,
		t->tm_isdst
	);
}

void debug_tm(struct tm * t)
{
	printf("sec %d min %d hr %d \nd %d m %d y %d wd %d yd %d dst %d\n",
		t->tm_sec,
		t->tm_min,
		t->tm_hour,
		t->tm_mday,
		t->tm_mon,
		t->tm_year,
		t->tm_wday,
		t->tm_yday,
		t->tm_isdst
	);
}

/* Copy rtc_time fields to tm */
void rtc_to_time(struct rtc_time * r, struct tm * t)
{
	t->tm_sec   = r->tm_sec;
	t->tm_min   = r->tm_min;
	t->tm_hour  = r->tm_hour;
	t->tm_mday  = r->tm_mday;
	t->tm_mon   = r->tm_mon;
	t->tm_year  = r->tm_year;
	t->tm_wday  = r->tm_wday;
	t->tm_yday  = r->tm_yday;
	t->tm_isdst = -1;
}

struct tm * print_time(struct rtc_time * rt)
{
	struct tm * t_p;
	t_p = (struct tm *) malloc(sizeof(struct tm));
	rtc_to_time(rt, t_p);
	printf("time %s", asctime(t_p));
	return t_p;
}

void print_alarm()
{
	struct rtc_time rt;
	ioctl(fd, RTC_ALM_READ, &rt);
	print_time(&rt);
}

int is_alarm_time()
{
	time_t delta;
	struct tm alm_time, cur_time;
	struct rtc_time alm_rtc, cur_rtc;
	ioctl(fd, RTC_ALM_READ, &alm_rtc);
	ioctl(fd, RTC_RD_TIME, &cur_rtc);
	#ifdef DEBUG
	print_time(&alm_rtc);
	print_time(&cur_rtc);
	#endif
	rtc_to_time(&alm_rtc, &alm_time);
	rtc_to_time(&cur_rtc, &cur_time);
	// find difference
	time_t a = mktime(&alm_time);
	time_t c = mktime(&cur_time);
	delta = mktime(&alm_time) - mktime(&cur_time);
	#ifdef DEBUG
	debug_rtc(&alm_rtc);
	debug_rtc(&cur_rtc);
	debug_tm(&alm_time);
	debug_tm(&cur_time);
	printf("alm %s", asctime(localtime(&a)));
	printf("cur %s", asctime(localtime(&c)));
	printf("alm %ld\n", a);
	printf("cur %ld\n", c);
	#endif
	if (delta < 0) delta *= -1;
	#ifdef DEBUG
	printf("delta %ld\n", delta);
	#endif
	return delta > THRESHOLD_SEC;
}

int main (int n, char * a[])
{
	int exit_code = 0;
 	fd = open(RTC_DEV_PATH, O_RDONLY);
 	if (fd < 0) exit errno;
 	#ifdef DEBUG
 	printf("fd %d\n", fd);
 	#endif
 	if (1 == n) {
 		#ifdef DEBUG
 		printf("then\n");
 		#endif
 		print_alarm();
 	} else {
 		#ifdef DEBUG
 		printf("else\n");
 		#endif
 		exit_code = is_alarm_time();
 		#ifdef DEBUG
 		if (exit_code) dprintf(2, "Error %d\n", exit_code);
 		#endif
 	}
 	close(fd);
	return exit_code;
}

all: sleep get
	@chmod +x system-sleep-script.sh
	@chmod +x play-podcasts-upon-waking.sh
	@chmod +x schedule-rtcwake.sh

sleep: sleep.c
	@gcc sleep.c -o gotosleep
	@sudo chown root:root gotosleep
	@sudo chmod +s gotosleep

get: get_alm_time.c
	@gcc get_alm_time.c -o get_alm_time
	@sudo chown root:root get_alm_time
	@sudo chmod +s get_alm_time

sleep: sleep.c
	gcc sleep.c -o gotosleep
	sudo chown root:root gotosleep
	sudo chmod +s gotosleep
	sudo mv gotosleep /usr/local/bin/

get: get_alm_time.c
	gcc get_alm_time.c -o get_alm_time
	sudo chown root:root get_alm_time
	sudo chmod +s get_alm_time
	sudo mv get_alm_time /usr/local/bin

all: sleep get system-sleep-script.sh
	sudo cp system-sleep-script.sh /lib/systemd/system-sleep/markham
	sudo chmod +x /lib/systemd/system-sleep/markham

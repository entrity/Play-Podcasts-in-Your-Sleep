#!/bin/sh
# This should be copied/linked into /lib/systemd/system-sleep/ and given +x permissions.

PW64=

if [ post = $1 ] && [ suspend = $2 ] && [ $(whoami) = root ]; then
	if get_alm_time 10; then
		export XAUTHORITY=~markham/.Xauthority
		export DISPLAY=:0
		# Login at login screen, then call the script to play the podcasts
		su -c "xdotool type $(printf $PW64 | base64 -d)" markham &&\
		su -c 'xdotool key Return' markham &&\
		su -c 'bash /usr/local/src/play-podcasts-upon-waking.sh' markham
	fi
fi
exit 0

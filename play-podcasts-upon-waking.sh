PLAYLIST_FILE=/home/markham/gPodder/Downloads/Nakamura-144037/playlist.m3u
LOG_FILE=/home/markham/gPodder/log.txt
SLEEP_TIME=$((3 * 60 * 60))
TARGET_VOLUME=36
FADE_SLEEP_TIME=0.2

[ $1 = "no-sleep" ] && NO_SLEEP=1

set_vol ()
{
	amixer -q -D pulse sset Master "$1%"
}

# Turn off screen (until next keypress)
xset dpms force off

# Get current volume
ORIG_VOLUME=$(amixer -D pulse sget Master | tail -n1 |grep -o '\[.*%\]' | tr -d \[\])

# Start volume at zero
VOLUME=0

# Start player in background
printf "Starting smplayer... \t" >> $LOG_FILE
date >> $LOG_FILE
smplayer "$PLAYLIST_FILE" -actions "normal_speed dec_speed dec_speed_4 dec_speed_1 dec_speed_1 play" &

# Fade in volume
while [ $VOLUME -lt $TARGET_VOLUME ]; do
	set_vol $VOLUME
	VOLUME=$(( $VOLUME + 1 ))
	sleep $FADE_SLEEP_TIME
done
set_vol $TARGET_VOLUME

# Leave player running for a specified number of seconds
sleep $SLEEP_TIME
printf "Sleep terminated... \t" >> $LOG_FILE
date >> $LOG_FILE

# Fade out volume
while [ $VOLUME -gt 0 ]; do
	VOLUME=$(( $VOLUME - 1 ))
	set_vol $VOLUME
	sleep $FADE_SLEEP_TIME
done
set_vol 0

# Stop player
while pid=$(ps -C smplayer | tail -n-1 | cut -d' ' -f1) && ! [[ -z $pid ]]; do
	kill $pid
	echo killed $pid
done
echo >> $LOG_FILE

# Restore original volume
set_vol $ORIG_VOLUME

# Suspend computer (optionatl)
(($NO_SLEEP)) || systemctl suspend
#!/bin/bash

fatal ()
{
	2> echo $1
	exit 1
}

# Get username
USER=$(whoami)
# Get password
read -s -p "Password for $USER: " -r PW
echo
[[ -z $PW ]] && fatal 'No password supplied'
PW64=$(printf $PW | base64)
# Get playlist file
PLAYLIST="$1"
[[ -z $PLAYLIST ]] && read -e -p "Playlist m3u: " -r PLAYLIST
[[ -r $PLAYLIST ]] || fatal 'Unreadable playlist file'
# Get logfile
LOGFILE="$2"
[[ -z $LOGFILE ]] && read -e -p "Logfile: " -r LOGFILE
[[ -z $LOGFILE ]] && LOGFILE=/dev/null

sed -i 's/PLAYLIST_FILE=.*/PLAYLIST_FILE='${PLAYLIST//\//\\\/}'/' play-podcasts-upon-waking.sh || exit 2
sed -i 's/LOG_FILE=.*/LOG_FILE='${LOGFILE//\//\\\/}'/' play-podcasts-upon-waking.sh || exit 3

sed -i 's/PW64=/PW64='${PW64//\//\\\/}'/' system-sleep-script.sh || exit 4

make all || exit 5
sudo mv get_alm_time /usr/local/bin || exit 6
sudo mv gotosleep /usr/local/bin/ || exit 7
sudo cp system-sleep-script.sh /lib/systemd/system-sleep/play-podcasts-if-alarm-wake.sh || exit 8
sudo cp play-podcasts-upon-waking.sh /usr/local/src/ || exit 9

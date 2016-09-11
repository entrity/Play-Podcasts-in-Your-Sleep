The purpose of this project is to add a helpful sleep utility and to create a system that wakes the computer in the middle of the night and quietly plays a playlist of podcasts.

The wake-and-play system requires the existence of an environment variable PW64, which is the base64-encoded password which is to be entered on the login screen. This variable must be available to the root user, for use in `system-sleep-script.sh`.

## Files

- **sleep.c** : Not essential to the wake-up-play-podcast system. This creates a helpful command for putting the computer to sleep. The purpose is to set the UID bit on this executable so that a keyboard shortcut can be created for quick and easy suspension.
- **system-sleep-script.sh** : This should be copied/linked into /lib/systemd/system-sleep/ and given +x permissions. When the computer wakes from any sleep/suspension, it will run this script. The script calls `get_alm_time` and conditionally runs `wake-and-play.sh`.
- **get_alm_time.c** : Used at time of wake up from suspension/sleep to guess whether the cause of the wakeup was RTC's alarm.
- **wake-and-play.sh** : Runs smplayer to play a playlist of podcasts. After a specified duration, the script kills smplayer and returns the computer to suspension.
- **schedule-rtcwake.sh** : Short script used to schedule a wakeup at a particular time.
- **Makefile** : Makefile
- **smplayer.keys** : Text file showing the names of actions that can be passed to a running smplayer with `smplayer -send-action <action>`
- **diag.c ** : Used to discover how RTC and rtcwake work.

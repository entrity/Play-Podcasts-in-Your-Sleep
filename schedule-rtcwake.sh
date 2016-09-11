#!/bin/sh
target=$(date --iso-8601 --date="Tomorrow 1:00")
sudo rtcwake -m no --date "$target"

#!/bin/bash

# run doesemu in background
dosemu release/game.exe &
pid=$!

# wait for some time to give the game time to start properly
sleep 2

# now take a screenshot
import -window root screenshot.png

# now shut down dosemu
kill $pid
wait $pid

echo "Success!"

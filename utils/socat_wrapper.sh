#!/bin/bash

# Temporary file to store PTY paths
tempfile=$(mktemp)

# Create a new tmux session
tmux new-session -d -s mysession

# Run socat in the first pane
tmux send-keys "socat -d -d pty,raw,echo=0 pty,raw,echo=0 2>&1 | tee $tempfile" C-m

# Wait for socat to capture the PTYs and get from tempfile
sleep 2
pts1=$(sed -n '1p' "$tempfile" | awk '{print $NF}')
pts2=$(sed -n '2p' "$tempfile" | awk '{print $NF}')

tmux split-window -v; tmux send-keys "watch -n 1 ls $pts1" C-m
tmux split-window -v; tmux send-keys "watch -n 1 ls $pts2" C-m

tmux select-pane -t 0
tmux split-window -v; tmux send-keys "./main $pts1" C-m
tmux split-window -v; tmux send-keys "cat $pts2" C-m

# Attach to the tmux session, kill on detach
tmux attach-session -t mysession
tmux kill-session

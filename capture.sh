#!/bin/sh
current_time=$(date "+%Y.%m.%d_%H.%M.%S")
filename="capture_"$current_time".pcap"

killall tcpdump
tcpdump -i any udp port 4729 -w $filename &
tcpdump_pid=$!
./src/osmo-qcdiag-log $*
kill -9 $tcpdump_pid


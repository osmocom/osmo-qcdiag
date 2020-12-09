#!/bin/bash

while [[ "$#" -gt 0 ]]; do
    case $1 in
        -s|--serialpath) serialpath="$2"; shift ;;
        -i|--ip) gsmtapip="$2"; shift ;;
        -f|--filename) filename="$2"; shift ;;
        -Q|--qcdebug) qcdebug=1 ;;
        *) echo "unknown parameter: $1"; exit 1 ;;
    esac
    shift
done

#killall tcpdump

if [ -n "$gsmtapip" ]; then
tcpdump -i any udp port 4729 and dst $gsmtapip -w $filename &
tcpdump_pid=$!
./src/osmo-qcdiag-log -s $serialpath -G -i $gsmtapip
else
tcpdump -i any udp port 4729 -w $filename &
tcpdump_pid=$!
./src/osmo-qcdiag-log -s $serialpath -G
fi

kill -9 $tcpdump_pid

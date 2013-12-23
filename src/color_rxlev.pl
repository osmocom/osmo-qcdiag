#!/usr/bin/perl -w
use strict;

while (my $l = <STDIN>) {
	chomp($l);
	#"15:39:37 l1_tch.c            : Meas Rpt: Serv RXLEV=21,Ncell RXLEV=0"
	#"15:39:30 l1_send.c           : Serving RXQUAL: FULL=0 SUB=0 DTX=0"
	if ($l =~ /^\d+:\d+:\d+\s+l1_send\.c\s+: Serving RXQUAL: FULL=(\d+) SUB=(\d+).*/) {
		my $rxlev = $1;
		if ($rxlev >= 3) {
			# enable red background and black foreground */
			printf "\033[30;41m";
		}
		printf("%s", $l);
		printf("\033[0m\n");
	}
}

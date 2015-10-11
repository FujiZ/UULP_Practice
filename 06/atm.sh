#!/bin/zsh
#
#atm.sh - a wrapper for two programs
#
while true do
	do_a_transaction
	if play_again then
		continue
	fi
	break
done


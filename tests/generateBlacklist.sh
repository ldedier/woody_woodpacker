#!/bin/bash

source include.sh

function checkIfBlockingBinary {
	binary=$1
	if [[ "${blackerlist[@]}" =~ "$(basename $binary)" ]]; then
		return
	fi
	{ $binary < /dev/null ; pkill -9 sleep ; } &
	sleep 1
	echo ended sleeping 1
	echo "last pid: $!"
	if [ "$(ps | grep $!)" ]; then
		echo $binary >> $blacklistFile
		echo "killing pid $!"
		pkill -9 "$(basename $binary)"
		kill -9 $!
	fi
}

function generateBlackList {
	echo "#!/bin/sh" > $blacklistFile
	echo -n "blacklist=(" >> $blacklistFile
	applyPATH checkIfBlockingBinary
	echo ")" >> $blacklistFile
	chmod 755 $blacklistFile
}

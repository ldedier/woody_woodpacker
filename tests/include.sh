#!/bin/bash

NAME='../woody_woodpacker'
PACKED_NAME='to_change_woody'
WoodyPackerErrorsDir=couldNotBePacked
blacklistFile=blacklist.sh
GREEN='\033[0;32m'
RED='\033[0;31m'
EOC='\033[0m'
blacklistFile=blacklist.sh
blackerlist=(calibrate_ppa "[" vim bash script gnome-keyring-d yes)

function applyELF {
	file=$1

	diff <(cat $file | head -c 4) <(printf '\x7fELF') > /dev/null
	if [ $? -eq 0 ]; then
	#	echo -e "${GREEN}${file} is ELF${EOC}"
	#	checkFileELF $file
		$2 $file
	else
	#	echo -e "${RED}${file} is not ELF${EOC}"
		return ;
	fi
}

function applyDir {
	dir=$1
	if [ ! "$(ls -A $dir)" ]; then
  		echo "$dir is Empty"
		return
	fi
	for file in $dir/*
	do
		applyELF $file $2
	done

}

function applyPATH {
	for dir in $(echo $PATH | tr ":" "\n")
	do
		applyDir $dir $1
	done
}

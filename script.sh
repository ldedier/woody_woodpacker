#!/bin/bash

blacklist=(baobab
alsaloop
aseqdump
aseqnet
bluetooth-sendto
bluetoothctl
broadwayd
btmgmt
)

NAME='woody_woodpacker'
PACKED_NAME='to_change_woody'
GREEN='\033[0;32m'
RED='\033[0;31m'
EOC='\033[0m'
WoodyPackerErrorsDir=couldNotBePacked

function checkFileELF {
	file=$1
	./$NAME $file
	if [ $? -ne 0 ];then
		echo -e "${RED}${file} is not packing correctly!${EOC}"
		mkdir -p $WoodyPackerErrorsDir
		ln -s $file $WoodyPackerErrorsDir/$(basename $file)
	else 
		echo -e "${GREEN}${file} got packed correctly!${EOC}"
		if [[ ! "${blacklist[@]}" =~ "$(basename $file)" ]]; then
			(./$PACKED_NAME) < /dev/null
		fi
	fi
}

function checkFile {
	file=$1

	diff <(cat $file | head -c 4) <(printf '\x7fELF') > /dev/null
	if [ $? -eq 0 ]; then
		echo -e "${GREEN}${file} is ELF${EOC}"
		checkFileELF $file
	else
	#	echo -e "${RED}${file} is not ELF${EOC}"
		return ;
	fi
}

function checkDir {
	dir=$1
	if [ ! "$(ls -A $dir)" ]; then
  		echo "$dir is Empty"
		return
	fi
	for file in $dir/*
	do
		checkFile $file
	done

}

function checkPATH {
	for dir in $(echo $PATH | tr ":" "\n")
	do
		checkDir $dir
	done
}

checkPATH

if [ ! "$(ls -A $WoodyPackerErrorsDir)" ]; then
	echo -e "${GREEN}OK!${EOC}"
	exit 0;
fi

for file in $WoodyPackerErrorsDir/*
	do ./$NAME $file
done

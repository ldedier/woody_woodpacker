#!/bin/bash

source include.sh
source generateBlacklist.sh

WoodyPackerErrorsDir=couldNotBePacked

GENERATE_BLACKLIST=0

if [ ! -f $blacklistFile ] || [ $GENERATE_BLACKLIST -ne 0 ]; then
	generateBlackList
else
	echo "using previous blacklist at $blacklistFile"
fi

source $blacklistFile

function checkFileELFPack {
	file=$1

	./$NAME $file
	if [ $? -ne 0 ];then
		echo -e "${RED}${file} is not packing correctly!${EOC}"
		mkdir -p $WoodyPackerErrorsDir
		ln -s $file $WoodyPackerErrorsDir/$(basename $file)
	else 
		echo -e "${GREEN}${file} got packed correctly!${EOC}"
	fi
}

function checkFileELFExec {
	file=$1

	./$NAME $file
	if [ $? -ne 0 ];then
		echo -e "${RED}${file} is not packing correctly!${EOC}"
	else 
		echo -e "${GREEN}${file} got packed correctly!${EOC}"
		if [[ ! "${blacklist[@]}" =~ "$file" ]] && [[ ! "${blackerlist[@]}" =~ "$(basename $file)" ]]  ; then
			./$PACKED_NAME < /dev/null
			echo "$file: ret = $?" >> RETS
		fi
	fi
}

function testFileELFPacking {

	applyPATH checkFileELFPack

}

function testFileELFExecuting {

	applyPATH checkFileELFExec

}

#testFileELFPacking
testFileELFExecuting

echo "FINIII"

com='
if [ ! "$(ls -A $WoodyPackerErrorsDir)" ]; then
	echo -e "${GREEN}OK!${EOC}"
	exit 0;
fi

for file in $WoodyPackerErrorsDir/*
	do ./$NAME $file
done'

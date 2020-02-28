#!/bin/bash

source include.sh
source generateBlacklist.sh

WoodyPackerErrorsDir=couldNotBePacked
WoodyExecErrorsDir=execFailures
execBlacklist=(/usr/bin/pidof /usr/bin/ssh-agent)

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
		if [[ ! "${blacklist[@]}" =~ "$file" ]] && [[ ! "${blackerlist[@]}" =~ "$(basename $file)" ]] && [[ ! "${execBlacklist[@]}" =~ "$file" ]]   ; then
			(./$PACKED_NAME < /dev/null)
			retValue=$?
			echo "$file: ret = $retValue" >> RETS
			if [ $retValue -ge 100 ]; then
				mkdir -p $WoodyExecErrorsDir
				ln -s $file $WoodyExecErrorsDir/$(basename $file)
			fi
		fi
	fi
}

rm -rf $WoodyExecErrorsDir
rm -rf $WoodyPackerErrorsDir

function testFileELFPacking {

	applyPATH checkFileELFPack

}

function testFileELFExecuting {
	rm -f RETS
	applyPATH checkFileELFExec

}

#testFileELFPacking
testFileELFExecuting

reset
echo "FINIII"

com='
if [ ! "$(ls -A $WoodyPackerErrorsDir)" ]; then
	echo -e "${GREEN}OK!${EOC}"
	exit 0;
fi

for file in $WoodyPackerErrorsDir/*
	do ./$NAME $file
done'

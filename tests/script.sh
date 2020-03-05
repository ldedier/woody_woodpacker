#!/bin/bash

source include.sh
source generateBlacklist.sh

WoodyPackerErrorsDir=couldNotBePacked
WoodyExecErrorsDir=execFailures
execBlacklist=(/usr/bin/pidof /usr/bin/ssh-agent)

retDiffsDir=return_value_diff
outputDiffsDir=stdout_diff

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
			#logDir=$WoodyExecErrorsDir/$(basename $file)
			myOutput=/tmp/myOutput
			trueOutput=/tmp/trueOutput
			$file < /dev/null > $trueOutput
			trueRet=$?
			./$PACKED_NAME < /dev/null > $myOutput
			myRet=$?
			logDir=$WoodyExecErrorsDir/$retDiffsDir
			returnDiffFile=$logDir/$(basename $file)
			if [ $trueRet -ne $myRet ]; then
				mkdir -p $logDir
				echo "############### $file #############" > $returnDiffFile
				echo "" >> $returnDiffFile
				echo "real ret:		$trueRet" >> $returnDiffFile
				echo "$PACKED_NAME ret:	$myRet" >> $returnDiffFile
				if [ $myRet -eq 139 ];then
					chmod 755 $returnDiffFile
				fi
			fi
			diff <(echo "...WOODY..." ; cat $trueOutput | sed s/$(echo -n $file | sed -e 's/\\/\\\\/g; s/\//\\\//g; s/&/\\\&/g')/$PACKED_NAME/g) $myOutput
			if [ $? -ne 0 ]; then
				logDir=$WoodyExecErrorsDir/$outputDiffsDir/$(basename $file)
				mkdir -p $logDir
				cp $myOutput $logDir/myOutput
				cp <(echo "...WOODY..." ; cat $trueOutput | sed s/$(echo -n $file | sed -e 's/\\/\\\\/g; s/\//\\\//g; s/&/\\\&/g')/$PACKED_NAME/g) $logDir/trueOutput
			fi
			rm $myOutput
			rm $trueOutput
		fi
	fi
}

function testFileELFPacking {

	applyPATH checkFileELFPack

}

function testFileELFExecuting {
	rm -f RETS
	rm -rf $WoodyExecErrorsDir
	applyPATH checkFileELFExec

}

rm -rf $WoodyExecErrorsDir
rm -rf $WoodyPackerErrorsDir

make -C $BINDIR

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

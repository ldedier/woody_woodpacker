#!/bin/bash

source include.sh
source generateBlacklist.sh

WoodyPackerErrorsDir=couldNotBePacked
WoodyValgrindErrorsDir=valgrindErrors
WoodyExecErrorsDir=execFailures
execBlacklist=(/usr/bin/pidof /usr/bin/ssh-agent /usr/bin/faked-sysv /usr/bin/faked-tcp )

retDiffsDir=return_value_diff
outputDiffsDir=stdout_diff

GENERATE_BLACKLIST=0

if [ ! -f $blacklistFile ] || [ $GENERATE_BLACKLIST -ne 0 ]; then
	generateBlackList
else
	echo "using previous blacklist at $blacklistFile"
fi

source $blacklistFile

function checkFileELFValgrind {
	file=$1
	valgrind_supps=$2
	error_exit_code=24
	tmp_trace=.__valgrind_tmp_trace__

	echo "valgrind --leak-check=full --error-exitcode=$error_exit_code --log-file=$tmp_trace ./$NAME $file > /dev/null 2>&1"

	valgrind --leak-check=full --error-exitcode=$error_exit_code  --log-file=$tmp_trace ./$NAME $file > /dev/null 2>&1
	res=$?
	if [ $res -ne 0 ] && [ $res -ne 1 ]; then
		mkdir -p $WoodyValgrindErrorsDir
		logdir=$WoodyValgrindErrorsDir/$(basename $file)
		cp $tmp_trace $logdir
		echo -e "${RED}valgrind errors for ${file} logged at ${logdir}${EOC}"
	else
		echo -e "${GREEN}no valgrind errors for ${file}${EOC}"
	fi
}

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

			relevantDiff=1
			diff <($file < /dev/null) <($file < /dev/null)
			if [ $? -ne 0 ]; then
				relevantDiff=0
			fi
			myOutput=/tmp/myOutput
			trueOutput=/tmp/trueOutput
			(exec -a "./$PACKED_NAME" $file < /dev/null > $trueOutput)
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
			if [ $relevantDiff -ne 0 ]; then
				diff <(echo "...WOODY..." ; cat $trueOutput) $myOutput
				if [ $? -ne 0 ]; then
					logDir=$WoodyExecErrorsDir/$outputDiffsDir/$(basename $file)
					mkdir -p $logDir
					cp $myOutput $logDir/myOutput
					cp <(echo "...WOODY..." ; cat $trueOutput) $logDir/trueOutput
				fi
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

function testFileELFValgrind {
	applyPATH checkFileELFValgrind

}

rm -rf $WoodyExecErrorsDir
rm -rf $WoodyPackerErrorsDir
rm -rf $WoodyValgrindErrorsDir
rm -f $PACKED_NAME

make -C $BINDIR

testFileELFPacking
testFileELFExecuting
testFileELFValgrind

rm -f 0 1 test.ttf fonts.scale file2brl.temp
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

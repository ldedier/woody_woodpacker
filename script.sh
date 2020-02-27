#!/bin/bash

blacklist=(alsaloop
alsamixer
alsaucm
aseqdump
aseqnet
baobab
bluetooth-sendto
bluetoothctl
broadwayd
btmgmt
busctl
ctstat
dbus-monitor
driverless
eog
evince
evolution
ex
file-roller
gcm-import
gcm-picker
gcm-viewer
gcr-viewer
gdb
gedit
gnome-calculator
gnome-calendar
gnome-clocks
gnome-contacts
gnome-control-center
gnome-disk-image-mounter
gnome-disks
gnome-font-viewer
gnome-help
gnome-logs
gnome-screenshot
gnome-shell
gnome-shell-extension-prefs
gnome-software
gnome-software-editor
gnome-system-monitor
gnome-terminal
gnome-text-editor
gnome-thumbnail-font
gnome-todo
gnome-www-browser
gold
gpasswd
gs
l2test
"less"
llc-7
lldb
lldb-7
llvm-PerfectShuffle-7
lnstat
lsof
monitor-sensor
mpris-proxy
nautilus
nm-applet
nm-connection-editor
nmtui
nmtui-connect
nmtui-edit
nmtui-hostname
obexctl
pager
pamon
parec
pkcheck
pkmon
pkttyagent
rctest
resize
rhythmbox
rhythmbox-client
rtstat
rview
rvim
scanimage
script
seahorse
shotwell
simple-scan
speaker-test
systemctl
systemd-cgls
systemd-delta
systemd-inhibit
tload
totem
transmission-gtk
vi
view
vim
vim.basic
vim.tiny
vimdiff
vinagre
xinit
yelp)

blackerlist=(calibrate_ppa "[" yes bash)

NAME='woody_woodpacker'
PACKED_NAME='to_change_woody'
GREEN='\033[0;32m'
RED='\033[0;31m'
EOC='\033[0m'
WoodyPackerErrorsDir=couldNotBePacked

rm LOG

function checkIfBlockingBinary {
	if [[ "${blackerlist[@]}" =~ "$(basename $file)" ]]; then
		return
	fi
	binary=$1
	echo $$
	{ $binary 10 < /dev/null ; pkill sleep ; } &
	ps -j
	echo "JOBS:" $(jobs -p)
	echo sleeping 2
	sleep 2
	echo ended sleeping 2
	if [ "$(ps | cut -d ' ' -f 2 | grep $!)" ]; then
		echo $binary >> LOG
		echo "killing pid $!"
		pkill -9 --group $!
		pkill -9 $!
	fi
	ps
}

function checkFileELF {
	file=$1
	
	./$NAME $file
	if [ $? -ne 0 ];then
		echo -e "${RED}${file} is not packing correctly!${EOC}"
		mkdir -p $WoodyPackerErrorsDir
		ln -s $file $WoodyPackerErrorsDir/$(basename $file)
	else 
		echo -e "${GREEN}${file} got packed correctly!${EOC}"
		if [[ ! "${blacklist[@]}" =~ "$(basename $file)" ]] && [[ ! "${blackerlist[@]}" =~ "$(basename $file)" ]]  ; then
			./$PACKED_NAME < /dev/null
		fi
	fi
}

function checkFile {
	file=$1

	diff <(cat $file | head -c 4) <(printf '\x7fELF') > /dev/null
	if [ $? -eq 0 ]; then
		echo -e "${GREEN}${file} is ELF${EOC}"
	#	checkFileELF $file
		checkIfBlockingBinary $file
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

#checkPath "/usr/bin"
#checkPATH

#checkDir "/usr/bin"
checkFile "/bin/sleep"

#checkIfBlockingBinary  "/usr/bin/vim"
#reset
#checkIfBlockingBinary  "/bin/ls"

com='
if [ ! "$(ls -A $WoodyPackerErrorsDir)" ]; then
	echo -e "${GREEN}OK!${EOC}"
	exit 0;
fi

for file in $WoodyPackerErrorsDir/*
	do ./$NAME $file
done'



if [ -z $1 ] ; then
	echo "Usage: $0 exec"
	exit 1
fi

make >/dev/null && ./woody_woodpacker $1 && ./to_change_woody
echo "ret: $?"

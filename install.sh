#!/bin/bash

executable=build/politician
exec_dest=/bin/

if [ -f $executable ]; then
	if ! (cp $executable $exec_dest); then
		echo "Error while copying the executable. Maybe you didn't run this script as root?" 1>&2
		exit 1
	fi
else
	echo "Executable does not exists. Please compile the source code first." 1>&2
	exit 1
fi

echo "Successfully installed!"

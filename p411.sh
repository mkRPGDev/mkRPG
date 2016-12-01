#!/bin/sh

# Get the pygame build for C411 computers
# run as follows:
#   $. p411.sh
# (note the '.' to set env correctly)

CURDIR=$(pwd)
ARCHIVE=pygame-1.9.2-build.tar.gz

if [ ! -d "pygame-1.9.2-build" ];
then

    if [ ! -f $ARCHIVE ];
    then
	echo "Download..."
	wget http://www.dptinfo.ens-cachan.fr/~pjeanmai/$ARCHIVE
    fi

    if [ $? -ne 0 ];
    then
	echo "Download failed."
	exit 1
    fi

    echo -n "Extraction... "
    tar xf $ARCHIVE
    #rm -f $ARCHIVE
    echo "OK"
fi

echo -n "Update PYTHONPATH... "
export PYTHONPATH=${PYTHONPATH}:$CURDIR/pygame-1.9.2-build/lib/python3.4/site-packages/
echo "OK"

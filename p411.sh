#!/bin/sh

# Get the pygame build for C411 computers
# run as follows:
#   $. p411.sh
# (note the '.' to set env correctly)

CURDIR=$(pwd)
ARCHIVE=python-modules.tar.gz # ~ 41MB

if [ ! -d "python-modules" ];
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
export PYTHONPATH=${PYTHONPATH}:$CURDIR/python-modules/lib/python3.4/site-packages/
echo "OK"

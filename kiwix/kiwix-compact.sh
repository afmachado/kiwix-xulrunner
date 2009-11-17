#!/bin/bash

PROFILES_PATH=~/.www.kiwix.org/kiwix/ ;

# Get the path of the xapian-compact install of the system
XAPIAN_COMPACT=`whereis xapian-compact | cut -d" " -f2`

# If no xapian-compact, take a look to the current directory
if [ ! "$XAPIAN_COMPACT" ] ; then
    XAPIAN_COMPACT=`find ./ -type f -name xapian-compact`
fi

# If no result print a message
if [ ! "$XAPIAN_COMPACT" ] || [ ! -f "$XAPIAN_COMPACT" ] ; then
    echo "'xapian-compact' is not installed, you have to install it to use kiwix-compact."
    exit;
fi

# Check if the profile path exists
if [ ! -d $PROFILES_PATH ] ; then
    echo "Unable to find a the kiwix profile directory '$PROFILES_PATH'." ;
fi

# Return the profile list
PROFILES=`cat /home/kelson/.www.kiwix.org/kiwix/profiles.ini | grep "Path=" | sed -e 's/Path=//' | tr '\n' ' '` ;

# Check if Kiwix is currently running
for PROFILE in $PROFILES ; do
    PROFILE_PATH=$PROFILES_PATH$PROFILE ;
    if [ `find $PROFILE_PATH -name "lock"` ] ; then
	echo "It seems that kiwix is currently running, please close it before running kiwix-compact";
	exit 1;
    fi
done

# Compact the indexes
for PROFILE in $PROFILES ; do
    PROFILE_PATH=$PROFILES_PATH$PROFILE ;
    for INDEX in `find $PROFILE_PATH -name "*.index"` ; do
	rm -rf /tmp/.tmpindex
	$XAPIAN_COMPACT $INDEX /tmp/.tmpindex > /dev/null
	if [ "$?" -eq "0" ] ; then
	    rm -rf $INDEX
	    mv /tmp/.tmpindex $INDEX
	    echo "Success by compacting the index $INDEX";
	else
	    echo "Error by compacting the index $INDEX";
	    exit 1 ;
	fi
    done
done

exit 0 ;
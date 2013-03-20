#!/bin/sh

FILESTOSEARCH="-name CMakeLists.txt"
PATTERN='OgreOverlay'

if [ "" != "$1" ]; 
then
  PATTERN=$1
  shift
else
  echo "usage: $0 <search pattern> [file pattern]"
  echo
  echo "search pattern: like argument to grep"
  echo "file pattern: like arguments to find"
  exit 1
fi


if [ "" != "$1" ]; 
then
  FILESTOSEARCH=$1
  shift
fi

echo "Finding \"$PATTERN\" in files matching \"$FILESTOSEARCH\"..."
echo

for f in `find . $FILESTOSEARCH`
do 
if grep $PATTERN $f >/dev/null 2>/dev/null
then
  echo $f 
  echo '-----------------------'
  grep $PATTERN $f
  echo; echo;
fi
done

#!/bin/sh

FILESTOSEARCH="-name CMakeLists.txt"
PATTERN='OgreOverlay'

if [ "" != "$1" ]; 
then
  PATTERN=$1
  shift
else
  echo "usage: $0 <search pattern> [file pattern]"
  echo
  echo "search pattern: like argument to grep"
  echo "file pattern: like arguments to find"
  exit 1
fi


if [ "" != "$1" ]; 
then
  FILESTOSEARCH=$1
  shift
fi

echo "Finding \"$PATTERN\" in files matching \"$FILESTOSEARCH\"..."
echo

for f in `find . $FILESTOSEARCH`
do 
if grep $PATTERN $f >/dev/null 2>/dev/null
then
  echo $f 
  echo '-----------------------'
  grep $PATTERN $f
  echo; echo;
fi
done


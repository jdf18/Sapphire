# /bin/sh

SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

cd $SCRIPTPATH/../bin

if [ -e "sapphire_repl" ]; then
    ./sapphire_repl
else
    cd ../scripts
    ./build.sh
    cd ../bin
    ./sapphire_repl
fi
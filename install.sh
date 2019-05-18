#!/bin/bash

INSTALL_DIR='/root/safe_shutdown'
RUNCMD="$INSTALL_DIR/safe_shutdown.sh > $INSTALL_DIR/safe_shutdown.log 2>&1 &"

if [ $EUID -ne 0 ]; then
  echo "This script must be run as root"
  exit 1
fi

if [ ! -e gpio_wait.c ]; then
  echo 'gpio_wait source not found. Perhaps you did not run the script from its own folder?'
  exit 1
fi

gcc -o gpio_wait gpio_wait.c

if [ ! -e gpio_wait ]; then
  echo 'Compilation failed'
  exit 1
fi

if [ ! -d $INSTALL_DIR ]; then
  mkdir $INSTALL_DIR
fi

cp ./safe_shutdown.sh $INSTALL_DIR
mv ./gpio_wait /usr/sbin

chmod +x $INSTALL_DIR/safe_shutdown.sh

# Add script at startup
if ! grep -q 'safe_shutdown' '/etc/rc.local'; then
sed -i  "/^exit/i$RUNCMD \
" /etc/rc.local
fi

# Kill any previous instance
for PID in $(pgrep safe_shutdown); do
  kill $PID
done

# Start the script!
$RUNCMD &

echo 'Install done'
exit 0

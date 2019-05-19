#!/bin/sh

DELAY=2
PIN=3

if [ ! -d /sys/class/gpio/gpio$PIN ]; then
  echo $PIN >> '/sys/class/gpio/export'
fi

if ! grep -q in /sys/class/gpio/gpio$PIN/direction; then
 echo 'in' >> "/sys/class/gpio/gpio$PIN/direction"
fi

if ! grep -q both /sys/class/gpio/gpio$PIN/edge; then
  echo 'both' >> "/sys/class/gpio/gpio$PIN/edge"
fi

if ! gpio_wait $PIN $DELAY; then
  echo "Failed to poll pin $PIN"
  exit 1
fi

echo 'Shutdown'
shutdown -h now
exit 0

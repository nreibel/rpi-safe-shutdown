# rpi-safe-shutdown
Safe shutdown/wakeup of Raspberry Pi with a configurable delay (hold button for X seconds)

# Installation
Checkout the repository in a folder of your Pi, and :
* cd "your checkout directory"
* chmod +x install.sh
* sudo ./install.sh
* That's it
  
# Configuration
By default, a 2s delay is configured on GPIO3 (physical pin 5). GPIO3 is recommanded because it allows to wake the Pi with the same button. Both delay and pin can be changed in safe_shutdown.sh.

# Advantages
* Allows for a delay to avoid unwanted shutdowns
* Uses interrupts and passive polling instead of active polling

# Improvements
* Set an action for short press ?

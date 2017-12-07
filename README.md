# a64servo
LKM a64-olinuxino software pwm 16 channels driver for servos


usage: Execute the script as root

Load module
```bash
./servo.sh load
```
Unoad module
```bash
./servo.sh unload
```
write channel:
echo CC:VVV, > /dev/servo
where CC = channel, VVV = value : 0 - disabled, 1-999

example: set ch 0 to 356 and ch 5 to 874
```bash
echo 0:356,5:874, > /dev/servo
```
read settings:
```bash
cat /dev/servo
```

_*Work in progress....*_

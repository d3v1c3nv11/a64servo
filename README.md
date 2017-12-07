# a64servo
LKM a64-olinuxino software pwm 16 channels driver for servos

How to build:
```bash
git clone https://github.com/d3v1c3nv11/a64servo.git
cd a64servo
make
```
Note that Olimex images dated after a64olinuxino_ubuntu_16.04.3_20171110.img comes ready for development.


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

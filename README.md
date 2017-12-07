# a64servo

LKM (Loadable Kernel Module) for Olimex A64-OLinuXino 

16-channel (16-pin) software PWM driver for servo motors.

How to get and build:

The procedure should be executed on the A64-OLinuXino board itself. You need to have Internet access 
via the board and then execute in the command line:
```bash
git clone https://github.com/d3v1c3nv11/a64servo.git
cd a64servo
make
```
Note that Olimex images newer than a64olinuxino_ubuntu_16.04.3_20171110.img come with the module already loaded.

General information:

The module enables PWM functionality at processor pins PE0-PE15. These pins are available at one of the rows the GPIO1 pads (pin #5, #7, #9...#33, #35) on the A64-OLinuXino board. Refer to the table at the end. IF YOU ARE STILL NOT SURE WHERE PROCESSOR PINS PE0-PE15 ARE AVAILABLE ON THE BOARD, REFER TO THE SCHEMATIC.

When you load the module, it doesn't change the current configuration of these 16 pins. However, when you use one of the commands from this kernel module, it would automatically changed the pin's direction to output. Even if we stop the PWM available at the specific pin, the direction would remain output. The only way to revert back to the original configuration of the pin before using the PWM function, is to either unload the servo kernel module or to overwrite the pin function using other means (you can set it to input by using the A64 python module, for example).

Usage: 

Execute the script as root (either log as root or use sudo before each command).

Load module
```bash
./servo.sh load
```
Unload module
```bash
./servo.sh unload
```
How to change value of a channel (pin):
echo CC:VVV, > /dev/servo
where CC = channel (processor pin #0-#15; gpio pin 5-35), VVV = value : 0 - disabled, 1-999

example: set ch 0 to 356 and ch 5 to 874
```bash
echo 0:356,5:874, > /dev/servo
```
read settings:
```bash
cat /dev/servo
```

Number # | Processor pin | GPIO1 pad #
:---: | :---: | :---:
0 | PE0 | #5
1 | PE1 | #7
2 | PE2 | #9
3 | PE3 | #11
4 | PE4 | #13
5 | PE5 | #15
6 | PE6 | #17
7 | PE7 | #19
8 | PE8 | #21
9 | PE9 | #23
10 | PE10 | #25
11 | PE11 | #27
12 | PE12 | #29
13 | PE13 | #31
14 | PE14 | #33
15 | PE15 | #35


_*Work in progress....*_

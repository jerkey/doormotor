/*
 * notes on motor controller board http://spaz.org/~jake/pix/2017-01/20170125_023207.jpg
middle chip HIP4082
http://spaz.org/~jake/pix/2017-01/20170125_023259.jpg
pins 2,3,4,7 are BHI, BLI, ALI, AHI
2=blue
3=green
4=yellow
7=white

numbers below refer to motor screw terminals on controller
http://spaz.org/~jake/pix/2017-01/20170125_023751.jpg
blue high   = 2 to + = BHI
green high  = 2 to - = BLI

white high  = 6 to + = AHI
yellow high = 6 to - = ALI

BELOW refers to chip near 1,2,3 motor screw terminals
pins 2,3,4,7 are BHI, BLI, ALI, AHI
2=blue
3=green
4=yellow
7=white

numbers below refer to motor screw terminals on controller
http://spaz.org/~jake/pix/2017-01/20170125_023751.jpg
blue high   = 1 to + = BHI
green high  = 1 to - = BLI

white high  = 3 to + = AHI
yellow high = 3 to - = ALI


 * motor 10-pin header:
 * orange / white = 18 ohm BRAKE winding
 * red = +5v
 * black = ground
 * green and blue = ANALOG quadrature?  shows values between 0-5V depending on position.
*/

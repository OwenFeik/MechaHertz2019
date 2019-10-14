# MechaHertz2019

Repo for team MechaHertz of Melbourne High School Robotics Club. 
Competing in open soccer.

# Discussion

Initial approach to nationals code was to adapt across the code from states in order to use 4 motors, as well as a sensor slave to prevent the stalling caused by time of flight sensors.

This entailed updating the drive library to allow for the new, matrixe based approach to motor driving, as well as the addition of serial code to this file.

The performance of this code was acceptable; the out-on-white detection worked excellently, however the core game loop, as ever, could use optimisation. The primary weakness of our robots for this competition was, of course, the lack of a 360 degree camera, meaning our bots had to wheel about and track the ball. This increased the chance of scoring own goals, which some own goal avoidance code reduced but couldn't eliminate, in addition to reducing the bots' effectiveness as a whole.

In terms of hardware, the most reliable pieces were the Pixy2 and Adafruit_TCS34725 colour sensor, both of which worked reliably and exceptionally. The points of weakness in this regard were primarily themotors; not only are they slower than more expensive models, they additionally suffer from frequent stalling.

In terms of design, our 4-wheeled robot suffered from issues experienced last year, but clearly unresolved; because of minor differences in wheel height, the robot would occasionally fail to place all four wheels on the ground, and thus get stuck in place. The use of jumper wires for the connection of sensors as opposed to PCBs also represented both an additional difficulty with assembly and an ongoing (minor) issue during games as wires would occasionally become unplugged.

Serial communication wasn't used because it turned out that Arduino Unos lack the RAM to run the time of flight and gyro sensors. We instead made do without the use of these sensors, relying initially on the camera alone, and adding between games a colour sensor to perform white-line detection.

# Install

```git clone https://github.com/OwenFeik/MechaHertz2019.git```

Open arduino/arduino.ino and change the pin numbers in the driver declarations at the top of the file.

* Toggle
	Accepts two pins, the two pins which the toggle-switch is connected to. Remember to ground the switch in addition.
* Drive
	Accepts 8 pins, two for each motor. The order is Front Right, Front Left, Back Right, Back Left. You'll have to play around to figure out which way around they go (i.e. 1, 2 vs 2, 1)

If you want to use the sensor slave set up, which is a good idea in theory, but hasn't been tested in practice, also edit the following values, and uncomment the functions ```read_serial()``` and ```handle_slave()``` alongside the call to ```handle_slave()``` at the bottom of arduino.ino. Note that both the slave and master will need to be run on an Arduino Mega: the Arduino Uno has insufficient RAM.

* ```SLAVE_RESET```
	Should be defined as a single digital pin, wired to the reset pin on the sensor slave.

Open slave/slave.ino and change the tof_shutdown_pin array to match your configuration.


# Dependencies
Adafruit Sensors:
* https://github.com/adafruit/Adafruit_Sensor
	(Global Adafruit sensor info library)
* https://github.com/adafruit/Adafruit_TCS34725
	(Colour sensor)
* https://github.com/adafruit/Adafruit_VL53L0X
	(Time of flight sensors)
* https://github.com/adafruit/Adafruit_L3GD20_U
	(Gyroscope)

Pixy2 Camera:
* https://pixycam.com/downloads-pixy2/

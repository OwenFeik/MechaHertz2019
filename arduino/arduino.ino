#include "pixy.h"
#include "drive.h"
#include "toggle.h"
#include "colour.h"

/*
    Initial approach to nationals code was to adapt across the code from states
    in order to use 4 motors, as well as a sensor slave to prevent the stalling
    caused by time of flight sensors.

    This entailed updating the drive library to allow for the new, matrixe based
    approach to motor driving, as well as the addition of serial code to this file.
    
    The performance of this code was acceptable; the out-on-white detection worked
    excellently, however the core game loop, as ever, could use optimisation. The 
    primary weakness of our robots for this competition was, of course, the lack of
    a 360* camera, meaning our bots had to wheel about and track the ball. This
    increased the chance of scoring own goals, which some own goal avoidance code
    reduced but couldn't eliminate, in addition to reducing the bots' effectiveness
    as a whole.

    In terms of hardware, the most reliable pieces were the Pixy2 and Adafruit_TCS34725
    colour sensor, both of which worked reliably and exceptionally. The points of 
    weakness in this regard were primarily the motors; not only are they slower
    than more expensive models, they additionally suffer from frequent stalling.

    In terms of design, our 4-wheeled robot suffered from issues experienced last
    year, but clearly unresolved; because of minor differences in wheel height, the
    robot would occasionally fail to place all four wheels on the ground, and thus
    get stuck in place. The use of jumper wires for the connection of sensors as
    opposed to PCBs also represented both an additional difficulty with assembly
    and an ongoing (minor) issue during games as wires would occasionally become unplugged.
*/


// #define SLAVE_RESET 10 

Toggle toggle = Toggle(52, 53);
Pixy pixy = Pixy();
Drive drive = Drive(7, 6, 2, 3, 9, 8, 5, 4); // BOT RED WIRES (Tapey)
// Drive drive = Drive(2, 3, 7, 6, 8, 9, 4, 5); // BOT BROWN WIRES (Tacky)
Colour colour = Colour();
                          
bool panning = false;
bool panning_dir = false;
int state = 0; // Toggle switch state

bool attacker_colour;

bool goalie_colour;
bool goalie_attacking;
bool goalie_returning;

int c = 0;

// Not in use because uno doesn't work.

// int tof_front, tof_left, tof_right, tof_back;
// int heading;

// char ser_char_in;
// String ser_str_in;
// unsigned long last_slave_report = 0;
// bool restarting_slave = false;


void setup() {
    update_all();

    if (pixy.u_visible) {
        goalie_colour = true;
    }
    else if (pixy.y_visible) {
        goalie_colour = false;
    }

    Serial.begin(115200);

    // digitalWrite(SLAVE_RESET, HIGH);
    // pinMode(SLAVE_RESET, OUTPUT);
}

void loop() {
    update_all();

    // Switch forward -> attacker
    if (state == 1 || goalie_attacking) {
        // Edge detection: If we're about to go out, drive backward.
        if (c == 1) {
            drive.euclid(180, 80);
            delay(300);
            return;
        }

        // Old (working) chase implementation
        if (pixy.visible) {
            // own goal -> blue
            if (goalie_colour) {
                //own goal
                if (pixy.u_visible) {
                    if (pixy.u_x > 50) {
                        drive.euclid(30, 100);
                    }
                    else {
                        drive.euclid(-30, 100);
                    }
                }
                else {
                    if (pixy.y < 35 && pixy.y_visible) {
                        drive.chase(100, (2 * pixy.y_x + pixy.x) / 3);
                    }
                    else {
                        drive.chase(100, pixy.x);
                    }
                }
            }
            // own goal -> yellow
            else {
                //own goal
                if (pixy.y_visible) {
                    if (pixy.u_x > 50) {
                        drive.euclid(60, 100, 0);
                    }
                    else {
                        drive.euclid(100, 60, 0);
                    }
                    
                }
                else {
                    if (pixy.y < 35 && pixy.u_visible) {
                        drive.chase(100, (2 * pixy.u_x + pixy.x) / 3);
                    }
                    else {
                        drive.chase(100, pixy.x);                        
                    }
                }
            }
        }
        else if (pixy.last_seen < 60) {
            if (pixy.x > 80) {
                drive.euclid(0, 0, -55);
            }
            else if (pixy.x < 20) {
                drive.euclid(0, 0, 55);
            }
        }
        else {
            drive.euclid(0, 0, 50);
        }
    
    }
    // Switch back -> goalie
    else if (state == 2) {
        if (c == 1) {
            drive.euclid(180, 80);
            delay(300);
            update_all();
        }

        if (pixy.visible) {
            if (pixy.y <= 60) {
                drive.chase(100, pixy.x);
                goalie_attacking = true;
                // goalie_attacking = true;
            }
            else {
                if (pixy.x > 30 && pixy.x < 70) {
                    drive.face(100, pixy.x);                    
                }
                else if (pixy.x <= 30) {
                    drive.euclid(90, 100);
                }
                else if (pixy.x >= 70) {
                    drive.euclid(270, 100);                                                                                                                                      
                }
            }
        }
        else if (goalie_colour) {
            if (pixy.y_visible) {
                drive.face(100, pixy.y_x);
            }
            else {
                drive.euclid(0, 0, 50);
            }
        }
        else if (!goalie_colour) {
            if (pixy.u_visible) {
                drive.face(100, pixy.u_x);
            }
            else {
                drive.euclid(0, 0, -50);
            }
        }

    }
    else {
        drive.stop();
    }
}

/*
    Serial communication wasn't used because it turned out that Arduino Unos lack the RAM 
    to run the time of flight and gyro sensors. We instead made do without the use of these
    sensors, relying initially on the camera alone, and adding between games a colour sensor
    to perform white-line detection.
*/


// void read_serial() {
//     while (Serial.available() > 0) {
//         ser_char_in = (char) Serial.read();

//         if (isDigit(ser_char_in)) {
//             ser_str_in += (char) ser_char_in;
//         }
//         else {
//             if (ser_char_in == 's') {
//                 last_slave_report = millis();
//             }
//             else if (ser_char_in == 'f') {
//                 tof_front = ser_str_in.toInt();
//             }
//             else if (ser_char_in == 'l') {
//                 tof_left = ser_str_in.toInt();
//             }
//             else if (ser_char_in == 'r') {
//                 tof_right = ser_str_in.toInt();
//             }
//             else if (ser_char_in == 'b') {
//                 tof_back = ser_str_in.toInt();
//             }
//             else if (ser_char_in == 'g') {
//                 heading = ser_str_in.toInt();
//             }

//             ser_str_in = "";
//         }
//     }
// }

// void handle_slave() {
//     read_serial();

//     if (restarting_slave) {
//         if (millis() - last_slave_report > 200) {
//             digitalWrite(SLAVE_RESET, HIGH);
//             restarting_slave = false;
//         }
//     }
//     else if (millis() - last_slave_report > 1000) {
//         digitalWrite(SLAVE_RESET, LOW);
//         last_slave_report = millis();
//         restarting_slave = true;
//     }
// }

void update_all() {
    // handle_slave();
    pixy.update();
    c = colour.getColour();
    state = toggle.getState();
}

#include "pixy.h"
#include "drive.h"
#include "toggle.h"

#define SLAVE_RESET 10 

Toggle toggle = Toggle(24, 25);
Pixy pixy = Pixy();
Drive drive = Drive(2, 3, 4, 5, 6, 7, 8, 9);

bool panning = false;
bool panning_dir = false;
int state = 0; // Toggle switch state

bool attacker_colour;

bool goalie_colour;
bool goalie_attacking;
bool goalie_returning;

int tof_front, tof_left, tof_right, tof_back;
int heading;

char ser_char_in;
String ser_str_in;
unsigned long last_slave_report = 0;
bool restarting_slave = false;


void setup() {
    
    // pixy.update();
    // if (state == 1) {
    //     if (pixy.y_visible) {
    //         goalie_colour = true;
    //     }
    //     else if (pixy.u_visible) {
    //         goalie_colour = false;
    //     }
    // } 
    // else if (state == 2) {
    
    // if (pixy.u_visible) {
    //     goalie_colour = true;
    // }
    // else if (pixy.y_visible) {
    //     goalie_colour = false;
    // }
    // }

    Serial.begin(115200);

    digitalWrite(SLAVE_RESET, HIGH);
    pinMode(SLAVE_RESET, OUTPUT);
}

void loop() {
    update_all();

    // Switch forward -> attacker
    // if (state == 1 || goalie_attacking) {
    if (state == 1) {
    
        // if (pixy.visible && gyro.facingForward(45)) {
        //     panning = false;

        //     if (pixy.y < 50 || (pixy.x > 40 && pixy.x < 60)) {
        //         drive.chase(100, pixy.x);
        //     }
        //     else {
        //         if (pixy.x > 50) {
        //             drive.strafe(100, gyro.heading);
        //         }
        //         else {
        //             drive.strafe(-100, gyro.heading);
        //         }
        //     }
        // }
        // else if (pixy.last_seen < 50 && gyro.facingForward(45)) {
        //     panning = false;

        //     if (pixy.x < 50) {
        //         drive.turn(-80);
        //     }
        //     else if (pixy.x > 50) {
        //         drive.turn(80);
        //     }
        // }
        // else if (gyro.facingForward(20)) {
        //     if (tof.left > 400) {
        //         drive.strafe(-100, gyro.heading);
        //     }
        //     else if (tof.back > 500) {
        //         drive.go(-100);
        //     }
        //     else {
        //         panning = true;
        //     }
        // }
        // else if (panning) {
        //     if (!(gyro.heading < 335 && gyro.heading > 295)) {
        //         if (gyro.heading < 295 && gyro.heading > 135) {
        //             drive.turn(-80);
        //         }
        //         else {
        //             drive.turn(80);
        //         }
        //     }
        // }
        // else {
        //     drive.forward(100, gyro.heading);
        // }
        
        // if (pixy.visible) {
        //     drive.go(100);
        // }

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
                        drive.chase(100, (2 * pixy.y_x + pixy.x) / 3);)
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
                        drive.drive(60, 100, 0);
                    }
                    else {
                        drive.drive(100, 60, 0);
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
                drive.turn(55);
            }
            else if (pixy.x < 20) {
                drive.turn(-55);
            }
        }
        else {
            drive.turn(50);
        }
    
    }
    // Switch back -> goalie
    else if (state == 2) {
    
        if (pixy.visible) {
            // if (goalie_attacking) {
            //     if (pixy.y < 70) {
            //         drive.chase(100, pixy.x);
            //     }
            // }
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
                    drive.strafe(-100);
                }
                else if (pixy.x >= 70) {
                    drive.strafe(100);                                                                                                                                      
                }
            }
        } // true -> opponent yellow
        // else if (goalie_returning) {
        //     if (goalie_colour) {
        //         if (pixy.y_visible) {
        //             if (pixy.y_y > 30) {
        //                 drive.chase(100, pixy.y_x);
        //             }
        //             else {
        //                 goalie_returning = false;
        //             }
        //         }
        //         else {
        //             drive.turn(80);
        //         }
        //     }
        //     else {
        //         if (pixy.u_visible) {
        //             if (pixy.u_y > 30) {
        //                 drive.chase(100, pixy.u_x);
        //             }
        //             else {
        //                 goalie_returning = false;
        //             }
        //         }
        //         else {
        //             drive.turn(-80);
        //         }
        //     }
        // }
        // else if (!pixy.visible) {
        //     if (goalie_attacking) {
        //         goalie_attacking = false;
        //         goalie_returning = true;
        //     }
        // }
        else if (goalie_colour) {
            if (pixy.y_visible) {
                drive.face(100, pixy.y_x);
            }
            else {
                drive.turn(50);
            }
        }
        else if (!goalie_colour) {
            if (pixy.u_visible) {
                drive.face(100, pixy.u_x);
            }
            else {
                drive.turn(-50);
            }
        }




        // panning = false;
        // if (tof.back > 800) {
        //     if (gyro.facingForward(30)) {
        //         drive.go(-100);                    
        //     }
        //     else {
        //         drive.forward(100, gyro.heading);
        //     }

        //     // panning = false;
        // }
        // else if (pixy.visible) {
        //     if (pixy.y <= 30 && pixy.x > 30 && pixy.x < 70) {
        //         drive.chase(100, pixy.x);
        //     }
        //     else {
        //         drive.face(100, pixy.x);
        //     }

        //     // panning = false;
        // }
        // else if (tof.back > 300) {
        //     if (gyro.facingForward(30)) {
        //         drive.go(-80);
        //     }
        //     else {
        //         drive.forward(100, gyro.heading);
        //     }

        //     // panning = false;
        // }
        // else if (pixy.last_seen < 50) {
        //     if (pixy.x < 50) {
        //         drive.turn(-80);
        //     }
        //     else if (pixy.x > 50) {
        //         drive.turn(80);
        //     }

        //     // panning = false;
        // }
        // else if (!panning && !gyro.facingForward(30)) {
        //     drive.forward(100, gyro.heading);
        // }
        // else if (!panning && tof.off_center) {
        //     if (tof.left > tof.right) {
        //         drive.strafe(80, gyro.heading);
        //     }
        //     else if (tof.right > tof.left) {
        //         drive.strafe(-80, gyro.heading);
        //     }
        // }
        // else if (panning) {
        //     if (gyro.heading > 180 && gyro.heading < 300) {
        //         panning_dir = true;
        //     }
        //     else if (gyro.heading < 180 && gyro.heading > 60) {
        //         panning_dir = false;
        //     }

        //     if (panning_dir) {
        //         drive.turn(-50);
        //     }
        //     else {
        //         drive.turn(50);
        //     }
        // }
        // else {
        //     panning = true;
        // }

    */    
    }
    else {
        drive.stop();
    }
}

void read_serial() {
    while (Serial.available() > 0) {
        ser_char_in = (char) Serial.read();

        if (isDigit(ser_char_in)) {
            ser_str_in += (char) ser_char_in;
        }
        else {
            if (ser_char_in == 's') {
                last_slave_report = millis();
            }
            else if (ser_char_in == 'f') {
                tof_front = ser_str_in.toInt();
            }
            else if (ser_char_in == 'l') {
                tof_left = ser_str_in.toInt();
            }
            else if (ser_char_in == 'r') {
                tof_right = ser_str_in.toInt();
            }
            else if (ser_char_in == 'b') {
                tof_back = ser_str_in.toInt();
            }
            else if (ser_char_in == 'g') {
                heading = ser_str_in.toInt();
            }

            ser_str_in = "";
        }
    }
}

void handle_slave() {
    read_serial();

    if (restarting_slave) {
        if (millis() - last_slave_report > 200) {
            digitalWrite(SLAVE_RESET, HIGH);
            restarting_slave = false;
        }
    }
    else if (millis() - last_slave_report > 1000) {
        digitalWrite(SLAVE_RESET, LOW);
        last_slave_report = millis();
        restarting_slave = true;
    }
}

void update_all() {
    handle_slave();
    pixy.update();
    state = toggle.getState();
}

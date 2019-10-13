#include "pixy.h"
#include "drive.h"
#include "toggle.h"
#include "colour.h"

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

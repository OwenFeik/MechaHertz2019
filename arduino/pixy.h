#include <Pixy2.h> // Library from PixyCam manufacturer

class Pixy {
    public:
        Pixy2 pixy; // Pixy driver

        int x, y, width, height, age, last_seen = 0;
        bool visible;

        int u_x, u_y, u_w, u_h, u_age, u_last_seen = 0;
        bool u_visible;

        int y_x, y_y, y_w, y_h, y_age, y_last_seen = 0;
        bool y_visible;

    Pixy(); // Constructur
    void update(); // Update public fields with camera data
};

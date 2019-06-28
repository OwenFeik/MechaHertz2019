class Motor {
    public:
        int pin_1, pin_2;

    Motor(int pin1, int pin2);
    void drive(int speed);
    void stop();
};

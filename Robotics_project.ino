#include <Wire.h>
#include <Servo.h>

const int MPU_ADDR = 0x68;   //I2C adress for the MPU 6050 
Servo myServo; //Initializes the Servo
   

void setup() {
    Wire.begin();
    Serial.begin(9600); //baud rate
    
    myServo.attach(9); //Allocates the information pin for the servo as A9
    myServo.write(90); //Sets the Servo to 90 degrees, (the middle)

    
    Wire.beginTransmission(MPU_ADDR); //MPU_ADDR is 0x68, which is the address to talk to MPU6050 according to Data Sheet
    Wire.write(0x6B);  //accesses power management one register
    Wire.write(0);     //Make sure that the MPU6050 is not in sleep mode
    Wire.endTransmission(true); //Ends communication with MPU6050
}

void loop() {
    Wire.beginTransmission(MPU_ADDR); //MPU_ADDR is 0x68, which is the address to talk to MPU6050 according to Data Sheet
    Wire.write(0x3B);  // Starting with register 0x3B (Acc X high byte)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 6, true);//calls for 6bytes from MPU6050, 2 bytes for each axis acceleration

    int16_t accelX = Wire.read() << 8 | Wire.read(); //collect raw data from MPU 6050 for x axis
    int16_t accelZ = Wire.read() << 8 | Wire.read(); //collect raw data from MPU 6050 for y axis

    float accelX_g = accelX / 16384.0; // turns raw data into the form of g's
    float accelZ_g = accelZ / 16384.0; //turns raw date into the form of g's

    
    float pitch = atan2(accelX_g, accelZ_g) * 180 / PI;//now take the acelleration and plug it into the formula for pitch.
    //formula for pitch is arctan*(aX/aZ) 
    // 180/Pi is trning angl from degrees to radians



    
    Serial.println("Pitch: ");
    Serial.print(pitch);
    myServo.write(pitch); //plugs pitch value into servo

    delay(10);  //10 millisecond delay so servo doesn't start fidgeting

}

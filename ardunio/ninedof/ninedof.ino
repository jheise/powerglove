#include <Adafruit_Sensor.h>
#include <Adafruit_LSM9DS0.h>

/*
* Program to collect and transmit Power Glove sensor data
*/

// Analog Pins
#define pinBend    3

// Digital Pins
#define pinMuxCtl0 10
#define pinMuxCtl1 16
#define pinMuxCtl2 14
#define pinMuxCtl3 15

// i2c pins
//#define pinSDA     2
//#define pinSCL     3

// LSM9DS0
Adafruit_LSM9DS0 lsm = Adafruit_LSM9DS0();

// Store Bend values
int bendT = 0;
int bendI = 0;
int bendM = 0;
int bendR = 0;

// lsm events
sensors_event_t accel, mag, gyro, temp;

// Store Acceleration Values
int accelX = 0;
int accelY = 0;
int accelZ = 0;

int magX = 0;
int magY = 0;
int magZ = 0;

int gyroX = 0;
int gyroY = 0;
int gyroZ = 0;

// Button Data
unsigned int buttons = 0;


// Which finger we're reading
int finger = 1;

void setupSensor()
{
    lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_2G);
    lsm.setupMag(lsm.LSM9DS0_MAGGAIN_2GAUSS);
    lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_245DPS);
}

void setup()
{
    while(!Serial); // wait for serial console
    Serial.begin(9600);
    //start LSM9DS0
    if(!lsm.begin())
    {
      Serial.println("Cannot detect LSM9DS0");
      while(1);
    }
    Serial.println("LSM9DS0 Active");
    setupSensor();

    // enable 
    pinMode(pinMuxCtl0, OUTPUT);
    pinMode(pinMuxCtl1, OUTPUT);
    pinMode(pinMuxCtl2, OUTPUT);
    pinMode(pinMuxCtl3, OUTPUT);

    digitalWrite(pinMuxCtl0, LOW);
    digitalWrite(pinMuxCtl1, LOW);
    digitalWrite(pinMuxCtl2, LOW);
    digitalWrite(pinMuxCtl3, LOW);

    analogReference(DEFAULT);
}

void loop()
{
    digitalWrite(pinMuxCtl0, LOW);
    digitalWrite(pinMuxCtl1, LOW);
    delay(10);
    bendT = analogRead(pinBend);

    digitalWrite(pinMuxCtl0, HIGH);
    digitalWrite(pinMuxCtl1, LOW);
    delay(10);
    bendI = analogRead(pinBend);

    digitalWrite(pinMuxCtl0, LOW);
    digitalWrite(pinMuxCtl1, HIGH);
    delay(10);
    bendM = analogRead(pinBend);

    digitalWrite(pinMuxCtl0, HIGH);
    digitalWrite(pinMuxCtl1, HIGH);
    delay(10);
    bendR = analogRead(pinBend);

    // actuall read data
    lsm.read();

    lsm.getEvent(&accel, &mag, &gyro, &temp);

    // get accel values
    //accelX = (int)lsm.accelData.x;
    //accelY = (int)lsm.accelData.y;
    //accelZ = (int)lsm.accelData.z;

    accelX = (int) lsm.accelData.x;
    accelY = (int) lsm.accelData.y;
    accelZ = (int) lsm.accelData.z;

    // get mag values
    magX = (int) lsm.magData.x;
    magY = (int) lsm.magData.y;
    magZ = (int) lsm.magData.z;

    // get gyro data
    gyroX = (int) lsm.gyroData.x;
    gyroY = (int) lsm.gyroData.y;
    gyroZ = (int) lsm.gyroData.z;

    // write the current state of all sensors to the serial line
    writeState();
    delay(50);
}

/*
* Writes the current sensor state to the serial out line. Format is:
* Thumb - Index - Middle - Ring - Accel X - Accel Y 
* - AccelZ - Mag X - Mag Y - Mag Z - Gyro X - Gyro Y - Gyro Z
*/
void writeState()
{
    // Print output --
    Serial.print(bendT);
    Serial.print("\t");
    Serial.print(bendI);
    Serial.print("\t");
    Serial.print(bendM);
    Serial.print("\t");
    Serial.print(bendR);
    Serial.print("\t");
    Serial.print(accelX);
    Serial.print("\t");
    Serial.print(accelY);
    Serial.print("\t");
    Serial.print(accelZ);
    Serial.print("\t");
    Serial.print(magX);
    Serial.print("\t");
    Serial.print(magY);
    Serial.print("\t");
    Serial.print(magZ);
    Serial.print("\t");
    Serial.print(gyroX);
    Serial.print("\t");
    Serial.print(gyroY);
    Serial.print("\t");
    Serial.println(gyroZ);
}

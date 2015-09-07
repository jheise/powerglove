/*
* Program to collect and transmit Power Glove sensor data
*/

// Analog Pins
#define pinBend 3
#define pinAccelX 2
#define pinAccelY 1
#define pinAccelZ 0

// Digital Pins
#define pinMuxCtl0 10
#define pinMuxCtl1 16
#define pinMuxCtl2 14
#define pinMuxCtl3 15 
#define pinLeft    9
#define pinUp      8
#define pinRight   7
#define pinDown    6
#define pinSelect  5
#define pinStart   4
#define pinB       3
#define pinA       2

// Store Bend values
int bendT = 0;
int bendI = 0;
int bendM = 0;
int bendR = 0;

// Store Acceleration Values
int accelX = 0;
int accelY = 0;
int accelZ = 0;

// Button Data
unsigned int buttons = 0;


// Which finger we're reading
int finger = 1;

void setup()
{
  Serial.begin(9600);
  pinMode(pinMuxCtl0, OUTPUT);
  pinMode(pinMuxCtl1, OUTPUT);
  pinMode(pinMuxCtl2, OUTPUT);
  pinMode(pinMuxCtl3, OUTPUT);
  
  digitalWrite(pinMuxCtl0, LOW);
  digitalWrite(pinMuxCtl1, LOW);
  digitalWrite(pinMuxCtl2, LOW);
  digitalWrite(pinMuxCtl3, LOW);

  pinMode(pinUp, INPUT);
  pinMode(pinDown, INPUT);
  pinMode(pinLeft, INPUT);
  pinMode(pinRight, INPUT);
  pinMode(pinB, INPUT);
  pinMode(pinA, INPUT);
  pinMode(pinSelect, INPUT);
  pinMode(pinStart, INPUT);

  digitalWrite(pinUp, HIGH);
  digitalWrite(pinDown, HIGH);
  digitalWrite(pinLeft, HIGH);
  digitalWrite(pinRight, HIGH);
  digitalWrite(pinB, HIGH);
  digitalWrite(pinA, HIGH);
  digitalWrite(pinSelect, HIGH);
  digitalWrite(pinStart, HIGH);
  
  analogReference(DEFAULT);
}

void loop()
{
  
  // Select and read finger
  //switch(finger)
  //{
  //  case 0:
  
      digitalWrite(pinMuxCtl0, LOW);
      digitalWrite(pinMuxCtl1, LOW);
 
      delay(10);
      bendT = analogRead(pinBend);
    //  break;
    //case 1:
      digitalWrite(pinMuxCtl0, HIGH);
      digitalWrite(pinMuxCtl1, LOW);
     
      delay(10);
      bendI = analogRead(pinBend);
      //break;
    //case 2:
      digitalWrite(pinMuxCtl0, LOW);
      digitalWrite(pinMuxCtl1, HIGH);

      delay(10);
      bendM = analogRead(pinBend);
      //break;
    //case 3:
      digitalWrite(pinMuxCtl0, HIGH);
      digitalWrite(pinMuxCtl1, HIGH);
 
      delay(10);
      bendR = analogRead(pinBend);
      //break;
  //}
  
  //Get Values from accelerometer
  
  accelX = analogRead(pinAccelX);
  accelY = analogRead(pinAccelY);
  accelZ = analogRead(pinAccelZ);

  // Get button values
  buttons = getButtons();
  
  // write the current state of all sensors to the serial line
  writeState();
  delay(50);
}

/*
 * * Returns an integer bitmask of the current digital button state
 * Konami Code Order -- U-D-L-R-B-A-Select-Start
 */

unsigned int getButtons()
{
  return (!digitalRead(pinUp) << 0) |
  (!digitalRead(pinDown) << 1) |
  (!digitalRead(pinLeft) << 2) |
  (!digitalRead(pinRight) << 3) |
  (!digitalRead(pinB) << 4) |
  (!digitalRead(pinA) << 5) |
  (!digitalRead(pinSelect) << 6) |
  (!digitalRead(pinStart) << 7);  
}

/*
* Writes the current sensor state to the serial out line. Format is:
* Dig. Button State - Thumb - Index - Middle - Ring - Accel X - Accel Y - AccelZ
*/
void writeState()
{
  // Print output -- 
  Serial.print(buttons);
  Serial.print("\t");
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
  Serial.println(accelZ);
}

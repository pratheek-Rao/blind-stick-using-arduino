#include "BluetoothSerial.h"

/* Check if Bluetooth configurations are enabled in the SDK */
/* If not, then you have to recompile the SDK */
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run make menuconfig to and enable it
#endif

BluetoothSerial SerialBT;

const int trigPin1 = 5;
const int echoPin1 = 18;
const int trigPin2 = 22;
const int echoPin2 = 23;
const int speakerPin = 27; // Change this to the desired pin for the speaker
const int LDR=36;
const int LED=19;
int  LDRVal;
//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration1, duration2;;
float distanceCm1, distanceCm2;
float distanceInch1, distanceInch2;

void setup() {
  Serial.begin(115200); // Starts the serial communication
  SerialBT.begin();
  pinMode(trigPin1, OUTPUT); // Sets the trigPin1 as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin1 as an Input
  pinMode(trigPin2, OUTPUT); // Sets the trigPin2 as an Output
  pinMode(echoPin2, INPUT); // Sets the echoPin2 as an Input
  pinMode(speakerPin, OUTPUT);
  pinMode(LED, OUTPUT);
}

void loop() {
// Sensor 1
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distanceCm1 = duration1 * SOUND_SPEED / 2;
  
  // Sensor 2
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distanceCm2 = duration2 * SOUND_SPEED / 2;

  // Printing distances
  Serial.println("Distances (cm):");
  Serial.print("Sensor 1: "); Serial.print(distanceCm1); Serial.print("\t");
  Serial.print("Sensor 2: "); Serial.print(distanceCm2); Serial.print("\t");

  if(distanceCm1<100){
  tone(speakerPin, 1500); // Generate a tone of 1000Hz
  delay(500); // Wait for 1 second
  noTone(speakerPin); // Stop the tone
  delay(500); // Wait for 1 second
  if (SerialBT.available())
  {
      SerialBT.println("Obstacle Detected"); // Send "hello" to Bluetooth terminal
      tone(speakerPin, 1500); // Generate a tone of 1000Hz
      delay(500); // Wait for 1 second
      noTone(speakerPin); // Stop the tone
      delay(500); // Wait for 1 second
    }
 }
  else if(distanceCm2>15){
  tone(speakerPin, 1000); // Generate a tone of 1000Hz
  delay(500); // Wait for 1 second
  noTone(speakerPin); // Stop the tone
  delay(500); // Wait for 1 second
  if (SerialBT.available())
  {
      SerialBT.println("Elevation"); // Send "hello" to Bluetooth terminal
      tone(speakerPin, 1000); // Generate a tone of 1000Hz
      delay(500); // Wait for 1 second
      noTone(speakerPin); // Stop the tone
      delay(500); // Wait for 1 second
    }
  }
  else{noTone(speakerPin);}

  LDRVal=analogRead(LDR);
  Serial.println(LDRVal);
  if(LDRVal>2300){
    digitalWrite(LED,HIGH);
    delay(250);
    digitalWrite(LED,LOW);
    delay(250);
  if (SerialBT.available())
  {
      SerialBT.println("Darkness"); 
      digitalWrite(LED,HIGH);
      delay(250);
      digitalWrite(LED,LOW);
      delay(250);
    }
  }
  else{digitalWrite(LED,LOW);}
}

#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <Wire.h>
  long accelX, accelY, accelZ;
  float gForceX, gForceY, gForceZ, pgx, pgy, pgz;
  float rotX, rotY, rotZ;
  long gyroX, gyroY, gyroZ;
SoftwareSerial mySerial(4, 5);
TinyGPS gps;
const uint8_t scl = D6;
const uint8_t sda = D7;
void gpsdump(TinyGPS &gps);
void printFloat(double f, int digits = 2);

void setup()  
{
  // Oploen serial communications and wait for port to open:
  Serial.begin(9600);
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  delay(1000);
  Serial.println("uBlox Neo 6M");
  pinMode(3,OUTPUT);
  Serial.print("Testing TinyGPS library v. "); Serial.println(TinyGPS::library_version());
  Serial.println("by Mikal Hart");
  Serial.println();
  Serial.print("Sizeof(gpsobject) = "); 
  Serial.println(sizeof(TinyGPS));
  Serial.println(); 
  Wire.begin(sda, scl);
  Wire.beginTransmission(0b1101000); //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
  Wire.write(0x6B); //Accessing the register 6B - Power Management (Sec. 4.28)
  Wire.write(0b00000000); //Setting SLEEP register to 0. (Required; see Note on p. 9)
  Wire.endTransmission();  
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1B); //Accessing the register 1B - Gyroscope Configuration (Sec. 4.4) 
  Wire.write(0x00000000); //Setting the gyro to full scale +/- 250deg./s 
  Wire.endTransmission(); 
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1C); //Accessing the register 1C - Acccelerometer Configuration (Sec. 4.5) 
  Wire.write(0b00000000); //Setting the accel to +/- 2g
  Wire.endTransmission(); 
}

void loop() // run over and over
{
  bool newdata = false;
  unsigned long start = millis();
  // Every 5 seconds we print an update
  while (millis() - start < 5000) 
  {
    if (mySerial.available()) 
    
    {
      char c = mySerial.read();
      //Serial.print(c);  // uncomment to see raw GPS data
      if (gps.encode(c)) 
      {
        newdata = true;
        break;  // uncomment to print new data immediately!
      }
    }
  }
  if (sqrt(((rotX * 0.01 )*(rotX * 0.01)) + ((rotY * 0.01)*(rotY * 0.01)) + ((rotZ * 0.01)*(rotZ * 0.01)))  >= 0.08){
      digitalWrite(3,HIGH);
  }
  else {
      digitalWrite(3,LOW);
  }
 velocity(); // Velocity calculation function is calling in the loop;
  
  long lat, lon;
  float flat, flon;
  unsigned long age, date, time, chars;
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned short sentences, failed;

 
  }

void  (double number, int digits)
{
  // Handle negative numbers
  if (number < 0.0) 
  {
     Serial.print('-');
     number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i=0; i<digits; ++i)
    rounding /= 10.0;
  
  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  Serial.print(int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0)
    Serial.print("."); 

  // Extract digits from the remainder one at a time
  while (digits-- > 0) 
  {
    remainder *= 10.0;
    int toPrint = int(remainder);
    Serial.print(toPrint);
    remainder -= toPrint;
  }
}
void velocity(){
    Wire.beginTransmission(0b1101000); //I2C address of the MPU
    Wire.write(0x43); //Starting register for Gyro Readings
    Wire.endTransmission();
    Wire.requestFrom(0b1101000,6); //Request Gyro Registers (43 - 48)
    while(Wire.available() < 6);
    
    //Recording the data
    pgx = gForceX;
    gForceX = accelX / 16384.0*10;
    pgy = gForceY;
    gForceY = accelY / 16384.0*10;
    pgz = gForceZ;
    gForceZ = accelZ / 16384.0*10;
    
    Serial.print("X ");
    Serial.print(pgx-gForceX);
    Serial.print(" | Y ");
    Serial.print(pgy-gForceY);
    Serial.print("| Z ");
    Serial.println(pgz-gForceZ);
   
    byte month, day, hour, minute, second, hundredths;
    int year;
    unsigned long age, date, time, chars;
    long lat, lon;
    float flat, flon;
    int x;
    if (sqrt(((rotX * 0.01)*(rotX * 0.01)) + ((rotY * 0.01)*(rotY * 0.01 )) + ((rotZ * 0.01)*(rotZ * 0.01)))  >= 0.10){
      int ps = second;
      int pm = minute;
      int ph = hour;
      gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
      Serial.print("From "); Serial.print(ph); Serial.print(":"); Serial.print(pm);Serial.print(":"); Serial.print(ps);  Serial.print(" | Till "); Serial.print(static_cast<int>(hour)); Serial.print(":"); Serial.print(static_cast<int>(minute));Serial.print(":"); Serial.print(static_cast<int>(second));
      x = static_cast<int>(second) - ps;
      Serial.print(x);
      Serial.print(" | The Velocity is "); Serial.print( sqrt(((rotX * x /1000)*(rotX * x/1000)) + ((rotY * x/1000)*(rotY * x/1000)) + ((rotZ * x /1000)*(rotZ * x /1000)))); Serial.println("m/2^");      
    }
    else{ 
      gps.f_get_position(&flat, &flon, &age);
      Serial.print("location: "); printFloat(flat, 5); Serial.print(", "); printFloat(flon, 5);
      gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
      Serial.print(" | Date: "); Serial.print(static_cast<int>(month)); Serial.print("/"); Serial.print(static_cast<int>(day)); Serial.print("/"); Serial.print(year); Serial.print(" | Time : "); Serial.print(static_cast<int>(hour+5));  Serial.print(":"); Serial.print(static_cast<int>(minute+30)); Serial.print(":"); Serial.print(static_cast<int>(second));
      Serial.println(" | No");
    }

    
    //Printing data
    Serial.print("Previous value");
    Serial.print("| X ");
    Serial.print(pgx);
    Serial.print("| Y ");
    Serial.print(pgy);
    Serial.print("| Z ");
    Serial.print(pgz);
    Serial.print(" New value");
    Serial.print("| X ");
    Serial.print(gForceX);
    Serial.print("| Y ");
    Serial.print(gForceY);
    Serial.print("| Z ");
    Serial.println(gForceZ);
     
    Wire.beginTransmission(0b1101000); //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
    Wire.write(0x6B); //Accessing the register 6B - Power Management (Sec. 4.28)
    Wire.write(0b00000000); //Setting SLEEP register to 0. (Required; see Note on p. 9)
    Wire.endTransmission();  
    Wire.beginTransmission(0b1101000); //I2C address of the MPU
    Wire.write(0x1B); //Accessing the register 1B - Gyroscope Configuration (Sec. 4.4) 
    Wire.write(0x00000000); //Setting the gyro to full scale +/- 250deg./s 
    Wire.endTransmission(); 
    Wire.beginTransmission(0b1101000); //I2C address of the MPU
    Wire.write(0x1C); //Accessing the register 1C - Acccelerometer Configuration (Sec. 4.5) 
    Wire.write(0b00000000); //Setting the accel to +/- 2g
    Wire.endTransmission(); 
  
  
  
    Wire.beginTransmission(0b1101000); //I2C address of the MPU
    Wire.write(0x3B); //Starting register for Accel Readings
    Wire.endTransmission();
    Wire.requestFrom(0b1101000,6); //Request Accel Registers (3B - 40)
    while(Wire.available() < 6);
    accelX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
    accelY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
    accelZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ

  
    Wire.beginTransmission(0b1101000); //I2C address of the MPU
    Wire.write(0x43); //Starting register for Gyro Readings
    Wire.endTransmission();
    Wire.requestFrom(0b1101000,6); //Request Gyro Registers (43 - 48)
    while(Wire.available() < 6);
    gyroX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
    gyroY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY    gyroZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  
    rotX = gyroX / 131.0;
    rotY = gyroY / 131.0; 
    rotZ = gyroZ / 131.0;
  }

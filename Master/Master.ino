
#include <Servo.h>
#include <SoftwareSerial.h>

#include "Queue.h"

///Decide which side of the robot will this script control
/// Make sure that the Slave script is set to the oppoiste
const bool isRightSide = true;



// Bluetooth
//Software serial limitswhich pins can be used as Recive pins
//Limited to (For The Arduino Mega): 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69
const uint8_t BT_Rx = 51;
//Transmit pin has no such limitations
const uint8_t BT_Tx = 47;

#define BT Serial1

//SoftwareSerial BT(BT_Tx,BT_Rx);

Queue MainQueue;

Servo Thumb;
Servo Index;
Servo Middle;
Servo Ring;
Servo Pinky;
Servo Wrist;
Servo Bicep;
Servo RotBicep;
Servo Shoulder;
Servo RotShoulder;



void setup() {
  Serial.begin(9600);
  Serial.println("Con");
  ServoSetup();
  BT.begin(9600);

}

void ServoSetup()
{
Thumb.attach(2);
Thumb.write(0);  
 Index.attach(3);
 Index.write(0);
 Middle.attach(4);
 Middle.write(0);
 Ring.attach(5);
 Ring.write(0);
 Pinky.attach(6);
 Pinky.write(0);
 Wrist.attach(7);
 Wrist.write(90);
 Bicep.attach(8);
  Bicep.write(0);

 RotBicep.attach(9);
 RotBicep.write(90);
 RotShoulder.attach(10);
 RotShoulder.write(30);
 Shoulder.attach(11);
 Shoulder.write(35);
}


void loop() {
  if (BT.available())  // If there is incoming data from BT, split them into groups of 3 bytes andstore them in the Queue
  {
    
    byte IncomingData[3];
    
    BT.readBytes(IncomingData, 3);
  

    if (IncomingData[0] == 1)  // packet begining with number 1 signalizes the begining of a new block of packets, therefore the Q needs to be cleared and this packet isnt written to it.
    {
      MainQueue.Clear();
        
    } else {
    
      MainQueue.Push(IncomingData);
    }



  } else  // if the BT buffer is empty, start reading queue
  {
   
    ProcessPacket();
    
  }
}

unsigned long ServoDelay = 0;
void ProcessPacket() {
  if(ServoDelay > millis())
  {
    return;
  }


  Packet pck = MainQueue.Get();

  if(pck.data[0]==0)
  {
    return;
  }

  if ((isRightSide == true && pck.data[0] == 76) || (isRightSide == false && pck.data[0] == 82)) {  //checks which side this script controls and which side the packet is for. If they dont match, send it to the other arduino
    //TO ADD: Hardware serial send to other arduino
    return;
  } else  // the sides match or the packet is for a central servo.
  {
    SetServos(pck.data[1],pck.data[2]);

    ServoDelay = millis() + 1000;
  }
}



void SetServos(byte id, byte val) {

  Serial.println(id);
  Serial.println(val);
  Serial.println("");
  switch (id) {
    case 84:
      Thumb.write(val);
    
      break;
    case 73:
      Index.write(val);

      break;
    case 77:
      Middle.write(val);
     
      break;
    case 82:
      Ring.write(val);
    
      break;
    case 80:
      Pinky.write(val);
   
      break;
    case 72:
      Wrist.write(val);
  
      break;
    case 66:
      Bicep.write(val);
  
      break;
    case 90:
      RotBicep.write(val);
  
      break;
    case 83:
      Shoulder.write(val);
 
      break;
    case 88:
      RotShoulder.write(val);
  
      break;
    default:
      Serial.print("Ignored: ");
      Serial.print(id);
      Serial.print("---");
      Serial.println(val);
      break;
  }
  
}

/* Made for using B with SoftwareSerial - Replaced by Serial1
void BT_Setup() {
  pinMode(BT_Rx, INPUT);
  pinMode(BT_Tx, OUTPUT);

  BT.begin(9600);
}*/

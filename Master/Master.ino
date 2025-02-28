
#include <Servo.h>
#include <SoftwareSerial.h>



///Decide which side of the robot will this script control
/// Make sure that the Slave script is set to the oppoiste
const bool isRightSide = true;

//sets the size of the array which hold the packets
//should be set to the amount of servos defined in the control app
const int maxPackets = 16;

// Bluetooth
//Software serial limitswhich pins can be used as Recive pins
//Limited to (For The Arduino Mega): 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69
const uint8_t BT_Rx = 51;
//Transmit pin has no such limitations
const uint8_t BT_Tx = 47;

SoftwareSerial BT(BT_Rx, BT_Tx);

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
 
}

void loop() {
  if(BT.available()>0) // If there is incoming data from BT, split them into groups of 3 bytes andstore them in the Queue
  {
    byte IncomingData[3];
    BT.readBytes(IncomingData,3);

    if(IncomingData[0]==1) // packet begining with number 1 signalizes the begining of a new block of packets, therefore the Q needs to be cleared and this packet isnt written to it.
    {
      MainQueue.Clear();
    }
    else{
      MainQueue.Push(IncomingData);
    }

  

  }
  else // if the BT buffer is empty, start reading queue
  {

  }


}

void ProcessPacket()
{
  Packet pck = MainQueue.Get();
  
  if((isRightSide == true && pck.data[0]==76)||(isRightSide == false && pck.data[0]==82)) //checks which side this script controls and which side the packet is for. If they dont match, send it to the other arduino
    //TO ADD: Hardware serial send to other arduino
    return;
  }
  else // the sides match or the packet is for a central servo.
  {

  }
}


void SetServos(byte id,byte val)
{
  switch(id)
  {
    case 84:
      Thumb.write(val);
    break;
  }
}


void BT_Setup() {
  pinMode(BT_Rx, INPUT);
  pinMode(BT_Tx, OUTPUT);

  BT.begin(9600);
}





struct Packet{
  byte data[3];
};


class Queue {
public:
  //returns the first non empty array of bytes wrapped in a packet Struct
  // The array then gets marked as read/ overwriteable
  Packet Get() {
    for (int i = 0; i < maxPackets; i++) 
    {
      Packet a;
      if (packetBuffer[i][0] != 0) 
      {
          a.data[0] = packetBuffer[i][0];
          a.data[1] = packetBuffer[i][1];
          a.data[2] = packetBuffer[i][2];
      }
      packetBuffer[i][0] =0;
      break;
    }
  }

  void Push(byte packet[3]) {
    for (int i = 0; i < maxPackets; i++) 
    {
      if (packetBuffer[i][0] == 0) 
      {
        for (int j = 0; j < 3; j++) 
        {
          packetBuffer[i][j] = packet[j];
        }
        break;
      }
    }
  }

  //soft-Formats the array, setting the first byte of every sub array to 0
  //any sub array starting with 0 will not be read and will be able to be overwritten
  void Clear() {
    for (int i = 0; i < maxPackets; i++) {
      packetBuffer[i][0] = 0;
    }
  }
private:
   byte packetBuffer[maxPackets][3];
}
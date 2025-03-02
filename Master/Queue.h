

struct Packet{
  byte data[3];
};

class Queue {
public:
//sets the size of the array which hold the packets
//should be set to the amount of servos defined in the control app
  const static int maxPackets = 16;
  //returns the first non empty array of bytes wrapped in a packet Struct
  // The array then gets marked as read/ overwriteable
  Packet Get() {
     Packet a;
     a.data[0] = 0;
    for (int i = 0; i < maxPackets; i++) 
    {
     
      if (packetBuffer[i][0] != 0) 
      {
          a.data[0] = packetBuffer[i][0];
          a.data[1] = packetBuffer[i][1];
          a.data[2] = packetBuffer[i][2];
          packetBuffer[i][0] =0;
          
          break;
      }
      
   
    }
    return a;
    
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
};


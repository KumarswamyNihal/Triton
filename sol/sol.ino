const int P0 = 14;
const int P1 = 14;
const int valve = 13;

const unsigned long BAUD_RATE = 9600;

const byte PACKET_START_BYTE = 0xAA;
const unsigned int PACKET_OVERHEAD_BYTES = 3;
const unsigned int PACKET_MIN_BYTES = PACKET_OVERHEAD_BYTES + 1;
const unsigned int PACKET_MAX_BYTES = 255;

void setup()
{
  pinMode(P0, INPUT);
  pinMode(P1, INPUT);
  pinMode(valve, OUTPUT);

  Serial.begin(BAUD_RATE);
}

boolean sendPacket(unsigned int payloadSize, byte *payload)
{
    // check for max payload size
    unsigned int packetSize = payloadSize + PACKET_OVERHEAD_BYTES;
    if(packetSize > PACKET_MAX_BYTES)
    {
        return false;
    }

    // create the serial packet transmit buffer
    static byte packet[PACKET_MAX_BYTES];

    // populate the overhead fields
    packet[0] = PACKET_START_BYTE;
    packet[1] = packetSize;
    byte checkSum = packet[0] ^ packet[1];

    // populate the packet payload while computing the checksum
    for(unsigned int i = 0; i < payloadSize; i++)
    {
        packet[i + 2] = payload[i];
        checkSum = checkSum ^ packet[i + 2];
    }

    // store the checksum
    packet[packetSize - 1] = checkSum;

    // send the packet
    Serial.write(packet, packetSize);
    Serial.flush();
    return true;
}

boolean validatePacket(unsigned int packetSize, byte *packet)
{
    // check the packet size
    if(packetSize < PACKET_MIN_BYTES || packetSize > PACKET_MAX_BYTES)
    {
        return false;
    }

    // check the start byte
    if(packet[0] != PACKET_START_BYTE)
    {
        return false;
    }

    // check the length byte
    if(packet[1] != packetSize)
    {
        return false;
    }

    // compute the checksum
    byte checksum = 0x00;
    for(unsigned int i = 0; i < packetSize - 1; i++)
    {
        checksum = checksum ^ packet[i];
    }

    // check to see if the computed checksum and packet checksum are equal
    if(packet[packetSize - 1] != checksum)
    {
        return false;
    }

    // all validation checks passed, the packet is valid
    return true;
}
void loop()
{
  boolean isRunning = true;
  boolean valveState = false;

  static byte buffer[PACKET_MAX_BYTES];
  static byte transmit[PACKET_MAX_BYTES];
  unsigned int count = 0;
  unsigned int packetSize = PACKET_MIN_BYTES;

  while(isRunning)
  {
    if(Serial.available())
    {
      byte b = Serial.read();

      if(count == 0 && b == PACKET_START_BYTE)
      {//First byte
        buffer[count] = b;
        count++;
        continue;
        
      }
      else if(count == 0)
      {//Invalid first byte
        continue;
      }
      else if(count == 1)
      {//Byte contains packet length
        buffer[count] = b;

        if(packetSize < PACKET_MIN_BYTES || packetSize > PACKET_MAX_BYTES)
        {
          count =0;
        }
        else
        {
          packetSize = b;
          count++;
        }
        continue;
      }
      else if(count<packetSize)
      {//Keep saving byte
        buffer[count] = b;
        count++;
      }
      if(count >= packetSize)
      {
        if(validatePacket(packetSize, buffer))
        {
          //TODO:: CHANGE VALVE STATE BASED ON Payload
          if(buffer[2] == 'V')
          {
            if(buffer[3] == 'T')
            {//TODO:: SET VALVE TO OPEN
              valveState = true;
            }
            else if(buffer[3] == 'F')
            {//TODO:: SET VALVE TO CLOSE
              valveState = false;
            }
          }
          //Echo back packet
          sendPacket(packetSize - PACKET_OVERHEAD_BYTES, buffer + 2);
        }
        count = 0;
      }
      //TODO:: SEND P0 AND P1 VALUES IN A BUFFER
      transmit[0] = analogRead(P0) & 0xFF;
      transmit[1] = analogRead(P0)>>8;
      transmit[2] = analogRead(P1) & 0xFF;
      transmit[3] = analogRead(P1)>>8;
      sendPacket(4, transmit);
    }
  }
}

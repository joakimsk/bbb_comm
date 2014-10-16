#include <SPI.h>
#include <Ethernet.h>
#include "Settimino.h"

const int pin_d2 = 2;

const int pin_a0 = 0; // Potmeter analog 0 to 255
const int pin_a1 = 1; // Potmeter analog 0 to 255
const int pin_a2 = 2; // Unused analog 0 to 255
const int pin_a3 = 3; // Unused analog 0 to 255
int a0 = -1;
int a1 = -1;
int a2 = -1;
int a3 = -1;


byte mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0x08, 0xE11 };  
IPAddress Local(129,241,154,235); // Local Address
IPAddress PLC(129,241,154,97);   // PLC Address

S7Client Client;
unsigned long Elapsed; // To calc the execution time
//----------------------------------------------------------------------
// Setup : Init Ethernet and Serial port
//----------------------------------------------------------------------
void setup() {
    // Open serial communications and wait for port to open:
    //Serial.begin(9600);
    // while (!Serial) {
   //   ; // wait for serial port to connect. Needed for Leonardo only
    //}
    
    pinMode(pin_a0, INPUT);
    pinMode(pin_a1, INPUT);
    pinMode(pin_a2, INPUT);
    pinMode(pin_a3, INPUT);
    
    pinMode(pin_d2, OUTPUT);

    
    // Start the Ethernet Library
    Ethernet.begin(mac, Local);
    // Setup Time
    delay(500); 
    //Serial.println(Ethernet.localIP());
}
//----------------------------------------------------------------------
// Connects to the PLC
//----------------------------------------------------------------------
bool Connect()
{
    int Result=Client.ConnectTo(PLC, 
                                  0,
                                  2); // Rack and slot mimics CPU315
    //Serial.print("Connecting to ");Serial.println(PLC);  
    if (Result==0) 
    {
    //  Serial.print("Connected ! PDU Length = ");Serial.println(Client.GetPDULength());
    }
    else{
    //  Serial.println("Connection error");
    }
    return Result==0;
}
//----------------------------------------------------------------------
// Dumps a buffer (a very rough routine)
//----------------------------------------------------------------------
void Dump(void *Buffer, int Length)
{
  int i, cnt=0;
  pbyte buf;
  
  if (Buffer!=NULL)
    buf = pbyte(Buffer);
  else  
    buf = pbyte(&PDU.DATA[0]);

  //Serial.print("[ Dumping ");Serial.print(Length);
  //Serial.println(" bytes ]==========================");
  for (i=0; i<Length; i++)
  {
    cnt++;
    //Serial.print("0");
    //Serial.print(buf[i], HEX);
    //Serial.print(" ");
  }  
}
//----------------------------------------------------------------------
// Prints the Error number
//----------------------------------------------------------------------
void CheckError(int ErrNo)
{
  //Serial.print("Error No. 0x");
  //Serial.println(ErrNo, HEX);
  
  // Checks if it's a Severe Error => we need to disconnect
  if (ErrNo & 0x00FF)
  {
    //Serial.println("SEVERE ERROR, disconnecting.");
    Client.Disconnect(); 
  }
}

void loop() 
{
     if(!digitalRead(pin_d2)){
      digitalWrite(pin_d2, HIGH);
    }
  
  int Size, Result;
  void *Target;
  
  Size=4;
  Target = NULL; // Uses the internal Buffer (PDU.DATA[])
  
  // Connection
  while (!Client.Connected)
  {
    if(digitalRead(pin_d2)){
      digitalWrite(pin_d2, LOW);
    }
    if (!Connect()){
      delay(500);
      //Serial.println("Waiting for connection");
    }
  }
  
  
  a0 = map(analogRead(pin_a0), 0, 1023, 0, 255);
  a1 = map(analogRead(pin_a1), 0, 1023, 0, 255);
  a2 = map(analogRead(pin_a2), 0, 1023, 0, 255);
  a3 = map(analogRead(pin_a3), 0, 1023, 0, 255);
  byte DataToWrite[4] = {a0,a1,a2, a3};

  Result=Client.WriteArea(S7AreaDB,1,0,Size,DataToWrite);
  /*
  Serial.println("Writing ");
  Serial.println(DataToWrite[0]);
  Serial.println(DataToWrite[1]);
  Serial.println(DataToWrite[2]);
  Serial.println(DataToWrite[3]);
  */
  if (Result==0){
  }else{
    CheckError(Result);
  } 

   // Serial.print("Reading ");Serial.print(Size);Serial.println(" bytes from DB1"); 
  //Result=Client.ReadArea(S7AreaDB,1,0,Size,Target);
  if (Result==0){
   // Dump(Target, Size);
  }else{
    //CheckError(Result);
  } 
  //Serial.println("");
  delay(50);
}




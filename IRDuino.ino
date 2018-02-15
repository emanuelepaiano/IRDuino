/* 
  *  IRDuino - IR Controller Sketch for Arduino
  *  License: GPL-3.0
  *  2018 - Emanuele Paiano - nixw0rm [at] gmail.com
  *  
  * Sintax serial:
  *  opcode, param1, param2, param3
  *  
  *  
  *  example: 0,7,3772793023,32  -> irSender.send(7, 3772793023, 32)
  *  where 3772793023  is E0E040BF's decimal value  
  *  
  *  param1: IRLib2 IR Protocol (i.e. 7 for NECx Protocol)
  *  param2: Hex command (i.e. E0E040BF for power ON/OFF)
  *  param3: Bits (i.e. 32 for NECx)
  *  
  * ----------------------------------------------------------------------
  *  opcode | param1   |  param2  |  param3  |      azione               
  * ----------------------------------------------------------------------
  *     0   |    X     |    Y     |    Z     |   irSender.send(X, Y, Z)
  * ----------------------------------------------------------------------
  *     1   |    0     |    0     |    0     |   irReceiver.receive()
  * ----------------------------------------------------------------------
  *     1   |    1     |    0     |    0     |   irReceiver.disableIRin()
  * ----------------------------------------------------------------------
  *     1   |    1     |    1     |    0     |   irReceiver.enableIRin()
  * ----------------------------------------------------------------------
  *     1   |    2     |    0     |    0     |   irReceiver.blink13(false)
  * ----------------------------------------------------------------------
  *     1   |    2     |    1     |    0     |   irReceiver.blink13(true)
  * ----------------------------------------------------------------------
  *     1   |    3     |    0     |    0     |   receiver signal as 
  *         |          |          |          |   [hex value,protocol]
  * ----------------------------------------------------------------------
  *     1   |    3     |    1     |    0     |   receiver dumpResult()
  * ----------------------------------------------------------------------
*/


#define LED_ON 1   //Set 0 to disable LED

#define PIN_LED 13 //LED indicator

#define PIN_RCV 2  //receiver Pin

#define DECODER_DETAILED_INFO false

#include <IRLibDecodeBase.h>
#include <IRLibSendBase.h>    //We need the base code
#include <IRLibAll.h>
//#include <IRLib_P01_NEC.h>
//#include <IRLib_P02_Sony.h>
//#include <IRLib_P07_NECx.h>
//#include <IRLib_P09_GICable.h>
//#include <IRLib_P11_RCMM.h>
#include <IRLibRecvPCI.h> 
#include <IRLibCombo.h>
#include <IRLibRecv.h>

bool dumpResultMode=false; //for Debug and Setup

bool isRcvOn=false;

IRdecode irDecoder;

IRsend irSender;
IRrecv irReceiver(PIN_RCV);

void setup() {
  /* Qui ci va la configurazione dei PIN */
  pinMode(PIN_LED, OUTPUT);

  
  
  /* Qui le istruzioni di "startup" */
  Serial.begin(9600);
}

void loop() {

  rcvRoutine();
  
  while (Serial.available())
    {
        String bufferInput=Serial.readStringUntil('\n');
        int delim[3], j=0;        
        for (int i = 0; i < bufferInput.length(); i++) {
          if (bufferInput.substring(i, i+1) == ",") {
            delim[j]=i;
            j++;
            if (j==3) break;
          }
        }
        int opcode=bufferInput.substring(0, delim[0]).toInt();
        long param1=bufferInput.substring(delim[0]+1, delim[1]).toInt();
        long param2=bufferInput.substring(delim[1]+1, delim[2]).toInt();
        long param3=bufferInput.substring(delim[2]+1).toInt();

        /*Serial.print(param1);
        Serial.print(",");
        Serial.print(param2);
        Serial.print(",");
        Serial.println(param3);*/

        cmdDispatch(opcode, param1, param2, param3);
        
    }
}

void rcvRoutine()
{
  if(isRcvOn){
    if (irReceiver.getResults()) { 
      irDecoder.decode();
      if(dumpResultMode)           
        irDecoder.dumpResults(DECODER_DETAILED_INFO);
      else{
        Serial.print(irDecoder.value, HEX);
        Serial.print(",");
        Serial.print(irDecoder.protocolNum);
        Serial.print(",");
        Serial.println(irDecoder.bits);
      }
      irReceiver.enableIRIn();
  }
  }
    
}

/* Lista comandi */
void cmdDispatch(int opcode, long param1, long param2, long param3)
{
 bool success=false;
 
  switch(opcode)
  {
   case 0:
          irSender.send(param1, (uint32_t) param2, param3);
          success=true;
          delay(300);
          break;
   case 1:
          success=recvCmdDispatch(param1, param2);
          break;
   case 2:
          break;
  }

  if (!success) 
    Serial.println("500 ERROR");
  else
    Serial.println("250 OK");
 
}

bool recvCmdDispatch(long param1, long param2)
{
  bool success=false;

  if (param1==0 && param2==0)
  {
    //irReceive.receive()
    success=true;  
  } 
  
  if (param1==1 && param2==0)
  {
    irReceiver.disableIRIn();
    isRcvOn=false;
    success=true;  
  } 
  
  else if (param1==1 && param2==1)
  {
    irReceiver.enableIRIn();
    isRcvOn=true;
    success=true;  
  }


  else if (param1==2 && param2==1)
  {
    irReceiver.blink13(true);
    success=true;  
  }

  else if (param1==2 && param2==0)
  {
    irReceiver.blink13(false);
    success=true;  
  }

   else if (param1==3 && param2==1)
  {
    dumpResultMode=true;
    success=true;  
  }

  else if (param1==3 && param2==0)
  {
    dumpResultMode=false;
    success=true;  
  }

  return success;
}



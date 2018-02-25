# IRDuino 

IRLib2 Arduino Sketch for jIRDuino (https://github.com/emanuelepaiano/jIRDuino)

Based on Cyborg5's library - https://github.com/cyborg5/IRLib2

## Arduino configuration
You should make a simple circuit to send and receive as described on Adafruit Tutorials:
<ul>
 <li>1) Sending IR with Arduino: goo.gl/zcFBpj</li>
 <li>2) Receiving IR with Arduino: https://goo.gl/JsK5Uc</li>
<ul>


<pre>
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
</pre>

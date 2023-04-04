 #include <arduino_utils.h>
 #include <bit_array.h>
 #include <dooyadc.h>
 #include <millis_utils.h>



DCRadio rx;
DCMessage lastCode;


void setup()
{

    Serial.begin(115200);
    rx.init(D2);
    Serial.println("");
    Serial.println("DCReceiveStart");
   
}

void loop()
{

    if (rx.available())
    {
     
        rx.message.debug();
     
      
    }

int a = Serial.read();

while(a > -1 ){
    Serial.print((char)a);
    a = Serial.read();
}

   
}

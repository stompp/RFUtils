
#include <dooyadc.h>

#define RADIO_RX_PIN 2

DCRadio rx;
DCMessage lastMessage;
DCCommandsQueue commandsQueue;


void setup()
{

    Serial.begin(115200);
  
    rx.init(RADIO_RX_PIN);
    rx.setBounce(100);
    commandsQueue.setAddBounce(250);
}

void loop()
{

    if (rx.available())
    {
        
        noInterrupts();
     
        rx.message.debug();
        interrupts();
    }
   
}

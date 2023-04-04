#include <arduino_utils.h>
#include <bit_array.h>
#include <dooyadc.h>
#include <millis_utils.h>

DCRadio rx;
DCMessage lastCode;
// CrossReceiver receiver;
MillisTimer receiveTimer(250);

void setup()
{

    Serial.begin(115200);
    rx.init(2, DCRadio::MODE_FULL_ISR);
    receiveTimer.start();
    Serial.println("DCReceiveStart");

   
}

void loop()
{

    if (rx.available())
    {
        rx.message.debug();
        //     debug("parent");
        // }
    }


    //     // if(receiver.available())
    //     //     receiver.message.debug();
    //     if (CrossReceiver::available())
    //     {
    //         // if (receiveTimer)
    //         // {
    //             CrossReceiver::message.debug();
    //             receiveTimer.start();
    //         // }
    //     }
    //  #endif
}

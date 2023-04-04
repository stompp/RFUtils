
#include <dooyadc.h>
// volatile unsigned long rising_start;
// volatile unsigned long falling_start;

// volatile unsigned long rised_time;
// volatile unsigned long fallen_time;

CrossMessage message;

const uint8_t sendPin = 4;
void setup()
{

    Serial.begin(115200);

    message.setCommand(CrossMessage::P2_CODE);
    message.setAddress(127255255, 1);
}

void loop()
{

    int c = Serial.read();
    // String s = Serial.readString();

    if (c > -1)
    {
        switch (char(c))
        {

        case 'u':
            message.sendCommand(CrossMessage::UP_PRESS_CODE, sendPin);
            message.debug();
            break;
        case 'v':
            message.sendCommand(CrossMessage::UP_RELEASE_CODE, sendPin);
            message.debug();
            break;
        case 'w':
            message.sendCommand(CrossMessage::UP_AND_STOP_CODE, sendPin);
            message.debug();
            break;

        case 'd':
            message.sendCommand(CrossMessage::DOWN_PRESS_CODE, sendPin);
            message.debug();
            break;
        case 'e':
            message.sendCommand(CrossMessage::DOWN_RELEASE_CODE, sendPin);
            message.debug();
            break;
        case 'f':
            message.sendCommand(CrossMessage::DOWN_AND_STOP_CODE, sendPin);
            message.debug();
            break;

        case 'p':
            message.sendCommand(CrossMessage::P2_CODE, sendPin);
            message.debug();
            break;

        case 'm':
            message.sendCommand(CrossMessage::M_CODE, sendPin);
            message.debug();
            break;

        case 'z':
            message.sendCommand(CrossMessage::UP_AND_DOWN_CODE, sendPin);
            message.debug();

            break;
        }
    }

    // {

    // if( c == 's'){
    //     Serial.print("SENDING ");
    //     message.debug(Serial);
    //     message.send(4);
    //     Serial.println("SENT");

    // }
    // }
}

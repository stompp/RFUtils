
#define LINKED_DEVICES_ADDRESS 0

#if defined(ESP8266) || defined(ESP32)

#define USE_ESP_EEPROM
#include <ESP_EEPROM.h>
#define ESP_EEPROM_SIZE 1024
#define DC_INTERRUPT_PIN D1

#elif defined(ARDUINO)

#include <EEPROM.h>
#define DC_INTERRUPT_PIN 2

#endif

#define DC_REPETITION_BOUNCE 100
// #include <bit_array.h>
#include <millis_utils.h>
// #include <arduino_utils.h>
#include <dooyadc.h>

#include <rgb_leds.h>
#include <RGBDimmer.h>



DCCommandsQueue commandsQueue;
DCReceiver rx;

class ScriptEvents : public DCReceiverEvents{
public:
    // responses
    ScriptEvents()
    {
    }
    void firstResponse()
    {

     
       
    }
    void secondResponse()
    {
     
      
      
    }

    void confirmationResponse()
    {
      
        // rx.flush();
    }
    void errorResponse()
    {
      
    }

    // input listeners
    void onCommandReceived(DCMessage m)
    {
        debug("Command received");
        m.debug();


        // if (commandsQueue.addMessage(m))
        // {

        //     switch (linkedModes.getCommandForAddress(m.fullAddress()))
        //     {
        //     case DOOYA_DC::UP_PRESS:
        //         rgbDimmerInputs.brightness(m.command());
        //         break;
        //     case DOOYA_DC::STOP:
        //         rgbDimmerInputs.toneChangerStopChangesStatus(m.command());
        //         break;
        //     case DOOYA_DC::DOWN_PRESS:
        //         rgbDimmerInputs.animations(m.command());
        //         break;
        //     case DOOYA_DC::UP_AND_DOWN:
        //         rgbDimmerInputs.toneStopChangesMode(m.command());
        //         break;
        //     case DOOYA_DC::UP_AND_STOP:
        //         rgbDimmerInputs.hue(m.command());
        //         break;
        //     case DOOYA_DC::DOWN_AND_STOP:
        //         rgbDimmerInputs.temperature(m.command());
        //         break;
        //     }
        // }
   
   
    }

    void onSettingsReceived(DCSettingsBuffer buff)
    {
        // debug("Settings Received");
        // buff.debug();
    }

    // memory events
    DCLinkedAddresses loadLinkedDevices()
    {

        DCLinkedAddresses a;
        int adr = LINKED_DEVICES_ADDRESS;
        EEPROM.get(adr, a);
        // rx.linkedAddresses = a;
        adr += sizeof(DCLinkedAddresses);
        EEPROM.get(adr, linkedModes);
        return a;
    }

    void saveLinkedDevices(DCLinkedAddresses a)
    {
        EEPROM.put(LINKED_DEVICES_ADDRESS, a);
#ifdef USE_ESP_EEPROM
        EEPROM.commit();
#endif
    }

    void saveScriptData()
    {
        int a = LINKED_DEVICES_ADDRESS + sizeof(DCLinkedAddresses);
        EEPROM.put(a, linkedModes);
#ifdef USE_ESP_EEPROM
        EEPROM.commit();
#endif
    }
    // memory events
    void loadData()
    {

        int adr = LINKED_DEVICES_ADDRESS;
        EEPROM.get(adr, rx.linkedAddresses);
        adr += sizeof(DCLinkedAddresses);
        EEPROM.get(adr, linkedModes);
    }

    void saveData()
    {
        int adr = LINKED_DEVICES_ADDRESS;
        EEPROM.put(adr, rx.linkedAddresses);
        adr += sizeof(DCLinkedAddresses);
        EEPROM.put(adr, linkedModes);
#ifdef USE_ESP_EEPROM
        EEPROM.commit();
#endif
    }

    bool onAddressAdded(DCMessage m)
    {

        if (linkedModes.add(m))
        {
            saveScriptData();
            return true;
        }

        else
        {
            rx.removeAddress(m);
        }
        return false;
        // debug("onAddressAdded end");
    }

    void onAddressRemoved(DCMessage m)
    {

        // debug("onAddressRemoved added start");
        if (linkedModes.remove(m))
        {
            saveScriptData();
        }
        // debug("address removed");
    }

    void onAllAddressesRemoved()
    {
        // debug("onAllAddressesRemoved added start");
        linkedModes.clear();
        saveScriptData();
        rgbDimmer.off();
    }
};

ScriptEvents scriptEvents;

// // responses
// void firstResponse()
// {
//     // led.blink(rgbDimmer.getRGB(false).toInt(), 2, 250);
//     led.blink(COLOR_BLUE, 2, 250);
//     // led.blink(rgbDimmer.getRGB().toInt(), 2, 250);
// }
// void secondResponse()
// {
//     // led.blink(COLOR_AIR_FORCE_BLUE_RAF, 2, 250);
//     led.blink(COLOR_VIOLET, 2, 250);
//     // firstResponse();
// }
// void confirmationResponse()
// {
//     led.blink(COLOR_GREEN_COLOR_WHEEL_X11_GREEN, 4, 250);
//     rx.flush();
// }
// void errorResponse()
// {
//     led.blink(COLOR_RED_DEVIL, 4, 125);
// }

// struct DCRGBDimmerInputs
// {

//     int brightness(uint8_t command)
//     {
//         switch (command)
//         {
//         case DOOYA_DC::UP_PRESS:
//             if (slowInputsTimer.check())
//                 rgbDimmer.brightnessUp();
//             break;
//         case DOOYA_DC::STOP:
//             if (buttonBounce.check())
//             {
//                 rgbDimmer.changeStatus();
//             }

//             break;
//         case DOOYA_DC::DOWN_PRESS:
//             if (slowInputsTimer.check())
//                 rgbDimmer.brightnessDown();
//             break;
//         case DOOYA_DC::UP_AND_STOP:
//             if (slowInputsTimer.check())
//                 rgbDimmer.setBrightness(255);
//             break;
//         case DOOYA_DC::DOWN_AND_STOP:
//             if (slowInputsTimer.check())
//                 rgbDimmer.setBrightness(1);
//             break;
//         default:
//             return -1;
//         }

//         return command;
//     }

//     int toneStopChangesMode(uint8_t command)
//     {
//         switch (command)
//         {
//         case DOOYA_DC::UP_PRESS:
//             rgbDimmer.toneUp();
//             break;
//         case DOOYA_DC::STOP:
//             if (buttonBounce.check())
//             {
//                 firstResponse();
//                 rgbDimmer.changeMode();
//                 buttonBounce.start();
//             }
//             break;
//         case DOOYA_DC::DOWN_PRESS:
//             rgbDimmer.toneDown();
//             break;

//         default:
//             return -1;
//         }

//         return command;
//     }

//     int toneChangerStopChangesStatus(uint8_t command)
//     {
//         switch (command)
//         {
//         case DOOYA_DC::UP_PRESS:
//             rgbDimmer.toneUp();
//             break;
//         case DOOYA_DC::UP_AND_DOWN:

//             if (buttonBounce.check())
//             {
//                 firstResponse();
//                 rgbDimmer.changeMode();
//                 buttonBounce.start();
//             }

//             break;
//         case DOOYA_DC::STOP:
//             if (buttonBounce.check())
//             {
//                 rgbDimmer.changeStatus();
//             }
//             break;
//         case DOOYA_DC::DOWN_PRESS:
//             rgbDimmer.toneDown();
//             break;

//         default:
//             return -1;
//         }

//         return command;
//     }
//     int toneStopChangesStatus(uint8_t command)
//     {

//         switch (command)
//         {
//         case DOOYA_DC::UP_PRESS:
//             rgbDimmer.toneUp();
//             break;
//         case DOOYA_DC::STOP:
//             if (buttonBounce.check())
//             {
//                 rgbDimmer.changeStatus();
//             }
//             break;
//         case DOOYA_DC::DOWN_PRESS:
//             rgbDimmer.toneDown();
//             break;

//         default:
//             return -1;
//         }

//         return command;
//     }
//     int hue(uint8_t command)
//     {
//         rgbDimmer.setModeToColor();

//         switch (command)
//         {
//         case DOOYA_DC::UP_PRESS:
//             rgbDimmer.toneUp();
//             break;
//         case DOOYA_DC::STOP:
//             if (buttonBounce.check())
//             {
//                 rgbDimmer.changeStatus();
//             }
//             break;
//         case DOOYA_DC::DOWN_PRESS:
//             rgbDimmer.toneDown();
//             break;

//         default:
//             return -1;
//         }

//         return command;
//     }
//     int temperature(uint8_t command)
//     {
//         rgbDimmer.setModeToTemperature();
//         switch (command)
//         {
//         case DOOYA_DC::UP_PRESS:
//             rgbDimmer.toneUp();
//             break;
//         case DOOYA_DC::STOP:
//             if (buttonBounce.check())
//             {
//                 rgbDimmer.changeStatus();
//             }
//             break;
//         case DOOYA_DC::DOWN_PRESS:
//             rgbDimmer.toneDown();
//             break;

//         default:
//             return -1;
//         }

//         return command;
//     }

//     int animations(uint8_t command)
//     {
//         switch (command)
//         {
//         case DOOYA_DC::UP_PRESS:
//             rgbDimmer.setAnimation(AnimationFunctions::BEATING);
//             break;
//         case DOOYA_DC::STOP:
//             rgbDimmer.setAnimation(NO_ANIMATION);
//             break;
//         case DOOYA_DC::DOWN_PRESS:
//             rgbDimmer.setAnimation(AnimationFunctions::RAINBOW1);
//             break;

//         default:
//             return -1;
//         }

//         return command;
//     }

// } rgbDimmerInputs;

// // input listeners
// void onCommandReceived(DCMessage m)
// {
//     debug("Command received");
//     m.debug();

//     if (commandsQueue.addMessage(m))
//     {

//         switch (linkedModes.getCommandForAddress(m.fullAddress()))
//         {
//         case DOOYA_DC::UP_PRESS:
//             rgbDimmerInputs.brightness(m.command());
//             break;
//         case DOOYA_DC::STOP:
//             rgbDimmerInputs.toneChangerStopChangesStatus(m.command());
//             break;
//         case DOOYA_DC::DOWN_PRESS:
//             rgbDimmerInputs.animations(m.command());
//             break;
//         case DOOYA_DC::UP_AND_DOWN:
//             rgbDimmerInputs.toneStopChangesMode(m.command());
//             break;
//         case DOOYA_DC::UP_AND_STOP:
//             rgbDimmerInputs.hue(m.command());
//             break;
//         case DOOYA_DC::DOWN_AND_STOP:
//             rgbDimmerInputs.temperature(m.command());
//             break;
//         }
//     }
// }

// void onSettingsReceived(DCSettingsBuffer buff)
// {
//     debug("Settings Received");
//     buff.debug();
// }

// // memory events
// DCLinkedAddresses loadLinkedDevices()
// {

//     DCLinkedAddresses a;
//     int adr = LINKED_DEVICES_ADDRESS;
//     EEPROM.get(adr, a);
//     // rx.linkedAddresses = a;
//     adr += sizeof(DCLinkedAddresses);
//     EEPROM.get(adr, linkedModes);
//     return a;
// }

// void saveLinkedDevices(DCLinkedAddresses a)
// {
//     EEPROM.put(LINKED_DEVICES_ADDRESS, a);
// #ifdef USE_ESP_EEPROM
//     EEPROM.commit();
// #endif
// }

// void saveScriptData()
// {
//     int a = LINKED_DEVICES_ADDRESS + sizeof(DCLinkedAddresses);
//     EEPROM.put(a, linkedModes);
// #ifdef USE_ESP_EEPROM
//     EEPROM.commit();
// #endif
// }
// // memory events
// void loadData()
// {

//     int adr = LINKED_DEVICES_ADDRESS;
//     EEPROM.get(adr, rx.linkedAddresses);
//     adr += sizeof(DCLinkedAddresses);
//     EEPROM.get(adr, linkedModes);
// }

// void saveData()
// {
//     int adr = LINKED_DEVICES_ADDRESS;
//     EEPROM.put(adr, rx.linkedAddresses);
//     adr += sizeof(DCLinkedAddresses);
//     EEPROM.put(adr, linkedModes);
// #ifdef USE_ESP_EEPROM
//     EEPROM.commit();
// #endif
// }

// bool onAddressAdded(DCMessage m)
// {

//     if (linkedModes.add(m))
//     {
//         saveScriptData();
//         return true;
//     }

//     else
//     {
//         rx.removeAddress(m);
//     }
//     return false;
//     // debug("onAddressAdded end");
// }

// void onAddressRemoved(DCMessage m)
// {

//     // debug("onAddressRemoved added start");
//     if (linkedModes.remove(m))
//     {
//         saveScriptData();
//     }
//     // debug("address removed");
// }

// void onAllAddressesRemoved()
// {
//     // debug("onAllAddressesRemoved added start");
//     linkedModes.clear();
//     saveScriptData();
//     rgbDimmer.off();
// }

void blink(uint16_t times, uint16_t ms_on, uint16_t ms_off = 0)
{
    uint16_t o = ms_off == 0 ? ms_on : ms_off;
    for (uint8_t n = 0; n < times; n++)
    {
        unsigned long flipMs = millis() + ms_on;
        long prev_c = -1;
        while (millis() < flipMs)
        {
            unsigned long c = rgbDimmer.getRGB(false).toInt();
            if (c != prev_c)
            {
                led.write(c);
            }
        }
        // delay(ms_on);
        led.off();
        delay(o);
    }
}

#define LINKED_DEVICES_ADDRESS 0

#if defined(ESP8266) || defined(ESP32)

#define USE_ESP_EEPROM

#include <ESP_EEPROM.h>
#define ESP_EEPROM_SIZE 1024
#define DC_INTERRUPT_PIN D1
#define LED_PIN D5
#define LED_IS_CC true

#elif defined(ARDUINO)

#include <EEPROM.h>
#define DC_INTERRUPT_PIN 2
#define LED_PIN 3
#define LED_IS_CC true

#endif

#define DC_REPETITION_BOUNCE 100

#include <millis_utils.h>
#include <dooyadc.h>

#include <rgb_leds.h>
#include <Dimmer.h>
using ColorUtils::Dimmer;
Dimmer dimmer;
AnalogLed led(LED_PIN, LED_IS_CC);

MillisTimer slowInputsTimer(100, true, true, true);
MillisTimer buttonBounce(500, true, true, true);

DCLinkedCommandsMemory linkedModes;
DCCommandsQueue commandsQueue;
DCReceiver rx;

class ScriptEvents : public DCReceiverEvents
{
public:
    // responses
    ScriptEvents()
    {
    }
    void firstResponse()
    {
        led.blink(255, 2, 250);
    }
    void secondResponse()
    {

        led.blink(255, 2, 250);
    }

    void confirmationResponse()
    {
        led.blink(255, 4, 250);
        rx.flush();
    }
    void errorResponse()
    {
        led.blink(255, 8, 125);
    }

    struct DCDimmerInputs
    {
        bool pressButtonCheck()
        {
            return (buttonBounce.check() && rx.firstPress());
        }

        bool wakeUp()
        {
            if (pressButtonCheck() && (dimmer.isOff()))
            {
                dimmer.on();
                return true;
            }

            return false;
        }

        void changeStatus()
        {
            if (pressButtonCheck())
            {
                dimmer.changeStatus();
            }
        }
        // bool turnOnFirst()
        // {

        //     if (buttonBounce.check() && rx.msPressed() == 0)
        //     {

        //             bool o = dimmer.isOff();
        //             dimmer.changeStatus();
        //             return
        //     }
        // }
        int brightness(uint8_t command)
        {
            switch (command)
            {
            case DOOYA_DC::UP_PRESS:
                // if (slowInputsTimer.check())
                dimmer.brightnessUp();
                break;
            case DOOYA_DC::STOP:
                changeStatus();

                break;
            case DOOYA_DC::DOWN_PRESS:
                // if (slowInputsTimer.check())
                dimmer.brightnessDown();
                break;
            case DOOYA_DC::UP_AND_STOP:
                // if (slowInputsTimer.check())
                dimmer.maxBrightness();
                break;
            case DOOYA_DC::DOWN_AND_STOP:
                // if (slowInputsTimer.check())
                dimmer.minBrightness();
                break;
            case DOOYA_DC::UP_AND_STOP_AND_DOWN:
                // if (slowInputsTimer.check())
                // rgbDimmer.setSaturation(SATURATION_MAX);
                break;
            default:
                return -1;
            }

            return command;
        }

        // int animations(uint8_t command)
        // {
        //     switch (command)
        //     {
        //     case DOOYA_DC::UP_PRESS:
        //         dimmer.setAnimation(AnimationFunctions::BEATING);
        //         break;
        //     case DOOYA_DC::STOP:
        //         wakeUp();
        //         rgbDimmer.setAnimation(NO_ANIMATION);
        //         break;
        //     case DOOYA_DC::DOWN_PRESS:
        //         rgbDimmer.setAnimation(AnimationFunctions::RAINBOW1);
        //         break;

        //     default:
        //         return -1;
        //     }

        //     return command;
        // }

    } dimmerInputs;

    // input listeners
    void onCommandReceived(DCMessage m)
    {

        if(m.isParent()){
            commandsQueue.clear();
        }
        if (commandsQueue.addMessage(m) || linkedModes.containsChildOf(m))
        {
            dimmerInputs.brightness(m.command());
        //     switch (linkedModes.getCommandForAddress(m.fullAddress()))
        //     {
        //     case DOOYA_DC::UP_PRESS:
        //         dimmerInputs.brightness(m.command());
        //         break;
        //     case DOOYA_DC::STOP:
        //         break;
        //     case DOOYA_DC::DOWN_PRESS:
        //         break;
        //         // case DOOYA_DC::UP_AND_DOWN:
        //         //     rgbDimmerInputs.toneStopChangesMode(m.command());
        //         //     break;
        //         // case DOOYA_DC::UP_AND_STOP:
        //         //     rgbDimmerInputs.hue(m.command());
        //         //     break;
        //         // case DOOYA_DC::DOWN_AND_STOP:
        //         //     rgbDimmerInputs.temperature(m.command());
        //         //     break;
        //     }
        }
    }

    void onSettingsReceived(DCSettingsBuffer buff)
    {
        debug("Settings Received");
        buff.debug();
    }

    // memory events
    DCLinkedAddresses loadLinkedDevices()
    {

        DCLinkedAddresses a;
        int adr = LINKED_DEVICES_ADDRESS;
        EEPROM.get(adr, a);
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
        dimmer.off();
    }
};

ScriptEvents scriptEvents;

// void blink(uint16_t times, uint16_t ms_on, uint16_t ms_off = 0)
// {
//     uint16_t o = ms_off == 0 ? ms_on : ms_off;
//     for (uint8_t n = 0; n < times; n++)
//     {
//         unsigned long flipMs = millis() + ms_on;
//         long prev_c = -1;
//         while (millis() < flipMs)
//         {
//             unsigned long c = rgbDimmer.getRGB(false).toInt();
//             if (c != prev_c)
//             {
//                 led.write(c);
//             }
//         }
//         // delay(ms_on);
//         led.off();
//         delay(o);
//     }
// }

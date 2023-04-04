#define LINKED_DEVICES_ADDRESS 0

#if defined(ESP8266) || defined(ESP32)

#define USE_ESP_EEPROM

#include <ESP_EEPROM.h>
#define ESP_EEPROM_SIZE 1024
#define DC_INTERRUPT_PIN D1
// #define LED1_PIN D6
// #define LED2_PIN D7
// #define LEDS_ARE_CC true

const uint8_t LEDS_PINS[] = {D6,D7};
const bool LEDS_CC[] = {true,true};
#elif defined(ARDUINO)

#include <EEPROM.h>
#define DC_INTERRUPT_PIN 2
// #define LED1_PIN 5
// #define LED2_PIN 6
const uint8_t LEDS_PINS[] = {5,6};
const bool LEDS_CC[] = {true,true};
// #define LEDS_ARE_CC true

#endif

#define DC_REPETITION_BOUNCE 200
#define DC_BUTTON_BOUNCE 400
#include <millis_utils.h>
#include <dooyadc.h>

#include <rgb_leds.h>
#include <Dimmer.h>
using ColorUtils::Dimmer;

#define N_OUTPUTS 2
Dimmer dimmers[N_OUTPUTS];

AnalogLed leds[N_OUTPUTS];
AnalogLedGroup ledGroup(leds, N_OUTPUTS);

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
        ledGroup.off();
        ledGroup.blink(255, 2, 250);
        // bool
        // leds[1].write(0);
        // leds[0].blink(255, 2, 250);
    }
    void secondResponse()
    {

        firstResponse();
    }
    void confirmationResponse()
    {
        firstResponse();
        firstResponse();
        // rx.flush();
    }
    void errorResponse()
    {

        leds[1].write(0);
        leds[0].blink(255, 8, 125);
        // led2.blink(255, 8, 125);
    }

    struct DCDimmerInputs
    {

        MillisTimer buttonBounce;
        DCDimmerInputs()
        {
            buttonBounce.set(DC_BUTTON_BOUNCE, true, true, true);
        }
        bool pressButtonCheck()
        {
            return (buttonBounce.check() && rx.firstPress());
        }

        bool wakeUp(Dimmer &dimmer)
        {
            if (pressButtonCheck() && (dimmer.isOff()))
            {
                dimmer.on();
                return true;
            }

            return false;
        }

        void toggleStatus(Dimmer &dimmer)
        {
            if (pressButtonCheck())
            {
                dimmer.toggleSwitch();
            }
        }

        int brightness(Dimmer &dimmer, uint8_t command)
        {
            switch (command)
            {
            case DOOYA_DC::UP_PRESS:
                // if (slowInputsTimer.check())
                if (dimmer.isOn())
                {
                    dimmer.up();
                }
                else
                    dimmer.toggleSwitch();
                break;
            case DOOYA_DC::STOP:
                toggleStatus(dimmer);

                break;
            case DOOYA_DC::DOWN_PRESS:
                // if (slowInputsTimer.check())
                 if (dimmer.isOn())
                {
                    dimmer.down();
                }
                // else
                //     dimmer.toggleSwitch();
                break;
                
            case DOOYA_DC::UP_AND_STOP:
                // if (slowInputsTimer.check())
                dimmer.setMax();
                break;
            case DOOYA_DC::DOWN_AND_STOP:
                // if (slowInputsTimer.check())
                dimmer.setMin();
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

        int asSwitch(Dimmer &dimmer, uint8_t command)
        {
            switch (command)
            {
            case DOOYA_DC::UP_PRESS:
                // if (slowInputsTimer.check())
                dimmer.on();
                break;
            case DOOYA_DC::STOP:
                dimmer.off();
                break;
            case DOOYA_DC::DOWN_PRESS:
                // if (slowInputsTimer.check())
                dimmer.off();
                break;
            // case DOOYA_DC::UP_AND_STOP:
            //     // if (slowInputsTimer.check())
            //     dimmer.setMax();
            //     break;
            // case DOOYA_DC::DOWN_AND_STOP:
            //     // if (slowInputsTimer.check())
            //     dimmer.setMin();
            //     break;
            // case DOOYA_DC::UP_AND_STOP_AND_DOWN:
            //     // if (slowInputsTimer.check())
            //     // rgbDimmer.setSaturation(SATURATION_MAX);
            //     break;
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
    };

    void parentInput(DCMessage m)
    {
        bool linkedWithDown, linkedWithUp;
        linkedWithDown = linkedModes.containsCommandForParent(DOOYA_DC::DOWN_PRESS, m);
        linkedWithUp = linkedModes.containsCommandForParent(DOOYA_DC::UP_PRESS, m);

        if (linkedModes.containsCommandForParent(DOOYA_DC::STOP, m) || (linkedWithUp && linkedWithDown))
        {
            if ((dimmers[0].getSwitchStatus() != dimmers[1].getSwitchStatus()))
            {
                if (m.isStop())
                {
                    dimmers[0].off();
                    dimmers[1].off();
                }
                else
                {
                    dimmers[0].on();
                    dimmers[1].on();
                }
            }
            else
            {
                dimmerInputs[0].brightness(dimmers[0], m.command());
                dimmerInputs[1].brightness(dimmers[1], m.command());
            }
        }
        else
        {
            if (linkedWithDown)
            {
                dimmerInputs[1].brightness(dimmers[1], m.command());
            }

            if (linkedWithUp)
            {
                dimmerInputs[0].brightness(dimmers[0], m.command());
            }
        }
    }
    DCDimmerInputs dimmerInputs[N_OUTPUTS];
    // input listeners
    void onCommandReceived(DCMessage m)
    {

        if (linkedModes.containsChildOf(m))
        {

            commandsQueue.clear();
            parentInput(m);
        }
        else if (commandsQueue.addMessage(m))
        {
            // dimmerInputs.brightness(m.command());
            switch (linkedModes.getCommandForAddress(m.fullAddress()))
            {
            case DOOYA_DC::UP_PRESS:
                dimmerInputs[0].brightness(dimmers[0], m.command());
                break;
            case DOOYA_DC::STOP:
                if (m.isStop() && (dimmers[0].isOn() != dimmers[1].isOn()))
                {
                    dimmers[0].off();
                    dimmers[1].off();
                    /* code */
                }
                else
                {
                    dimmerInputs[0].brightness(dimmers[0], m.command());
                    dimmerInputs[1].brightness(dimmers[1], m.command());
                }

                break;
            case DOOYA_DC::DOWN_PRESS:
                dimmerInputs[1].brightness(dimmers[1], m.command());
                break;
            case DOOYA_DC::UP_AND_STOP:
                dimmerInputs[0].asSwitch(dimmers[0], m.command());
                break;
            case DOOYA_DC::DOWN_AND_STOP:
                dimmerInputs[1].asSwitch(dimmers[1], m.command());
                break;
            case DOOYA_DC::UP_AND_DOWN:
                dimmerInputs[0].asSwitch(dimmers[0], m.command());
                dimmerInputs[1].asSwitch(dimmers[1], m.command());
                break;
            }
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

        if (!m.isStop() && !m.isCommand(DOOYA_DC::DOWN_PRESS) && !m.isCommand(DOOYA_DC::UP_AND_STOP) && !m.isCommand(DOOYA_DC::UP_AND_DOWN) && !m.isCommand(DOOYA_DC::DOWN_AND_STOP))
        {
            m.setCommand(DOOYA_DC::UP_PRESS);
        }

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
            dimmers[0].off();
            dimmers[1].off();
        }
        // debug("address removed");
    }

    void onAllAddressesRemoved()
    {
        // debug("onAllAddressesRemoved added start");
        linkedModes.clear();
        saveScriptData();
        dimmers[0].off();
        dimmers[1].off();
    }
};


ScriptEvents scriptEvents;


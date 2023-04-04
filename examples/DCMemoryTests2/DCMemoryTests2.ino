#include <bit_array.h>
#include <millis_utils.h>
#include <arduino_utils.h>
#include <dooyadc.h>
#include <EEPROM.h>
#define LINKED_DEVICES_ADDRESS 0

// DCRadio radio;

int redPin = 3;
bool redPinStatus = false;

DCReceiver rx;

DCOnCommands onCommands;
DCCommandsQueue commandsQueue;

void firstResponse()
{
    rblink(redPin, 2, 250);
}
void secondResponse()
{
    rblink(redPin, 2, 250);
    // rblink(redPin, 4, 125);
}
void confirmationResponse()
{
    rblink(redPin, 4, 250);
    rx.flush();
}
void errorResponse()
{
    // rblink(redPin, 4, 125);
}

DCLinkedAddresses loadLinkedDevices()
{
    DCLinkedAddresses a;
    EEPROM.get(LINKED_DEVICES_ADDRESS, a);

    EEPROM.get(LINKED_DEVICES_ADDRESS + sizeof(DCLinkedAddresses), onCommands);
    return a;
}

void saveLinkedDevices(DCLinkedAddresses a)
{
    EEPROM.put(LINKED_DEVICES_ADDRESS, a);
}

void onCommandReceived(DCMessage m)
{
    debug("Command received");
    m.debug();

    // if (m.isCommand(DOOYA_DC::UP_PRESS))
    // {
    //     redPinStatus = false;
    //     digitalWrite(redPin, redPinStatus);
    // }
    // else

    if (commandsQueue.addMessage(m))
    {
        if (m.isCommand(DOOYA_DC::STOP))
        {
            redPinStatus = true;
            digitalWrite(redPin, redPinStatus);
        }
        else if (onCommands.indexOf(m) > -1)
        {
            // redPinStatus = false;
            redPinStatus = !redPinStatus;
            digitalWrite(redPin, redPinStatus);
        }
    }
}

void onSettingsReceived(DCSettingsBuffer buff)
{
    debug("Settings Received");
    buff.debug();
}

void onAddressAdded(DCMessage m)
{
    onCommands.add(m);
    EEPROM.put(LINKED_DEVICES_ADDRESS + sizeof(DCLinkedAddresses), onCommands);
    // debug("addres added");
}

void onAddressRemoved(DCMessage m)
{
    onCommands.remove(m);
    EEPROM.put(LINKED_DEVICES_ADDRESS + sizeof(DCLinkedAddresses), onCommands);
    // debug("address removed");
}

void onAllAddressesRemoved()
{
    onCommands.clear();
    EEPROM.put(LINKED_DEVICES_ADDRESS + sizeof(DCLinkedAddresses), onCommands);
    // debug("Jandemor");
}

void setup()
{

    Serial.begin(115200);

    rx.init(2, DCRadio::MODE_FULL_ISR);
    rx.setBounce(DC_REPETITION_BOUNCE);
    rx.resp.set(&firstResponse, &secondResponse, &confirmationResponse, &errorResponse);
    rx.setOnCommandReceived(&onCommandReceived);
    rx.setOnSettingsReceived(&onSettingsReceived);
    rx.setAddressesMemoryHandlers(&loadLinkedDevices, &saveLinkedDevices);
    rx.setOnAddressEvents(&onAddressAdded, &onAddressRemoved, &onAllAddressesRemoved);

    rx.loadAddressesFromMemory();

    pinMode(redPin, OUTPUT);
    rblink(redPin, 2, 250);

    // rx.listenForDevice();
    rx.doLinking();

    // DCMessage m = rx.getLinkingMessage();
    // if (m.isValid())
    // {
    //     rx.linkedAddresses.add(m.fullAddress());
    //     debug("Linked");
    //     rx.resp.confirmation();
    // }
    // else
    // {
    //     debug("setup error");
    //     rx.resp.error();
    // }
    // debug("init");
}

void loop()
{
  
    if (rx.update())
    {
        // for(int n = 0 ; n < rx.linkedAddresses.items ; n ++){
        //     debug(rx.linkedAddresses.addresses[n].address);
        // }
        // debug("update");
    }

    // if(Serial.read() == 'l'){
    //     for(int n = 0 ; n < rx.linkedAddresses.items; n++){
    //         debug(rx.linkedAddresses.addresses[n].address);
    //     }
    //     debug("adresses");
    // }
}

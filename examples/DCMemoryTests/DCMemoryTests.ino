#define LINKED_DEVICES_ADDRESS 0

#if defined(ESP8266) || defined(ESP32)

#define USE_ESP_EEPROM
#include <ESP_EEPROM.h>

#define ESP_EEPROM_SIZE 1024
#define DC_INTERRUPT_PIN D6
#define RESPONSE_PIN D3

#elif defined(ARDUINO)

#include <EEPROM.h>
#define DC_INTERRUPT_PIN 2
#define RESPONSE_PIN 3

#endif

#define DC_REPETITION_BOUNCE 50
#include <bit_array.h>
#include <millis_utils.h>
#include <arduino_utils.h>
#include <dooyadc.h>

// DCRadio radio;

int responsePin = RESPONSE_PIN;
bool responsePinStatus = true;

DCOnCommands onCommands;
DCCommandsQueue commandsQueue;
DCReceiver rx;

// responses
void firstResponse()
{
    rblink(responsePin, 2, 250);
}
void secondResponse()
{
    rblink(responsePin, 2, 250);
    // rblink(responsePin, 4, 125);
}
void confirmationResponse()
{
    rblink(responsePin, 4, 250);
    rx.flush();
}
void errorResponse()
{
    rblink(responsePin, 4, 125);
}

// input listeners
void onCommandReceived(DCMessage m)
{
    debug("Command received");
    m.debug();

    // if (m.isCommand(DOOYA_DC::UP_PRESS))
    // {
    //     responsePinStatus = false;
    //     digitalWrite(responsePin, responsePinStatus);
    // }
    // else

    if (commandsQueue.addMessage(m))
    {
        debug("onCommandReceived message added");
        if (m.isCommand(DOOYA_DC::STOP))
        {
            responsePinStatus = true;
            digitalWrite(responsePin, responsePinStatus);
        }
        else if (onCommands.indexOf(m) > -1)
        {
            // responsePinStatus = false;
            responsePinStatus = !responsePinStatus;
            digitalWrite(responsePin, responsePinStatus);
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
    // rx.linkedAddresses = a;
    adr += sizeof(DCLinkedAddresses);
    EEPROM.get(adr, onCommands);
    return a;
}

void saveLinkedDevices(DCLinkedAddresses a)
{
    EEPROM.put(LINKED_DEVICES_ADDRESS, a);
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
    EEPROM.get(adr, onCommands);
}

void saveData()
{
    int adr = LINKED_DEVICES_ADDRESS;
    EEPROM.put(adr, rx.linkedAddresses);
    adr += sizeof(DCLinkedAddresses);
    EEPROM.put(adr, onCommands);
#ifdef USE_ESP_EEPROM
    EEPROM.commit();
#endif
}

bool onAddressAdded(DCMessage m)
{

    if (onCommands.add(m))
    {
        int a = LINKED_DEVICES_ADDRESS + sizeof(DCLinkedAddresses);
        EEPROM.put(a, onCommands);
#ifdef USE_ESP_EEPROM
        EEPROM.commit();
#endif
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
    if (onCommands.remove(m))
    {
        EEPROM.put((const int)(LINKED_DEVICES_ADDRESS + sizeof(DCLinkedAddresses)), (const DCOnCommands)onCommands);

#ifdef USE_ESP_EEPROM
        EEPROM.commit();
#endif
    }
    // debug("address removed");
}

void onAllAddressesRemoved()
{
    // debug("onAllAddressesRemoved added start");
    onCommands.clear();
    EEPROM.put((const int)(LINKED_DEVICES_ADDRESS + sizeof(DCLinkedAddresses)), onCommands);
#ifdef USE_ESP_EEPROM
    EEPROM.commit();
#endif
    // debug("Jandemor");
}

void setup()
{

    Serial.begin(74880);
    Serial.println();

    // delay(1500);

#ifdef USE_ESP_EEPROM
    EEPROM.begin(ESP_EEPROM_SIZE);
    debug("ESP_EEPROM began");
#endif

    pinMode(responsePin, OUTPUT);
    firstResponse();

    rx.init(DC_INTERRUPT_PIN, DCRadio::MODE_FULL_ISR);
    rx.setBounce(DC_REPETITION_BOUNCE);
    rx.resp.set(&firstResponse, &secondResponse, &confirmationResponse, &errorResponse);
    rx.setOnCommandReceived(&onCommandReceived);
    rx.setOnSettingsReceived(&onSettingsReceived);
    rx.setAddressesMemoryHandlers(&loadLinkedDevices, &saveLinkedDevices);
    rx.setOnAddressEvents(&onAddressAdded, &onAddressRemoved, &onAllAddressesRemoved);

    rx.loadAddressesFromMemory();
    // debug("setup adresses loaded");
    // debug("setup do linking start");
    rx.doLinking();
    // debug("setup linking done");
}

void loop()
{

    if (rx.update())
    {
    }
}

#include <arduino_utils.h>
#include <bit_array.h> 
#include <dooyadc.h>
#include <millis_utils.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <ArduinoTrace.h>
#include <avr/pgmspace.h>

// static const char MESSAGE_TYPE_KEY[] PROGMEM = "messageType";
// static const char MESSAGE_CONTENT_KEY[] PROGMEM = "messageContent";
// static const char DC_MESSAGE_KEY[] PROGMEM = "dc_message";
// static const char DC_SEND_KEY[] PROGMEM = "dc_send";
// static const char CHUNKS_KEY[] PROGMEM = "chunks";
static const char MESSAGE_TYPE_KEY[] PROGMEM = "messageType";
static const char MESSAGE_CONTENT_KEY[] PROGMEM = "messageContent";
static const char DC_MESSAGE_KEY[] PROGMEM = "dc_message";
static const char DC_SEND_KEY[] PROGMEM = "dc_send";
static const char CHUNKS_KEY[] PROGMEM = "chunks";
#if defined(ESP8266) || defined(ESP32)

#define DC_RX_PIN D2
#define DC_TX_PIN D8
SoftwareSerial bluetooth(D3, D4);
#else 
#define FPSTR(X) (char*)pgm_read_word(&(X))
#define DC_RX_PIN 2
#define DC_TX_PIN 8
SoftwareSerial bluetooth(3, 4);
#endif

#define DEBUG_SERIAL Serial
#define SNIFFER_SERIAL bluetooth

DCRadio rx;
DCMessage lastMessage;
DCCommandsQueue commandsQueue;

StaticJsonDocument<256> doc;
String buff = "";
void setup()
{

    Serial.begin(115200);
    bluetooth.begin(9600);
     bluetooth.setTimeout(100);

    rx.init(DC_RX_PIN);
    rx.setBounce(100);
    commandsQueue.setAddBounce(500);
}

void loop()
{

    if (rx.available())
    {
        noInterrupts();
          DEBUG_SERIAL.println("Rx available");
        lastMessage = rx.message;
        if (commandsQueue.addMessage(lastMessage))
        {

            // lastCode.debug(Serial);
            // lastCode.debug(SNIFFER_SERIAL);

            sendJson(lastMessage);
        }

        interrupts();
    }
    else
    {
        
        parseInput2();
        // buff = "";
        // readStreamLineToString(&SNIFFER_SERIAL, buff);
        // if (buff.length() > 0)
        // {
        //     DEBUG_SERIAL.println("RECEIVED");
        //     DEBUG_SERIAL.println(buff);
        //     parseInput();
        // }
    }
}


void sendJson(DCMessage message)
{

    char temp[12];
    StaticJsonDocument<512> doc;

    doc[FPSTR(MESSAGE_TYPE_KEY)] = "dc_message";

    JsonObject messageContent = doc.createNestedObject(FPSTR(MESSAGE_CONTENT_KEY));

    JsonArray messageContent_chunks = messageContent.createNestedArray("chunks");

    for (int n = 0; n < DOOYA_DC::CHUNKS; n++)
    {
        snprintf(temp, 5, "%#02x", message.data.bytes[n]);
        // snprintf(temp, 5, "%03d", message.data.bytes[n]);
        messageContent_chunks.add(temp);
    }

    snprintf(temp, 11, "%#x", message.address().address);
    messageContent["address"] = temp;

    // snprintf(temp, 11, "%#X", message.address().address);
    messageContent["base"] = message.baseAddress();

    snprintf(temp, 5, "%#02x", message.channel());
    messageContent["channel"] = temp;

    snprintf(temp, 5, "%#02x", message.command());
    messageContent["command"] = temp;
    messageContent["commandString"] = message.getCommandString();

    String output;

    serializeJson(doc, output);
    DEBUG_SERIAL.println("SENT");
    DEBUG_SERIAL.println(output);

    // serializeJson(doc, SNIFFER_SERIAL);
    SNIFFER_SERIAL.println(output);
}

void parseInput()
{
    if (buff.startsWith("{"))
    {

        doc.clear();
        deserializeJson(doc, buff);

        const char *type = doc[FPSTR(MESSAGE_TYPE_KEY)];

        if (strcmp_P(type, DC_SEND_KEY) == 0)
        {
            DCMessage m;
            // DEBUG_SERIAL.println("IS DC_SEND");
            for (uint8_t n = 0; n < DOOYA_DC::CHUNKS; n++)
            {
                m.data.bytes[n] = doc[FPSTR(MESSAGE_CONTENT_KEY)][n];
            }

            //  m.debug(DEBUG_SERIAL);

            noInterrupts();
            m.send(DC_TX_PIN);
            interrupts();
        }
    }
}

void parseInput2()
{

    doc.clear();
    // TRACE();
    DeserializationError err = deserializeJson(doc, SNIFFER_SERIAL);
    // TRACE();

    if (doc.containsKey(FPSTR(MESSAGE_TYPE_KEY)))
    {
        const char *type = doc[FPSTR(MESSAGE_TYPE_KEY)];

        if (strcmp_P(type, DC_SEND_KEY) == 0)
        {
            DCMessage m;
            // DEBUG_SERIAL.println("IS DC_SEND");
            for (uint8_t n = 0; n < DOOYA_DC::CHUNKS; n++)
            {
                m.data.bytes[n] = doc[FPSTR(MESSAGE_CONTENT_KEY)][n];
            }

            m.debug(DEBUG_SERIAL);

            noInterrupts();
            m.send(DC_TX_PIN);
            interrupts();
        }
    }
}
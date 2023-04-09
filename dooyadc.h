#ifndef DOOYADC_H_
#define DOOYADC_H_

#include <arduino_utils.h>
#include <millis_utils.h>
#include <bit_array.h>

#ifdef ARDUINO
#include <avr/pgmspace.h>
#if defined(ESP8266) || defined(ESP32)
// interrupt handlers and related code must be in RAM
// ICACHE_RAM_ATTR is deprecated
#define ISR_ATTR IRAM_ATTR
#else
#define ISR_ATTR
#endif

#elif defined(RPI)
// #include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

#endif

#define DC_ONLY_FULL_ISR 1

#define DOOYA_DC_DEBUG 0

#ifndef DC_SEND_REPEATS
#define DC_SEND_REPEATS 8
#endif

#ifndef DC_SEND_REPEAT_DELAY
#define DC_SEND_REPEAT_DELAY 100UL
#endif

#ifndef DC_REPETITION_BOUNCE
#define DC_REPETITION_BOUNCE 250UL
#endif

#define LINKING_TIMEOUT 10000UL
#define COMMAND_TIMEOUT 3000UL

#ifndef DC_MEMORY_SIZE
#define DC_MEMORY_SIZE 10
#endif

// #define DC_ADDRESS_CHUNKS 4
/**
 * @brief
 *
 */
// const char M_STRING[] PROGMEM = "M";
// const char P2_STRING[] PROGMEM = "P2";
// const char UP_PRESSED_STRING[] PROGMEM = "UP_PRESSED";
// const char UP_RELEASED_STRING[] PROGMEM = "UP_RELEASED";
// const char STOP_STRING[] PROGMEM = "STOP";
// const char DOWN_PRESSED_STRING[] PROGMEM = "DOWN_PRESSED";
// const char DOWN_RELEASED_STRING[] PROGMEM = "DOWN_RELEASED";
// const char UP_AND_STOP_STRING[] PROGMEM = "UP_AND_STOP";
// const char DOWN_AND_STOP_STRING[] PROGMEM = "DOWN_AND_STOP";
// const char UP_AND_DOWN_STRING[] PROGMEM = "UP_AND_DOWN";
// const char UP_AND_STOP_AND_DOWN_STRING[] PROGMEM = "UP_AND_STOP_AND_DOWN";
// const char HEARTBEAT_STRING[] PROGMEM = "HEARTBEAT";
// const char WIND_ALARM_STRING[] PROGMEM = "WIND_ALARM";
// const char SENSOR_DOWN_STRING[] PROGMEM = "SENSOR_DOWN";
// const char SENSOR_UP_STRING[] PROGMEM = "SENSOR_UP";
// const char UNKNOWN_STRING[] PROGMEM = "UNKNOWN";

const char M_STRING[] = "M";
const char P2_STRING[] = "P2";
const char UP_PRESSED_STRING[] = "UP_PRESSED";
const char UP_RELEASED_STRING[] = "UP_RELEASED";
const char STOP_STRING[] = "STOP";
const char DOWN_PRESSED_STRING[] = "DOWN_PRESSED";
const char DOWN_RELEASED_STRING[] = "DOWN_RELEASED";
const char UP_AND_STOP_STRING[] = "UP_AND_STOP";
const char DOWN_AND_STOP_STRING[] = "DOWN_AND_STOP";
const char UP_AND_DOWN_STRING[] = "UP_AND_DOWN";
const char UP_AND_STOP_AND_DOWN_STRING[] = "UP_AND_STOP_AND_DOWN";
const char HEARTBEAT_STRING[] = "HEARTBEAT";
const char WIND_ALARM_STRING[] = "WIND_ALARM";
const char SENSOR_DOWN_STRING[] = "SENSOR_DOWN";
const char SENSOR_UP_STRING[] = "SENSOR_UP";
const char UNKNOWN_STRING[] = "UNKNOWN";

struct DOOYA_DC
{

    // Bits in message
    static const uint8_t N_BITS = 40;
    // Bytes in message
    static const uint8_t CHUNKS = 5;
    // Bytes in address
    static const uint8_t ADDRESS_CHUNKS = 4;
    /**
     * @brief DC Remote protocol
     * Message starts with a long high pulse about 4750 us length
     * Then low for about 1500 us
     * Then 40 bits
     * Message is 4 bytes address, 1 byte command
     * The 4 most significant bits of the forth address is byte the channel
     * ¿Is the encoding is big endian?
     * ¿Is the base address 3,5 bytes length?
     *
     */

    // [4750, 4810] micros
    static const uint16_t SYNC_HIGH_WIDTH = 4750;
    //[1500,1600] micros
    static const uint16_t SYNC_LOW_WIDTH = 1500;

    /**
     * Short 353.7464788732394
     * Long 714.6938775510204
     * Period 1098.1538461538462
     *
     */

    // TODO Test

    // // Pulse width for low symbol
    // static const uint16_t LOW_WIDTH = 350;
    // // Pulse width for high symbol
    // static const uint16_t HIGH_WIDTH = 700;

    // // Symbol period, maybe just 1000 is fine
    // static const uint16_t SYMBOL_PERIOD = 1100;

    // // Threshold to determine if it's high or low
    // static const uint16_t WIDTH_THRESHOLD = 400;

    // // Timeout for symbols errors
    // static const uint16_t SYMBOL_TIMEOUT = 1500;

    // // Sync Pulse Raised min duration
    // static const uint16_t SYNC_PULSE_RAISED_MIN = 4750;

    // // Sync Pulse Raised max duration
    // static const uint16_t SYNC_PULSE_RAISED_MAX = 4810;

    // // Sync Pulse Fallen min duration
    // static const uint16_t SYNC_PULSE_FALLEN_MIN = 1500;

    // // Sync Pulse Fallen max duration
    // static const uint16_t SYNC_PULSE_FALLEN_MAX = 1600;

    // Pulse width for low symbol
    static const uint16_t LOW_WIDTH = 345;
    // Pulse width for high symbol 480
    static const uint16_t HIGH_WIDTH = 700;

    // Symbol period, maybe just 1000 is fine REAL 1020
    static const uint16_t SYMBOL_PERIOD = 1020;

    // Threshold to determine if it's high or low 400 500
    static const uint16_t WIDTH_THRESHOLD = 600;

    // Timeout for symbols errors 1200 1500
    static const uint16_t SYMBOL_TIMEOUT = 1200;

    // Sync Pulse Raised min duration 4000
    static const uint16_t SYNC_PULSE_RAISED_MIN = 4000;

    // Sync Pulse Raised max duration 4900
    static const uint16_t SYNC_PULSE_RAISED_MAX = 4900;

    // Sync Pulse Fallen min  900
    static const uint16_t SYNC_PULSE_FALLEN_MIN = 900;

    // Sync Pulse Fallen max duration 1600
    static const uint16_t SYNC_PULSE_FALLEN_MAX = 1600;

    // REMOTE COMMAND CODES
    static const uint8_t M = 0b10000011;
    static const uint8_t P2 = 0b00110011;
    static const uint8_t STOP = 0b10101010;
    static const uint8_t UP_PRESS = 0b10001000;
    static const uint8_t UP_RELEASE = 0b1111000;
    static const uint8_t DOWN_PRESS = 0b11001100;
    static const uint8_t DOWN_RELEASE = 0b00111100;
    static const uint8_t UP_AND_STOP = 0b00000001;
    static const uint8_t DOWN_AND_STOP = 0b10000001;
    static const uint8_t UP_AND_DOWN = 0b10011110;
    static const uint8_t UP_AND_STOP_AND_DOWN = 0b01000001;
    static const uint8_t HEARTBEAT = 0b10001001;
    static const uint8_t WIND_ALARM = 0b11101001;
    static const uint8_t SENSOR_UP = 0b10101001;
    static const uint8_t SENSOR_DOWN = 0b11001001;

    static const unsigned long ADDRESS_MAX = 0X0fffffff;

    static bool isP2(uint8_t &code)
    {
        return code == P2 ? true : false;
    }

    static bool isAction(uint8_t &code)
    {
        return ((code != M) && (code != P2));
    }
    // #ifdef RPI
    static const char *getCommandString(uint8_t command)
    {
        switch (command)
        {
        case DOOYA_DC::M:
            return M_STRING;
        case DOOYA_DC::P2:
            return P2_STRING;
        case DOOYA_DC::UP_PRESS:
            return UP_PRESSED_STRING;
        case DOOYA_DC::UP_RELEASE:
            return UP_RELEASED_STRING;
        case DOOYA_DC::STOP:
            return STOP_STRING;
        case DOOYA_DC::DOWN_PRESS:
            return DOWN_PRESSED_STRING;
        case DOOYA_DC::DOWN_RELEASE:
            return DOWN_RELEASED_STRING;
        case DOOYA_DC::UP_AND_STOP:
            return UP_AND_STOP_STRING;
        case DOOYA_DC::DOWN_AND_STOP:
            return DOWN_AND_STOP_STRING;
        case DOOYA_DC::UP_AND_DOWN:
            return UP_AND_DOWN_STRING;
        case DOOYA_DC::UP_AND_STOP_AND_DOWN:
            return UP_AND_STOP_AND_DOWN_STRING;
        case DOOYA_DC::HEARTBEAT:
            return HEARTBEAT_STRING;
        case DOOYA_DC::WIND_ALARM:
            return WIND_ALARM_STRING;
        case DOOYA_DC::SENSOR_DOWN:
            return SENSOR_DOWN_STRING;
        case DOOYA_DC::SENSOR_UP:
            return SENSOR_UP_STRING;

        default:

            break;
        }
        return UNKNOWN_STRING;
    }
};

#ifdef RPI
#define DC_N_BASE_COMMANDS 15
struct DCBaseCommand
{

    uint8_t cmnd;
    const char *str;
};

const DCBaseCommand DC_BASE_COMMANDS_DATA[DC_N_BASE_COMMANDS] = {

    {DOOYA_DC::M, M_STRING},
    {DOOYA_DC::P2, P2_STRING},
    {DOOYA_DC::UP_PRESS, UP_PRESSED_STRING},
    {DOOYA_DC::UP_RELEASE, UP_RELEASED_STRING},
    {DOOYA_DC::STOP, STOP_STRING},
    {DOOYA_DC::DOWN_PRESS, DOWN_PRESSED_STRING},
    {DOOYA_DC::DOWN_RELEASE, DOWN_RELEASED_STRING},
    {DOOYA_DC::UP_AND_STOP, UP_AND_STOP_STRING},
    {DOOYA_DC::DOWN_AND_STOP, DOWN_AND_STOP_STRING},
    {DOOYA_DC::UP_AND_DOWN, UP_AND_DOWN_STRING},
    {DOOYA_DC::UP_AND_STOP_AND_DOWN, UP_AND_STOP_AND_DOWN_STRING},
    {DOOYA_DC::HEARTBEAT, HEARTBEAT_STRING},
    {DOOYA_DC::WIND_ALARM, WIND_ALARM_STRING},
    {DOOYA_DC::SENSOR_DOWN, SENSOR_DOWN_STRING},
    {DOOYA_DC::SENSOR_UP, SENSOR_UP_STRING}};

uint8_t getDCCommandForString(const char *s)
{
    for (uint8_t n = 0; n < DC_N_BASE_COMMANDS; n++)
    {
        if (strcmp(s, DC_BASE_COMMANDS_DATA[n].str) == 0)
        {

            return DC_BASE_COMMANDS_DATA[n].cmnd;
        }
    }
    return 0;
}

const char *getDCCommandStringForCommand(uint8_t c)
{

    for (uint8_t n = 0; n < DC_N_BASE_COMMANDS; n++)
    {
        if (DC_BASE_COMMANDS_DATA[n].cmnd == c)
            return DC_BASE_COMMANDS_DATA[n].str;
    }
    return UNKNOWN_STRING;
}

#endif

//     static const char *getCommandString(uint8_t command)
//     {
//         strcpy_P(commandBuff, (char *)pgm_read_word(&(getCommandPnt(command))));
//         return commandBuff;
//     }
// };
// #endif

union DCAddress
{
    unsigned long address;
    uint8_t bytes[DOOYA_DC::ADDRESS_CHUNKS];

    DCAddress()
    {
        address = 0;
    }
    DCAddress(unsigned long _address)
    {
        set(_address);
    }

    DCAddress(uint8_t _bytes[])
    {
        set(_bytes);
    }

    void clear()
    {
        address = 0;
    }
    void set(unsigned long _address)
    {
        address = _address;
    }

    void set(uint8_t _bytes[])
    {
        for (uint8_t n = 0; n < DOOYA_DC::ADDRESS_CHUNKS; n++)
        {
            bytes[n] = _bytes[n];
        }
    }

    /**  _channel [0,15] **/
    void setChannel(uint8_t _channel)
    {
        uint8_t c = bit_endian_swap(_channel);
        bytes[3] &= 0x0f;
        bytes[3] |= c;
    }
    /** _address [0,268435455] _channel [0,15] **/
    void setAddress(unsigned long _baseAddress, uint8_t _channel)
    {

        set(_baseAddress);
        setChannel(_channel);
    }

    /** _address [0,268435455] _channel [0,15] **/
    void setBaseAddress(unsigned long _baseAddress)
    {
        DCAddress a(_baseAddress);

        // if (a.isParent())
        // {

        a.setChannel(channel());
        address = a.address;
        // }
    }

    bool operator==(unsigned long _address)
    {
        return (address == _address);
    }

    void operator=(unsigned long _address)
    {
        address = _address;
    }

    void operator=(uint8_t _bytes[])
    {
        for (uint8_t n = 0; n < DOOYA_DC::ADDRESS_CHUNKS; n++)
        {
            bytes[n] = _bytes[n];
        }
    }

    /** Returns the channel of the given address*/
    static uint8_t channel(unsigned long _address)
    {
        DCAddress a;
        a = _address;

        uint8_t c = (bit_endian_swap(a.bytes[3])) & 0x0f;
        return c;
    }

    /** Channel  */
    uint8_t channel()
    {
        return channel(address);
    }

    /** Base address, address without channel or as channel 0 */
    unsigned long baseAddress()
    {
        DCAddress out(address);
        out.bytes[3] &= 0xf;
        return out.address;
    }
    /** Checks if address is valid */
    bool isValid()
    {
        return (baseAddress() > 0);
    }

    /** Checks if this address is a parent address, valid address and channel = 0 */
    bool isParent()
    {
        return isValid() && (channel() == 0);
    }

    /** Checks if this address is a cild address, valid address and channel > 0 */
    bool isChild()
    {
        return isValid() && (channel() > 0);
    }

    /** Checks if this address is parent of another, same base address and channel = 0 */
    bool isParentOf(unsigned long _otherAddress)
    {
        if (isParent())
        {
            DCAddress other(_otherAddress);
            return (baseAddress() == other.baseAddress());
        }
        return false;
    }

    bool isParentOf(DCAddress other)
    {
        if (isParent())
        {
            // DCAddress other(_otherAddress);
            return (baseAddress() == other.baseAddress());
        }
        return false;
    }

    bool equals(unsigned long _otherAddress)
    {
        return (_otherAddress == address);
    }
    bool equals(DCAddress other)
    {
        return (other.address == address);
    }
    /** Checks if this address is equal to or parent of other given address, channel = 0  and same base address*/
    bool equalsOrParentOf(unsigned long _otherAddress)
    {
        return (address == _otherAddress) | isParentOf(_otherAddress);
    }

    /** Checks if a given address is a parent address, same base address and channel = 0 */
    static bool isParent(unsigned long _address)
    {
        DCAddress a;
        a.address = _address;
        return a.isParent();
    }
};

struct DCMessageContent
{
    DCAddress address;
    uint8_t command;
};

union DCMessageData
{
    uint8_t bytes[DOOYA_DC::CHUNKS];
    DCMessageContent content;

    DCMessageData()
    {
        content.address.address = 0;
        content.command = 0;
    }
};

struct DCMessage
{

    DCMessageData data;

    DCMessage(){};

    DCMessage(uint8_t *d)
    {
        set(d);
    }

    void clear()
    {
        data.content.address.clear();
        data.content.command = 0;
    }
    void set(uint8_t *d)
    {
        for (uint8_t n = 0; n < DOOYA_DC::CHUNKS; n++)
        {
            data.bytes[n] = d[n];
        }
    }
    void set(DCMessage other)
    {
        for (uint8_t n = 0; n < DOOYA_DC::CHUNKS; n++)
        {
            data.bytes[n] = other.data.bytes[n];
        }
    }
    bool equals(DCMessage other)
    {

        for (uint8_t n = 0; n < DOOYA_DC::CHUNKS; n++)
        {
            if (data.bytes[n] != other.data.bytes[n])
            {
                return false;
            }
        }
        return true;
    }

    bool sameAddress(DCMessage &other)
    {
        return data.content.address.equals(other.data.content.address);
    }
    bool operator==(DCMessage other)
    {
        return equals(other);
    }

    bool operator!=(DCMessage other)
    {
        return !equals(other);
    }

    DCAddress address()
    {
        return data.content.address;
    }
    /** Base address */
    unsigned long baseAddress()
    {

        return data.content.address.baseAddress();
    }

    /** Full address */
    unsigned long fullAddress()
    {
        return data.content.address.address;
    }
    /** Check if message has valid address */
    bool isValid()
    {
        return data.content.address.isValid();
    }

    uint8_t channel()
    {

        return data.content.address.channel();
    }

    uint8_t command()
    {
        return data.content.command;
    }

    bool isParent()
    {
        return data.content.address.isParent();
    }

    bool isP2()
    {
        return DOOYA_DC::isP2(data.content.command);
    }

    bool isAction()
    {
        return DOOYA_DC::isAction(data.content.command);
    }

    bool isUpOrDown()
    {
        return isCommand(DOOYA_DC::UP_PRESS) || isCommand(DOOYA_DC::DOWN_PRESS);
    }

    bool isRelease()
    {
        return isCommand(DOOYA_DC::UP_RELEASE) || isCommand(DOOYA_DC::DOWN_RELEASE);
    }

    bool isCommand(uint8_t _command)
    {
        return data.content.command == _command;
    }

    bool isStop()
    {
        return data.content.command == DOOYA_DC::STOP;
    }

    void setCommand(uint8_t c)
    {
        data.content.command = c;
        // data.bytes[4] = c;
    }

    /**  _channel [0,15] **/
    void setChannel(uint8_t _channel)
    {
        data.content.address.setChannel(_channel);
    }
    /** _address [0,268435455] _channel [0,15] **/
    void setAddress(unsigned long _address, uint8_t _channel)
    {
        data.content.address.setAddress(_address, _channel);
        // unsigned long a = bit_endian_swap(_adress, 4);
        // uint8_t c = bit_endian_swap(_channel) & 0x0f;

        // for (uint8_t n = 0; n < (DOOYA_DC::CHUNKS - 1); n++)
        // {
        //     data.bytes[n] = (_address >> 8 * n) & 0xff;
        // }

        // setChannel(_channel);
    }

    /** _address [0,268435455] _channel [0,15] **/
    void setAddress(unsigned long _address)
    {
        data.content.address.set(_address);
    }

    /** _address [0,268435455]  **/
    void setBaseAddress(unsigned long _address)
    {
        setAddress(_address, channel());
    }
#ifdef ARDUINO
    void debugBinary(Stream &s, char delimiter = ',')
    {
        for (uint8_t n = 0; n < DOOYA_DC::CHUNKS; n++)
        {
            for (int m = 0; m < 8; m++)
            {
                s.print(data.bytes[n] >> m & 1);
            }
            if (n < (DOOYA_DC::CHUNKS - 1))
                s.print(delimiter);
        }
        s.println();
    }

    void debugHex(Stream &s, char delimiter = ',')
    {
        uint8_t n;
        uint8_t m = DOOYA_DC::CHUNKS - 1;
        for (n = 0; n < m; n++)
        {
            s.print(data.bytes[n], HEX);
            s.print(delimiter);
        }
        s.println(data.bytes[n], HEX);
    }

    String getCommandString()
    {
        return DOOYA_DC::getCommandString(command());
    }

    void debug(Stream &s = Serial)
    {
        debugHex(s);
        debugBinary(s);
        s.print("Base Address: ");
        s.print(baseAddress());
        s.print(" Channel: ");
        s.print(channel());
        s.print(" ");
        s.println(DOOYA_DC::getCommandString(command()));
    }
#elif defined(RPI)

    string getBinary(char delimiter = ',')
    {
        string st = "";

        for (uint8_t n = 0; n < DOOYA_DC::CHUNKS; n++)
        {
            for (int m = 0; m < 8; m++)
            {
                st += to_string(data.bytes[n] >> m & 1);
            }
            if (n < (DOOYA_DC::CHUNKS - 1))
                st += delimiter;
        }
        return st;
    }

    void debugBinary(char delimiter = ',')
    {
        cout << getBinary(delimiter) << endl;
    }

    void debugHex(char delimiter = ',')
    {
        cout << getHex(delimiter) << endl;
    }
    string getHex(char delimiter = ',')
    {
        string st = "";
        char buff[6];
        uint8_t n;

        for (n = 0; n < DOOYA_DC::CHUNKS; n++)
        {
            sprintf(buff, "%#02x", data.byates[n]);
            st += buff;
            if (n < (DOOYA_DC::CHUNKS - 1))
                st += delimiter;
        }

        return st;
    }

    string getCommandString()
    {
        return DOOYA_DC::getCommandString(command());
    }
    void debugBase()
    {
        // debugHex();
        // debugBinary();
        cout << ("Base Address: ")
             << (baseAddress())
             << (" Channel: ")
             << (to_string(channel()))
             << (" Command: ")
             << (to_string(command()))
             << " "
             << (DOOYA_DC::getCommandString(command()))
             << endl;
    }
    void debug()
    {
        // debugHex();
        // debugBinary();
        cout << getHex()
             << " "
             << getBinary()
             << endl
             << ("Base Address: ")
             << (baseAddress())
             << (" Channel: ")
             << (to_string(channel()))
             << (" Command: ")
             << (to_string(command()))
             << " "
             << (DOOYA_DC::getCommandString(command()))
             << endl;
    }
#endif

    void send(uint8_t pin, uint8_t sendRepeats, unsigned long repeatDelayMicros)
    {
        noInterrupts();
        pinMode(pin, OUTPUT);

        // uint8_t repeats = 4;
        uint8_t n = 0;
        uint16_t w = 0;
        bool isHigh = false;

        for (uint8_t r = 0; r < sendRepeats; r++)
        {
            // sync pulse
            digitalWrite(pin, HIGH);
            delayMicroseconds(DOOYA_DC::SYNC_HIGH_WIDTH);
            digitalWrite(pin, LOW);
            delayMicroseconds(DOOYA_DC::SYNC_LOW_WIDTH);
            for (n = 0; n < DOOYA_DC::N_BITS; n++)
            {

                isHigh = (data.bytes[n / 8] >> (n % 8)) & 1;
                w = isHigh ? DOOYA_DC::HIGH_WIDTH : DOOYA_DC::LOW_WIDTH;

                digitalWrite(pin, HIGH);
                delayMicroseconds(w);
                digitalWrite(pin, LOW);
                delayMicroseconds(DOOYA_DC::SYMBOL_PERIOD - w);
            }

            delayMicroseconds(repeatDelayMicros);
        }

        interrupts();
    }

    void send(uint8_t pin)
    {
        send(pin, DC_SEND_REPEATS, DC_SEND_REPEAT_DELAY);
       
    }

    void sendCommand(uint8_t _code, uint8_t pin)
    {
        setCommand(_code);
        send(pin);
    }
};

class DCRadio
{

protected:
    static volatile uint8_t inputPin;
    static volatile uint8_t _interrupt;
#if DC_ONLY_FULL_ISR == 0
    volatile static uint8_t readMode;
#endif
    volatile static unsigned long rising_start;
    volatile static unsigned long falling_start;
    volatile static uint8_t index;
    static BitArray buffer;
    volatile static int _available;

    static unsigned long bounceMs;
    static unsigned long lastBounce;

    static DCMessage buffMsg;

    static void ISR_ATTR START()
    {
        buffer.clear();
        index = 0;
        attachInterrupt(_interrupt, SYNC_PULSE_START, RISING);
    }
    static void ISR_ATTR SYNC_PULSE_START()
    {

        rising_start = micros();
        attachInterrupt(_interrupt, SYNC_PULSE_END, FALLING);
    }

    static void ISR_ATTR SYNC_PULSE_END()
    {
        falling_start = micros();
        detachInterrupt(_interrupt);

        unsigned long rised_time = falling_start - rising_start;
        if ((rised_time >= DOOYA_DC::SYNC_PULSE_RAISED_MIN) && (rised_time <= DOOYA_DC::SYNC_PULSE_RAISED_MAX))
        {

            attachInterrupt(_interrupt, CHECK_MESSAGE_START, RISING);
        }
        else
        {

            START();
        }
    }

    static void ISR_ATTR CHECK_MESSAGE_START()
    {
        rising_start = micros();
        detachInterrupt(_interrupt);

        unsigned long fallen_time = rising_start - falling_start;

        if ((fallen_time >= DOOYA_DC::SYNC_PULSE_FALLEN_MIN) && (fallen_time <= DOOYA_DC::SYNC_PULSE_FALLEN_MAX))
        {

#if DC_ONLY_FULL_ISR == 0
            if (readMode == MODE_FULL_ISR)
                attachInterrupt(_interrupt, &PULSE_END, FALLING);
            else
            {
                _available = 1;
                // doRead = true;
            }
#else
            attachInterrupt(_interrupt, PULSE_END, FALLING);
#endif
        }
        else
        {
            START();
        }
    }

    static void ISR_ATTR PULSE_START()
    {
        detachInterrupt(_interrupt);

        unsigned long t = micros();
        if ((t - rising_start) > DOOYA_DC::SYMBOL_TIMEOUT)
        {
            START();
#if DOOYA_DC_DEBUG
            Serial.println("STO");
#endif
        }
        else
        {
            rising_start = t;
            attachInterrupt(_interrupt, PULSE_END, FALLING);
        }
    }

    static void ISR_ATTR PULSE_END()
    {
        detachInterrupt(_interrupt);
        falling_start = micros();
        unsigned long t = falling_start - rising_start;

        if (t < DOOYA_DC::SYMBOL_TIMEOUT)
        {
            if (t < DOOYA_DC::WIDTH_THRESHOLD)
            {
                buffer.clear(index);
            }
            else
            {
                buffer.set(index);
            }
            index++;
            if (index == DOOYA_DC::N_BITS)
            {
                _available = 1;
                buffMsg.set(buffer.data());
#if DOOYA_DC_DEBUG
                buffMsg.debug();
#endif
                START();
            }
            else
            {
                attachInterrupt(_interrupt, PULSE_START, RISING);
            }
        }
        else
        {
#if DOOYA_DC_DEBUG
            Serial.println("BPE");
#endif
            START();
        }
    }

    static void ISR_ATTR startISRModeRead()
    {
        detachInterrupt(_interrupt);
        uint8_t n = 0;
        int last = HIGH;
        int current;

        unsigned long lastChange = rising_start;

        while ((n < DOOYA_DC::N_BITS) && ((micros() - lastChange) < DOOYA_DC::SYMBOL_TIMEOUT))
        {
            current = digitalRead(inputPin);
            if (current != last)
            {
                lastChange = micros();

                if (current == HIGH)
                {
                    rising_start = lastChange;
                }
                else
                {
                    falling_start = lastChange;
                    if ((falling_start - rising_start) < DOOYA_DC::WIDTH_THRESHOLD)
                    {
                        buffer.clear(n);
                    }
                    else
                    {
                        buffer.set(n);
                    }
                    n++;
                }
                last = current;
            }
        }

        if (n != DOOYA_DC::N_BITS)
        {
            buffer.clear();
        }

        buffMsg.set(buffer.data());
        START();
    }

    static uint8_t readAvailable()
    {

        uint8_t a = _available;

        if (a)
        {
            _available = 0;
#if DC_ONLY_FULL_ISR == 0
            if (DCRadio::readMode == DCRadio::MODE_START_ISR)
            {
                startISRModeRead();
            }
#endif
        }
        return a;
    }

public:
    static const uint8_t MODE_FULL_ISR;
    static const uint8_t MODE_START_ISR;
    static DCMessage message;

    // static unsigned long rised_time;
    // static unsigned long fallen_time;

    DCRadio() {}
#if DC_ONLY_FULL_ISR == 0
    static void init(uint8_t pin, uint8_t mode = MODE_FULL_ISR)
    {
        readMode = mode;
#else
    static void init(uint8_t pin)
    {
#endif

        pinMode(pin, INPUT);
#ifdef ARDUINO

        _interrupt = digitalPinToInterrupt(pin);
#else
        _interrupt = pin;
#endif
        inputPin = pin;

        START();
    }

    static uint8_t ISR_ATTR available()
    {

        if (readAvailable())
        {

            if (buffMsg.isValid() && (!buffMsg.equals(message) || millisOver(lastBounce, bounceMs)))
            {
                message = buffMsg;
                lastBounce = millis();
                buffMsg.clear();
                return 1;
            }
        }
        return 0;
    }

    static void setBounce(unsigned long ms)
    {
        bounceMs = ms;
    }
    // static uint8_t bouncedAvailable()
    // {

    //     uint8_t a = available();
    //     if (elapsedMillis(lastBounce) >= bounceMs)
    //     {
    //         lastBounce = millis();
    //         return a;
    //     }
    //     return 0;
    // }

    static uint8_t waitForMessage(unsigned long timeOut)
    {

        unsigned long startMs = millis();
        uint8_t a = 0;
        while (elapsedMillis(startMs) < timeOut)
        {
            a = available();
            if (a)
            {
                break;
            }
        }

        return a;
    }

    static uint8_t waitForMessage(DCMessage msg, unsigned long timeOut)
    {

        unsigned long startMs = millis();
        uint8_t a = 0;
        while (elapsedMillis(startMs) < timeOut)
        {
            a = available();
            if (a)
            {
                if (msg.equals(message))
                {
                    return a;
                }
            }
        }

        return a;
    }

    static void ISR_ATTR listen()
    {
        START();
    }

    static void ISR_ATTR flush()
    {
        while (readAvailable())
            delay(0);
    }

    static uint8_t fetchAvailable()
    {
        return _available;
    }
};

volatile uint8_t DCRadio::inputPin = 2;
volatile uint8_t DCRadio::_interrupt = 0;
volatile unsigned long DCRadio::rising_start = 0;
volatile unsigned long DCRadio::falling_start = 0;
BitArray DCRadio::buffer = BitArray(DOOYA_DC::N_BITS);
volatile uint8_t DCRadio::index = 0;
DCMessage DCRadio::buffMsg = DCMessage();
DCMessage DCRadio::message = DCMessage();

volatile int DCRadio::_available = 0;

#if DC_ONLY_FULL_ISR == 0
const uint8_t DCRadio::MODE_FULL_ISR = 0;
const uint8_t DCRadio::MODE_START_ISR = 1;

volatile uint8_t DCRadio::readMode = 1;
#endif
unsigned long DCRadio::bounceMs;
unsigned long DCRadio::lastBounce;

struct DCLinkedAddresses
{

    uint8_t items;
    DCAddress addresses[DC_MEMORY_SIZE];
    /** Clears all memory */
    void clear()
    {
        for (uint8_t n = 0; n < DC_MEMORY_SIZE; n++)
        {
            addresses[n].address = 0;
        }
        items = 0;
    }
    // DCAddress operator[](uint8_t n)
    // {
    //     return addresses[n];
    // }

    /** Checks if address is already in memory */
    bool contains(DCAddress address)
    {
        return (indexOf(address) > -1);
    }

    /** Checks if given address is parent of any memorized address*/
    bool containsChildOf(DCAddress parent)
    {

        if (parent.isParent())
        {
            for (int n = 0; n < items; n++)
            {
                if (parent.isParentOf(addresses[n].address))
                {
                    return true;
                }
            }
        }
        return false;
    }

    bool isValid(DCAddress address)
    {
        return address.isValid() && (contains(address) || containsChildOf(address));
    }

    bool isValid(DCMessage message)
    {
        // DCAddress address = message.fullAddress();
        return isValid(message.address());
        // return address.isValid() && (contains(address) || containsChildOf(address));
    }

    /** Returns the index of the address if it's in memory*/
    int indexOf(DCAddress address)
    {
        for (int n = 0; n < items; n++)
        {
            if (address.equals(addresses[n]))
            {
                return n;
            }
        }
        return -1;
    }

    /** Adds address if valid*/
    bool add(DCAddress address)
    {

        if (!contains(address))
        {
            if (address.isValid() && !address.isParent() && (items < DC_MEMORY_SIZE))
            {
                addresses[items] = address;
                items++;
                // debug("ADDDRESS ADDDEDDD");
                return true;
            }

            return false;
        }

        return true;
    }

    bool add(DCMessage message)
    {
        return add(message.address());
    }

    /** Removes address if valid*/
    bool remove(DCAddress address)
    {
        uint8_t index = indexOf(address);
        if (index > -1)
        {
            addresses[index].address = 0;
            items--;
            for (uint8_t n = index; n < items; n++)
            {
                addresses[index].address = addresses[index + 1].address;
            }
            return true;
        }
        return false;
    };

    bool addOrRemove(DCAddress address)
    {
        if (!address.isParent())
        {
            if (contains(address))
            {
                remove(address);
            }
            else
            {
                add(address);
            }
            return true;
        }

        return false;
    }
};

class DCLinkedCommandsMemory
{

public:
    uint8_t items;
    DCMessage commands[DC_MEMORY_SIZE];

    DCLinkedCommandsMemory()
    {
        clear();
    }

    /** Clears all memory */
    void clear()
    {
        for (uint8_t n = 0; n < DC_MEMORY_SIZE; n++)
        {
            commands[n].clear();
        }
        items = 0;
    }

    /** Returns the index of the given address if stored or -1.  */
    int indexOfAddress(DCAddress address)
    {
        for (int n = 0; n < items; n++)
        {
            if (address == commands[n].fullAddress())
            {
                return n;
            }
        }
        return -1;
        // return (indexOf(command) > -1);
    }
    /** Returns the index of the address of the given command if stored or -1.  */
    int indexOfAddress(DCMessage command)
    {
        for (int n = 0; n < items; n++)
        {
            if (command.fullAddress() == commands[n].fullAddress())
            {
                return n;
            }
        }
        return -1;
        // return (indexOf(command) > -1);
    }

    bool containsAddress(DCMessage command)
    {
        return indexOfAddress(command) > -1;
        // return (indexOf(command) > -1);
    }

    bool containsCommand(uint8_t command)
    {
        for (uint8_t n = 0; n < items; n++)
        {
            if (command == commands[n].command())
            {
                return true;
            }
        }
        return false;
    }

    bool containsCommandForParent(uint8_t command, DCMessage &parentMessage)
    {

        if (parentMessage.isParent())
        {
            for (uint8_t n = 0; n < items; n++)
            {
                if (command == commands[n].command())
                {
                    if (parentMessage.address().isParentOf(commands[n].address()))
                        return true;
                }
            }
            return false;
        }
    }

    int getCommandFor(DCMessage msg)
    {
        int i = indexOf(msg);
        return (i > -1) ? commands[i].command() : i;
    }
    int getCommandForAddress(DCAddress address)
    {
        int i = indexOf(address);
        return (i > -1) ? commands[i].command() : i;
    }

    // int getCommandForAddressOrChild(DCAddress address)
    // {
    //     int i = indexOfAddress(address);
    //     return (i > -1) ? commands[i].command() : i;
    // }
    bool contains(DCMessage command)
    {

        for (uint8_t n = 0; n < items; n++)
        {
            if (command.equals(commands[n]))
            {
                return true;
            }
        }
        return false;
    }

    //     return false;
    //     // return (indexOf(command) > -1);
    // }

    int countChildren(DCMessage command)
    {

        int children = 0;
        if (command.address().isParent())
        {
            for (int n = 0; n < items; n++)
            {
                if (command.address().isParentOf(commands[n].fullAddress()))
                {
                    children++;
                }
            }
        }
        return children;
    }

    DCMessage getChild(DCMessage command, int childN)
    {

        int children = 0;
        DCMessage emptyMesssage;
        if (command.address().isParent())
        {
            for (int n = 0; n < items; n++)
            {
                if (command.address().isParentOf(commands[n].fullAddress()))
                {
                    // children++;
                    if (++children == childN)
                    {
                        return commands[n];
                    }
                }
            }
        }
        return emptyMesssage;
    }

    bool containsChildOf(DCMessage command)
    {

        if (command.data.content.address.isParent())
        {
            for (int n = 0; n < items; n++)
            {
                if (command.address().isParentOf(commands[n].fullAddress()))
                {
                    return true;
                }
            }
        }
        return false;
    }

    /** Returns the index of the address if it or any child is contained */
    int indexOf(DCAddress address)
    {
        for (int n = 0; n < items; n++)
        {
            if (address.equalsOrParentOf(commands[n].fullAddress()))
            {
                return n;
            }
        }
        return -1;
    }

    /** Returns the index of the address if it or it's first child is contained */
    int indexOf(DCMessage command)
    {
        for (int n = 0; n < items; n++)
        {
            if ((command.sameAddress(commands[n]) || command.address().isParentOf(commands[n].address().address)) && (command.isCommand(commands[n].command())))
            {
                return n;
            }
        }
        return -1;
    }

    /** Adds address if valid*/
    virtual bool add(DCMessage command)
    {
        bool success = false;
        if (command.address().isChild())
        {
            success = true;
            int i = indexOfAddress(command);
            if (i > -1)
            {
                commands[i] = command;
            }
            else if (items < DC_MEMORY_SIZE)
            {
                commands[items] = command;
                items++;
            }
            else
            {
                success = false;
            }
        }

        return success;
    }

    /** Removes address if valid*/
    bool remove(DCMessage command)
    {
        uint8_t index = indexOfAddress(command);
        if (index > -1)
        {
            commands[index].clear();
            items--;
            for (uint8_t n = index; n < items; n++)
            {
                commands[index] = commands[index + 1];
            }
            return true;
        }
        return false;
    };

    bool addOrRemove(DCMessage command)
    {
        if (!command.data.content.address.isParent())
        {
            if (containsAddress(command))
            {
                remove(command);
            }
            else
            {
                add(command);
            }
            return true;
        }

        return false;
    }
};

class DCOnCommands : public DCLinkedCommandsMemory
{
public:
    DCOnCommands()
    {
    }

    /** Adds address if valid*/
    virtual bool add(DCMessage command)
    {
        bool success = false;
        if (command.address().isChild() && (command.isUpOrDown() || command.isP2() || command.isCommand(DOOYA_DC::STOP)))
        {
            if (command.isP2() || command.isCommand(DOOYA_DC::STOP))
            {
                command.setCommand(DOOYA_DC::UP_PRESS);
            }
            success = true;

            int i = indexOfAddress(command);
            if (i > -1)
            {
                commands[i] = command;
                // debug("changed");
            }
            else if (items < DC_MEMORY_SIZE)
            {
                commands[items] = command;
                items++;
                // debug("added");
            }
            else
            {
                success = false;
            }
        }
        // debug("not added");
        return success;
    }
};

class DCMessagesBaseBuffer
{

public:
    static const uint8_t MAX_ITEMS = 3;
    DCMessagesBaseBuffer(/* args */) {}
    ~DCMessagesBaseBuffer() {}

    void clear()
    {

        for (uint8_t n = 0; n < MAX_ITEMS; n++)
        {
            buff[n].clear();
        }
        items = 0;
    }

    uint8_t length() { return items; }

protected:
    DCMessage buff[MAX_ITEMS];
    uint8_t items;

    bool empty() { return items == 0; }
    bool notEmpty() { return items > 0; }
    bool full() { return items == MAX_ITEMS; }
    bool moreThanOne() { return items > 1; }
};

class DCCommandsQueue : public DCMessagesBaseBuffer
{

protected:
    unsigned long lastAddedMs;
    unsigned long addBounce = 100;

public:
    void setAddBounce(unsigned long ms)
    {
        addBounce = ms;
    }
    bool start(DCMessage msg)
    {
        clear();
        return addMessage(msg);
    }

    void buffAdd(DCMessage msg)
    {

        for (int n = (MAX_ITEMS - 1); n > 0; n--)
        {
            buff[n] = buff[n - 1];
        }
        buff[0] = msg;
        if (items < MAX_ITEMS)
        {
            items++;
        }
        lastAddedMs = millis();
    }

    DCMessage last()
    {
        return buff[0];
    }

    bool upDownPressedFilter(DCMessage &msg)
    {
        return !(last().isUpOrDown() && last().equals(msg) && !millisOver(lastAddedMs, addBounce));
    }
    bool addMessage(DCMessage msg)
    {
        // filter up and down

        bool canBeAdded = upDownPressedFilter(msg);
        if (canBeAdded)
        {
            buffAdd(msg);
        }

        return canBeAdded;

        //     if (last().isUpOrDown() && last().equals(msg) && !millisOver(lastAddedMs, addBounce))
        //     {
        //         return false;
        //     }
        //     buffAdd(msg);
        //     return true;
    }

    bool sameAddress()
    {

        if (items >= 1)
        {
            for (int n = 1; n < items; n++)
            {
                if (!buff[n].sameAddress(buff[n - 1]))
                {
                    return false;
                }
            }
            return true;
        }

        return false;
    }

    // void debug(Stream &s = Serial)
    // {
    //     if (items == 0)
    //     {
    //         s.println("Empty settings buffer");
    //     }
    //     else
    //     {
    //         for (int n = 0; n < items; n++)
    //         {
    //             s.print(DOOYA_DC::getCommandString(buff[n].command()));
    //             s.print(" ");
    //         }
    //         if (sameAddress())
    //         {
    //             s.println("A");
    //         }
    //         else
    //         {
    //             s.println("A B");
    //         }
    //     }
    // }
};

class DCSettingsBuffer : public DCMessagesBaseBuffer
{
public:
    bool start(DCMessage msg)
    {
        clear();
        return addMessage(msg);
    }

    /*
     * @brief 2 first same address, third
     *
     * @param msg
     * @return true
     * @return false
     */
    bool addMessage(DCMessage msg)
    {

        switch (length())
        {
        // adds first message
        case 0:
            if (msg.isP2())
            {
                buff[items++] = msg;

                return true;
            }
            return false;

        //
        case 1:
            if (buff[0].sameAddress(msg))
            {
                if (!msg.isRelease())
                {
                    buff[items++] = msg;
                }
                return true;
            }
            clear();
            return false;
        case 2:
            // if (sameAddress())
            // {

            //     if (buff[1].sameAddress(msg))
            //     {
            //         if (msg.isRelease())
            //         {
            //             return true;
            //         }
            //         if (buff[1].isP2() && msg.isP2())
            //         {
            //             clear();
            //             return false;
            //         }
            //     }
            //     else
            //     {
            //         if (msg.isRelease())
            //         {
            //             clear();
            //             return false;
            //         }
            //     }

            //     buff[length] = msg;
            //     length++;
            //     return true;
            // }
            // break;

            if (sameAddress())
            {

                if (buff[1].sameAddress(msg))
                {
                    if (msg.isRelease())
                    {
                        return true;
                    }
                    if (buff[1].isP2() && msg.isP2())
                    {
                        clear();
                        return false;
                    }
                }
                else
                {
                    if (msg.isRelease())
                    {
                        clear();
                        return false;
                    }
                }

                buff[items++] = msg;
                return true;
            }
            break;
        default:
            break;
        }

        clear();
        return false;
        // return addMessage(msg);
    }

    bool startsWithP2()
    {
        if (notEmpty())
        {

            return (buff[0].isP2());
        }

        return false;
    }
    bool startsWithDoubleP2()
    {
        if (moreThanOne())
        {

            return (buff[0] == buff[1]) && buff[0].isP2();
            // return (buff[0].sameAddress(buff[1]) && buff[0].isP2() && buff[1].isP2());
        }

        return false;
    }

    bool sameAddress()
    {

        if (moreThanOne())
        {
            for (int n = 1; n < items; n++)
            {
                if (!buff[n].sameAddress(buff[n - 1]))
                {
                    return false;
                }
            }
            return true;
        }

        return false;
    }
    // P2A P2B
    bool isAddOrRemoveDevice()
    {
        return (startsWithDoubleP2() && !sameAddress());
    }
    // P2 STOP P2
    bool isRemoveAllDevices()
    {
        if (settingsAvailable() && sameAddress())
        {
            return first().isP2() && middle().isCommand(DOOYA_DC::STOP) && last().isP2();
        }
        return false;
    }

    inline DCMessage first()
    {
        return buff[0];
    }
    inline DCMessage middle()
    {
        return buff[1];
    }
    inline DCMessage last()
    {
        return buff[2];
    }

    bool settingsAvailable()
    {
        return items == MAX_ITEMS;
    }

    bool isLinkingCommand()
    {
        return settingsAvailable() && sameAddress() && startsWithDoubleP2();
    }
#ifdef ARDUINO
    void debug(Stream &s = Serial)
    {
        if (empty())
        {
            s.println("Empty settings buffer");
        }
        else
        {
            for (int n = 0; n < length(); n++)
            {
                s.print(DOOYA_DC::getCommandString(buff[n].command()));
                s.print(" ");
            }
            if (sameAddress())
            {
                s.println("A");
            }
            else
            {
                s.println("A B");
            }
        }
    }
#endif
};

class DCReceiverEvents
{
private:
    /* data */
public:
    // DCReceiverEvents(/* args */);
    // ~DCReceiverEvents();

    virtual void firstResponse() {}
    virtual void secondResponse() {}
    virtual void confirmationResponse() {}
    virtual void errorResponse() {}
    // input listeners
    virtual void onCommandReceived(DCMessage m) {}
    virtual void onSettingsReceived(DCSettingsBuffer buff) {}
    // memory events
    virtual DCLinkedAddresses loadLinkedDevices()
    {
        DCLinkedAddresses l;
        return l;
    }

    virtual void saveLinkedDevices(DCLinkedAddresses a) {}
    virtual void saveScriptData() {}
    // memory events
    virtual void loadData() {}
    virtual void saveData() {}

    virtual bool onAddressAdded(DCMessage m) { return true; }
    virtual void onAddressRemoved(DCMessage m) {}
    virtual void onAllAddressesRemoved() {}
};

class DCReceiver : public DCRadio

{

protected:
    bool checkSettingsMessage()
    {
        bool noError, success;
        noError = true;
        success = false;

        if (settingsBuffer.start(message))
        {
            // debug("settings started");
            re->firstResponse();
            flush();

            unsigned long lastCommand = millis();

            while (!success && noError)
            {

                if (available())
                {

                    if (settingsBuffer.addMessage(message))
                    {

                        if (settingsBuffer.settingsAvailable())
                        {
                            success = true;
                        }
                        else if (settingsBuffer.length() == 2)
                        {

                            if (!message.isRelease())
                            {
                                re->secondResponse();
                            }
                        }
                        else
                        {

                            noError = false;
                        }

                        lastCommand = millis();
                    }
                    else
                    {

                        // debugValue("Settings cant add message ",settingsBuffer.length);
                        noError = false;
                    }

                    flush();
                }
                else if (elapsedMillis(lastCommand) > COMMAND_TIMEOUT)
                {
                    //  debug("Settings timeout");
                    noError = false;
                }
            }

            if (!noError)
            {
                re->errorResponse();
                settingsBuffer.clear();
            }
        }
        // else {
        //     debug("can't start settings");
        // }
        flush();
        return success;
    }

    bool receiverSettingsInput()
    {
        bool consumed = false;
        if (settingsBuffer.isAddOrRemoveDevice())
        {
            consumed = true;
            if (linkedAddresses.contains(settingsBuffer.last().fullAddress()))
            {
                linkedAddresses.remove(settingsBuffer.last().fullAddress());
                onAddressRemoved(settingsBuffer.last());
            }
            else if (linkedAddresses.add(settingsBuffer.last().fullAddress()))
            {

                onAddressAdded(settingsBuffer.last());
            }
            else
            {
                consumed = false;
            }
            // else
            // {
            //     linkedAddresses.remove(settingsBuffer.last().fullAddress());
            //     onAddressRemoved(settingsBuffer.last());
            // }
            if (consumed)
                saveAddressesToMemory();

            consumed = true;
            // if (linkedAddresses.addOrRemove(settingsBuffer.last().fullAddress()))
            // {
            //     saveAddressesToMemory();
            //     consumed = true;
            // }
        }
        else if (settingsBuffer.isRemoveAllDevices())
        {
            linkedAddresses.clear();
            saveAddressesToMemory();
            consumed = true;
            onAllAdressesRemoved();
        }

        if (consumed)
        {
            re->confirmationResponse();
        }
        return consumed;
    }

    bool isLinked(DCMessage &msg)
    {
        return linkedAddresses.isValid(msg);
    }
    bool onMessageReceived()
    {
        if (isLinked(message))
        {

            // SETTINGS
            if (checkSettingsMessage())
            {

                if (!receiverSettingsInput())
                {
                    onSettingsReceived(settingsBuffer);
                }
                flush();
            }

            // COMMANDS
            else
            {
                onCommandReceived(message);
            }

            return true;
        }

        return false;
    }

    bool onAddressAdded(DCMessage m)
    {
        return re->onAddressAdded(m);
        // if (re->onAddressAdded != 0)
        // {
        //     return _onAddressAdded(m);
        // }
        // return true;
    }
    void onAddressRemoved(DCMessage m)
    {
        re->onAddressRemoved(m);
        // if (_onAdressRemoved)
        // {
        //     _onAdressRemoved(m);
        // }
    }
    void onAllAdressesRemoved()
    {
        re->onAllAddressesRemoved();
        // execute(_onAllAdressesRemoved);
    }

public:
    DCLinkedAddresses linkedAddresses;
    DCSettingsBuffer settingsBuffer;
    // ResponsesListeners resp;
    DCReceiverEvents *re;
    bool silence;
    bool isRepeat;
    unsigned long pressStartMs;
    DCMessage lastMessage;

    DCReceiver(/* args */)
    {
        isRepeat = false;
        silence = true;
    }
    ~DCReceiver()
    {
    }

    void setEvents(DCReceiverEvents *e)
    {
        re = e;
    }
    void loadAddressesFromMemory()
    {

        DCLinkedAddresses a;
        bool valid = false;

        if (re)
        {
            a = re->loadLinkedDevices();

            if (a.items)
            {
                valid = true;
                for (int n = 0; n < a.items; n++)
                {
                    if (!a.addresses[n].isValid())
                    {
                        valid = false;
                        break;
                    }
                }
            }
        }
        else
        {
            valid = false;
        }

        if (valid)
        {
            linkedAddresses = a;
        }
        else
        {
            linkedAddresses.clear();
            saveAddressesToMemory();
        }
    }

    void saveAddressesToMemory()
    {
        re->saveLinkedDevices(linkedAddresses);
        // if (_saveAddressesToMemory)
        // {
        //     _saveAddressesToMemory(linkedAddresses);
        // }
    }

    void onCommandReceived(DCMessage m)
    {
        re->onCommandReceived(m);
        // execute(&_onCommand,m);
        // if (_onCommand)
        // {
        //     _onCommand(m);
        // }
    }

    void onSettingsReceived(DCSettingsBuffer s)
    {
        re->onSettingsReceived(s);
        // if (_onSettings)
        // {
        //     _onSettings(s);
        // }
    }

    bool addAddress(DCMessage m)
    {

        if (linkedAddresses.add(m.fullAddress()))
        {
            if (onAddressAdded(m))
            {
                saveAddressesToMemory();
                re->confirmationResponse();
                return true;
            }
            else
            {
                linkedAddresses.remove(m.fullAddress());
            }
        }
        re->errorResponse();
        return false;
    }

    bool removeAddress(DCMessage m)
    {
        if (m.isValid())
        {
            if (linkedAddresses.remove(m.fullAddress()))
            {
                saveAddressesToMemory();
                onAddressRemoved(m);
            }
            re->confirmationResponse();
            return true;
        }

        re->errorResponse();
        return false;
    }

    void doLinking()
    {

        unsigned long listeningStart, lastCommand;
        listeningStart = lastCommand = millis();
        bool error;
        settingsBuffer.clear();
        while (elapsedMillis(listeningStart) < LINKING_TIMEOUT)
        {
            delay(0);
            if (elapsedMillis(lastCommand) > COMMAND_TIMEOUT)
            {
                // debug("command timeout");
                settingsBuffer.clear();
                lastCommand = millis();
                error = false;
                // debug("command timeout end");
            }

            if (available())
            {

                error = false;
                // debug("available input");

                if (settingsBuffer.addMessage(message))
                {

                    if (settingsBuffer.isLinkingCommand())
                    {

                        if (addAddress(settingsBuffer.last()))
                        {
                            flush();
                            return;
                        }
                        else
                        {

                            error = true;
                        }
                        // linkedAddresses.add(settingsBuffer.last().address());
                        // // settingsBuffer.clear();
                        // saveAddressesToMemory();
                        // onAddressAdded(settingsBuffer.last());
                        // flush();
                        // resp.confirmation();
                        // return;
                    }
                    else if (settingsBuffer.length() == 1)
                    {

                        // debug("P2");
                        re->firstResponse();
                        listeningStart += 1000;
                        // lastCommand = millis();
                    }
                    else if (settingsBuffer.length() == 2)
                    {
                        if (settingsBuffer.startsWithDoubleP2())
                        {

                            re->secondResponse();
                            listeningStart += 1000;

                            // lastCommand = millis();
                        }
                        else
                        {
                            if (linkedAddresses.isValid(message.address()) && message.isAction())
                            {
                                onCommandReceived(message);
                                flush();
                                return;
                            }
                            else
                            {
                                error = true;
                            }
                        }
                    }
                    else
                    {
                        // debug("settings lenght = 3");
                        error = true;
                    }

                    lastCommand = millis();
                }
                else
                {
                    // debug("can't add message");
                    if (linkedAddresses.isValid(message.address()) && message.isAction())
                    {
                        onCommandReceived(message);
                        flush();
                        return;
                    }
                    error = true;
                }

                if (error)
                {
                    // debug("error started");
                    settingsBuffer.clear();
                    // debug("error settings buffer cleared");
                    // resp.error();
                    // listeningStart += 1000;
                    lastCommand = millis();
                    error = false;
                }
                flush();
            }
        }

        return;
    }

    bool update()
    {
        if (millisOver(lastBounce, bounceMs))
        {
            lastBounce = millis();

            if (readAvailable() && isLinked(buffMsg))
            {
                message.set(buffMsg);

                DCMessage m;
                m.set(message);
                if (silence)
                {
                    pressStartMs = millis();
                    isRepeat = false;
                    silence = false;
                    lastMessage.clear();
                    //  debug("woke up");
                }
                else
                {
                    if ((lastMessage.equals(m)))
                    {
                        // debug("equals");
                        // if (!isRepeat)
                        // {
                        isRepeat = true;
                        // }
                    }
                    else
                    {
                        //   debug("not equals");
                        pressStartMs = millis();
                        isRepeat = false;
                    }
                }

                bool ok = onMessageReceived();
                lastMessage.set(m);
                // debugValue("Silence", silence);
                // debugValue("isRepeat", silence);
                return ok;
            }
            // else /*if (!silence)*/
            // {
            lastMessage.clear();
            silence = true;
            isRepeat = false;
        }

        return false;
    }

    bool pressed()
    {
        return (!silence && isRepeat) ? true : false;
    }

    unsigned long msPressed()
    {
        return pressed() ? (millis() - pressStartMs) : 0;
    }

    bool longPressed()
    {
        return (msPressed() > 2999);
    }

    bool firstPress()
    {
        return (!silence && !isRepeat);
    }
};

#endif
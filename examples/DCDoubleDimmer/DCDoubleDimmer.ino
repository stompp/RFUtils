#include "dc_config.h"
#include <ValueWatcher.h>
// #include <SoftwareSerial.h>
// SoftwareSerial bluetooth(D2, D3);

ValueWatcher<uint8_t> watcher[N_OUTPUTS];

#define STREAM_INPUT_BUFFER_SIZE 50
String streamInputBuffer;

// On,Xvalue
void sketchStringInput(String &s)
{
  if (s.length() > 0)
  {
    if (getCSVElementsLength(s) == 2)
    {
      int i = s.indexOf(',');
      String first = s.substring(0, i);
      String second = s.substring(i + 1);

      /*Output selection 0 is all*/
      if (first.startsWith("O"))
      {
        first = first.substring(1);
        int outputIndex = first.toInt();

        for (int n = 0; n < N_OUTPUTS; n++)
        {
          if (outputIndex == 0 || (n == (outputIndex - 1)))
            dimmers[n].stringInput(second);
        }
      }
    }
  }
}

void streamInputLoop(Stream &s = Serial)
{

  while (s.available())
  {
    streamInputBuffer = "";
    readStreamLineToString(&s, streamInputBuffer, 50, STREAM_INPUT_BUFFER_SIZE);
    streamInputBuffer.trim();
    sketchStringInput(streamInputBuffer);
  }
}

void onValueChanged(ValueWatcher<uint8_t> *v)
{

  leds[v->getID()].write(v->getValue());
  dimmers[v->getID()].debug();
}

void setup()
{

  Serial.begin(74880);
  Serial.println();

  streamInputBuffer.reserve(STREAM_INPUT_BUFFER_SIZE);

#ifdef USE_ESP_EEPROM
  EEPROM.begin(ESP_EEPROM_SIZE);
  // debug("ESP_EEPROM began");
#endif

  // leds[0].init(LED1_PIN, LED_IS_CC);
  // leds[1].init(LED2_PIN, LED_IS_CC);

  for (int n = 0; n < N_OUTPUTS; n++)
  {
    leds[n].init(LEDS_PINS[n], LEDS_CC[n]);
    dimmers[n].transitions.enableAll(true);
    dimmers[n].setDimmingTransitionMs(100);
    dimmers[n].setFadeOutMs(500);
    dimmers[n].setLevels(10);
    watcher[n].setOnValueChangedListener(&onValueChanged);
    watcher[n].setID(n);
  }

  commandsQueue.setAddBounce(100);

  rx.init(DC_INTERRUPT_PIN);
  rx.setBounce(DC_REPETITION_BOUNCE);
  rx.setEvents(&scriptEvents);
  rx.loadAddressesFromMemory();

  scriptEvents.firstResponse();

  rx.doLinking();
  dimmers[0].debug();
}

void loop()
{

  if (!rx.update())
  {
    streamInputLoop();
  }

  for (int n = 0; n < N_OUTPUTS; n++)
  {
    watcher[n].update(dimmers[n].output());
  }
}

#include "dc_config.h"

// #include <SoftwareSerial.h>
// SoftwareSerial bluetooth(D2, D3);

void setup()
{

  // Initialize serial and wait for port to open:
  Serial.begin(74880);
  Serial.println();
  // bluetooth.begin(9600);

#ifdef USE_ESP_EEPROM
  EEPROM.begin(ESP_EEPROM_SIZE);
  // debug("ESP_EEPROM began");
#endif

  dimmer.setBrightnessTransitionMs(100);
  dimmer.setFadeOutMs(500);
  dimmer.setBrightnessLevels(10);

  commandsQueue.addBounce = 100;
  
  rx.init(DC_INTERRUPT_PIN);
  rx.setBounce(DC_REPETITION_BOUNCE);
  rx.setEvents(&scriptEvents);
  rx.loadAddressesFromMemory();
  
  scriptEvents.firstResponse();
  
  rx.doLinking();

  // slowInputsTimer.start();
  // buttonBounce.start();
  
}

void loop()
{
  
  uint8_t b = dimmer.outputBrightness();
  led.write(b);
  
  if (rx.update())
  {
  }
}

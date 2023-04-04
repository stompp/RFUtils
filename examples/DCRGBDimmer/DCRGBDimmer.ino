
#include "dc_config.h"


// #include <SoftwareSerial.h>
// SoftwareSerial bluetooth(D2, D3);

#if USE_ARDUINO_IOT == 1
int kelvin;
bool switchValue;
MillisTimer cloudUpdateTimer(300, true, true, true);
void postStatus()
{

  thingKelvin = kelvin;
  RGB rgb = rgbDimmer.getRGB();
  HSV hsv = rgb.toHSV();
  float b = (100.0f * rgb.brightness()) / 255.0f;
  thingBrightness.setBrightness(b);
  thingBrightness.setSwitch(switchValue);

  thingColor.getValue().setColorRGB(rgb.red, rgb.green, rgb.blue);

  // thingColor.requestUpdate();

  thingColoredLight.setHue(hsv.hue);
  thingColoredLight.setBrightness(b);
  thingColoredLight.setSaturation((100.0f * hsv.saturation) / 255.0f);

  thingColoredLight.setSwitch(switchValue);
}
#endif
void setup()
{
  // led.blink(COLOR_ORANGE_PEEL, 2, 250);

  // Initialize serial and wait for port to open:
  Serial.begin(74880);
  Serial.println();
  // bluetooth.begin(9600);

#if USE_ARDUINO_IOT == 1
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(-1);
#endif

#ifdef USE_ESP_EEPROM
  EEPROM.begin(ESP_EEPROM_SIZE);
  debug("ESP_EEPROM began");
#endif

  // rgbDimmer
  rgbDimmer.setStatusOnOnToneSet = true;
  rgbDimmer.enableTransitions(true);
  rgbDimmer.setToneDimmingMs(100);
  rgbDimmer.setBrightnessDimmingMs(100);
  rgbDimmer.setFadeOutMs(400);
  
  rgbDimmer.setBrightnessLevels(10);
  rgbDimmer.setHueLevels(180);
  // rgbDimmer.setHueLevels(320);

  commandsQueue.setAddBounce(100);

  rx.init(DC_INTERRUPT_PIN);
  rx.setBounce(DC_REPETITION_BOUNCE);
  rx.setEvents(&scriptEvents);
  rx.loadAddressesFromMemory();

  scriptEvents.firstResponse();
  
  rx.doLinking();
  

  debug(rgbDimmer.transitionsEnabled());

  debug("setup end");
}

void loop()
{
  ColorUtils::RGB o = rgbDimmer.getRGB();
  led.write(o.red, o.green, o.blue);

  if (rx.update())
  {
    // ColorUtils::RGB o = rgbDimmer.getRGB();
    // led.write(o.red, o.green, o.blue);
  }
  else
  {
    //  ColorUtils::checkStreamInput(rgbDimmer, Serial);
    // postStatus();
#if USE_ARDUINO_IOT == 1
    if (cloudUpdateTimer.check())
    {
      postStatus();
      ArduinoCloud.update();
    }
#endif

    // ColorUtils::checkStreamInput(rgbDimmer, bluetooth);
  }
}
#if USE_ARDUINO_IOT == 1
/*
  Since ThingColor is READ_WRITE variable, onThingColorChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onThingColorChange()
{
  ColorUtils::RGB rgb;
  thingColor.getValue().getRGB(rgb.red, rgb.green, rgb.blue);
  ColorUtils::HSV hsv = rgb.toHSV();

  rgb.maximize();
  rgbDimmer.setHue(hsv.hue);
  rgbDimmer.setBrightness(hsv.value);

  postStatus();
  // writeColorToLed();
}
/*
  Since ThingColoredLight is READ_WRITE variable, onThingColoredLightChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onThingColoredLightChange()
{
  // Add your code here to act upon ThingColoredLight change
  // led.blink(COLOR_VIOLET_WEB, 5, 200);
  ColorUtils::HSV hsv = ColorUtils::HSV::FROMHSV100(thingColoredLight.getHue(), thingColoredLight.getSaturation(), thingColoredLight.getBrightness());
  // ColorUtils::RGB rgb = hsv.toRGB();

  // rgb.maximize();
  // rgbDimmer.setRGB(rgb);
  // rgbDimmer.setBrightness(hsv.value);

  // rgbDimmer.setHue(hsv.hue);
  rgbDimmer.setHSV(hsv);

  switchValue = thingColoredLight.getSwitch();
  if (switchValue)
  {
    rgbDimmer.on();
  }
  else
  {
    rgbDimmer.off();
  }
  postStatus();
  // writeColorToLed();
}
/*
  Since ThingBrightness is READ_WRITE variable, onThingBrightnessChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onThingBrightnessChange()
{
  switchValue = thingBrightness.getSwitch();
  // Add your code here to act upon ThingBrightness change
  rgbDimmer.setBrightness(map(thingBrightness.getBrightness(), 0, 100, 0, 255));
  if (switchValue)
  {
    rgbDimmer.on();
  }
  else
  {
    rgbDimmer.off();
  }

  postStatus();
}
/*
  Since ThingKelvin is READ_WRITE variable, onThingKelvinChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onThingKelvinChange()
{
  // Add your code here to act upon ThingKelvin change
  kelvin = thingKelvin;
  rgbDimmer.setTemperature(kelvin);
  postStatus();
}

// void writeColorToLed()
// {

//   if (switchValue)
//   {
//     led.write(ledColor.red(), ledColor.green(), ledColor.blue());
//   }
//   else
//   {
//     led.off();
//   }

//   thingKelvin = kelvin;

//   thingBrightness.setBrightness((100.0f * ledColor.brightness()) / 255.0f);
//   thingBrightness.setSwitch(switchValue);

//   thingColor.getValue().setColorRGB(ledColor.red(), ledColor.green(), ledColor.blue());
//   thingColor.requestUpdate();
//   thingColoredLight.setHue(ledColor.hue());
//   thingColoredLight.setBrightness((100.0f * ledColor.brightness()) / 255.0f);
//   thingColoredLight.setSaturation((100.0f * ledColor.saturation()) / 255.0f);

//   thingColoredLight.setSwitch(switchValue);
// }
#endif
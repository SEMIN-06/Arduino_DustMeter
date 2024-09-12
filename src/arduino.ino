#include <LedControl.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial btSerial(2, 3);
LedControl lc = LedControl(9, 11, 10, 1); // DIN, CLK, CS
LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned long prevMillisOfDisplay = 0;

struct _emoticons
{
  char verybad[9][8];
  char bad[9][8];
  char soso[9][8];
  char good[9][8];
} emoticons;

unsigned int selectedDustLed = 0; // 1 - 좋음 2 - 보통 3 - 나쁨 4 - 매우 나쁨

int measurePin = 0;
int ledPower = 3;
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;
#define NUMREADINGS 10
int readings[NUMREADINGS] = {
    0,
};
int index = 0;
int total = 0;
int val = 0;

void setup()
{
  Serial.begin(9600);
  btSerial.begin(9600);
  Serial.setTimeout(5000);
  pinMode(ledPower, OUTPUT);

  lc.shutdown(0, false);
  lc.setIntensity(0, 3); // 0 - 15
  lc.clearDisplay(0);

  lcd.begin();

  /*_emoticons initData = {
    {"00000000", "00000000", "00000000", "00000000", "00000000", "00000000", "00000000", "00000000"},
    {"00000000", "00000000", "00000000", "00000000", "00000000", "00000000", "00000000", "00000000"},
    {"00000000", "00000000", "00000000", "00000000", "00000000", "00000000", "00000000", "00000000"},
    {"00000000", "00000000", "00000000", "00000000", "00000000", "00000000", "00000000", "00000000"}

  };
  EEPROM.put(0, initData);*/

  EEPROM.get(0, emoticons);
}

void loop()
{
  if (btSerial.available() > 0)
  {
    // Serial.write(btSerial.read());

    String command = btSerial.readStringUntil("|");
    Serial.println(command);
    if (command.startsWith("set"))
    {
      int commandIndex = command.indexOf('/');
      if (commandIndex != -1)
      {
        String value = command.substring(commandIndex + 1, command.length());
        Serial.println(value);

        unsigned int valueIndexes[9] = {};
        String valueStrings[9] = {};

        for (int i = 0; i < 9; i++)
        {
          if (i == 0)
          {
            valueIndexes[i] = value.indexOf(',');
          }
          else
          {
            valueIndexes[i] = value.indexOf(',', (valueIndexes[i - 1] + 1));
          }
        }

        for (int j = 0; j < 9; j++)
        {
          if (j == 0)
          {
            valueStrings[j] = value.substring(0, valueIndexes[j]);
          }
          else
          {
            valueStrings[j] = value.substring((valueIndexes[j - 1] + 1), (valueIndexes[j]));
          }
        }

        valueStrings[8].replace("|", "");

        Serial.println("-datas-");
        for (int k = 0; k < 8; k++)
        {
          Serial.print(k);
          Serial.print(": ");
          Serial.println(valueStrings[k]);
          // showLineLed(k, valueStrings[k]);
          selectedDustLed = 0;
        }

        Serial.print("type: ");
        Serial.println(valueStrings[8]);

        if (valueStrings[8] == "1")
        {
          for (int k = 0; k < 8; k++)
          {
            char _char[8] = {0};
            valueStrings[k].toCharArray(_char, 9);
            strcpy(emoticons.good[k], _char);
          }
        }
        else if (valueStrings[8] == "2")
        {
          for (int k = 0; k < 8; k++)
          {
            char _char[8] = {0};
            valueStrings[k].toCharArray(_char, 9);
            strcpy(emoticons.soso[k], _char);
          }
        }
        else if (valueStrings[8] == "3")
        {
          for (int k = 0; k < 8; k++)
          {
            char _char[8] = {0};
            valueStrings[k].toCharArray(_char, 9);
            strcpy(emoticons.bad[k], _char);
          }
        }
        else if (valueStrings[8] == "4")
        {
          for (int k = 0; k < 8; k++)
          {
            char _char[8] = {0};
            valueStrings[k].toCharArray(_char, 9);
            strcpy(emoticons.verybad[k], _char);
          }
        }

        EEPROM.put(0, emoticons);
      }
    }
  }

  if (millis() - prevMillisOfDisplay >= 1000)
  {
    prevMillisOfDisplay = millis();

    digitalWrite(ledPower, LOW);
    delayMicroseconds(samplingTime);

    voMeasured = analogRead(measurePin);

    delayMicroseconds(deltaTime);
    digitalWrite(ledPower, HIGH);
    delayMicroseconds(sleepTime);

    calcVoltage = voMeasured * (5.0 / 1024.0);

    if (calcVoltage > 0.52)
    {
      dustDensity = (calcVoltage - 0.52) / 0.005;
    }
    else
    {
      dustDensity = 0;
    }

    Serial.print("Raw Signal Value (0-1023): ");
    Serial.print(voMeasured);

    Serial.print(" - Voltage: ");
    Serial.print(calcVoltage);

    Serial.print(" - NOW Dust Density: ");
    Serial.print(dustDensity);
    Serial.print(" - AVG Dust Density: ");
    total -= readings[index];

    if ((dustDensity) < 0)
    {
      readings[index] = 0;
    }
    else
    {
      readings[index] = dustDensity;
    }

    total += readings[index];
    index++;

    if (index >= NUMREADINGS)
    {
      index = 0;
    }

    val = total / NUMREADINGS;
    Serial.println(val);

    btSerial.print("dataStart");
    btSerial.print(" ");
    btSerial.print(val);
    btSerial.print(" ");
    btSerial.print("dataEnd");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Dust: ");
    lcd.print(val);
    lcd.print(" ug/m3");

    if (val < 35)
    {
      // 좋음
      if (selectedDustLed != 1)
      {
        selectedDustLed = 1;
        for (int k = 0; k < 8; k++)
        {
          showLineLed(k, emoticons.good[k]);
        }
      }
    }
    else if (val < 75)
    {
      // 보통
      if (selectedDustLed != 2)
      {
        selectedDustLed = 2;
        for (int k = 0; k < 8; k++)
        {
          showLineLed(k, emoticons.soso[k]);
        }
      }
    }
    else if (val < 115)
    {
      // 나쁨
      if (selectedDustLed != 3)
      {
        selectedDustLed = 3;
        for (int k = 0; k < 8; k++)
        {
          showLineLed(k, emoticons.bad[k]);
        }
      }
    }
    else
    {
      // 매우 나쁨
      if (selectedDustLed != 4)
      {
        selectedDustLed = 4;
        for (int k = 0; k < 8; k++)
        {
          showLineLed(k, emoticons.verybad[k]);
        }
      }
    }
  }
}

void showLineLed(int a, String line)
{
  for (int j = 0; j < 8; j++)
  {
    a;
    j;
    (line[j] == '1');
    lc.setLed(0, a, j, line[j] == '1');
  }
}
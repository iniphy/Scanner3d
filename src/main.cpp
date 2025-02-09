#include <Arduino.h>
#include "PCF8574.h"
#include "defines.h"
#include "global_vars.h"
#include "interrupts.h"
#include "control.h"

volatile struct motor motor;
volatile struct analogs analogs;
PCF8574 expander(0x39);
int zmienna;
int predkosc_max;
int predkosc_pocz;
int czas;
int droga;
int dystans;
int adc;
unsigned long Time_start;
unsigned long Time_current;
unsigned long Time_diff;
unsigned long t_limit;
byte command = 0;
byte stan = 0;
const byte CHECK_IF_READY_TO_WORK = 5;
const byte SCAN_START = 20;
const byte SCAN_STOP = 21;
const byte RESPONSE_OK = 30;
const byte RESPONSE_ERROR = 31;
const byte MEASURE_BATTERY = 41;
const byte SCANNING = 1;
const byte NOT_SCANNING = 2;

void send_command(byte cmd) {
    Serial.write(cmd);
    Serial.flush();
}

void send_float(float value) {
  union cvt {
    float val;
    unsigned char b[4];
  } x;
  x.val= value;
  Serial.write(x.b, 4);
  Serial.flush();
}

void setup()
{

  Serial.begin(9600);
  // TODO: here, we have messages which are not handled on PhysioServer-side, thus, we need to supress them for the moment.
  // Serial.println("Communication init baudrate 9600");

  init_GPIO();
  expander.begin(0);

  cli();
  int0_init();
  int1_init();
  timer1_init();
  timer2_init();
  sei();
  init_motor(LIMIT77, SLOW);

  zmienna = 1;
  predkosc_pocz = 30;
  predkosc_max = 80;
  droga = motor.turns_x10;
  dystans = 530;
  t_limit = 10000; // timeout na skanerze
}

void loop()
{
  if (Serial.available() > 0)
  {
    command = Serial.read();
    if (command == MEASURE_BATTERY) {
      send_command(RESPONSE_OK);
      adc = analogRead(A0);
      float battery_voltage = float(adc) * 5.0 / 1024.0 * 8.02;
      send_float(battery_voltage);
    }
    else if (command == SCAN_START)
    {
      send_command(RESPONSE_OK);
      delay(100);
      // Serial.println("OK");
      Time_start = millis();
      stan = SCANNING;
      for (int i = 20; i <= predkosc_max; i++)
      {
        set_motor_speed(i, RIGHT); // max speed set_motor_speed(100, RIGHT)
        delay(20);
      }
    } else if (command == CHECK_IF_READY_TO_WORK)
    {
      send_command(RESPONSE_OK);
    }
    
  }
  if (stan == SCANNING)
  {
    Time_current = millis();
    Time_diff = Time_current - Time_start;
  }

  if ((motor.turns_x10 / 10 > dystans) || ( Time_diff>t_limit))
  {
    // Serial.println("Wystarczy");
    for (int i = predkosc_max; i >= 0; i--)
    {
      set_motor_speed(i, RIGHT);
      delay(20);
      expander.write8(analogs.val_to_leds);
    }
    send_command(SCAN_STOP);
    stan = NOT_SCANNING;
    Time_diff = 0;
    Time_current = 0;
    Time_start = 0;
    deinit_motor();
    init_motor(LIMIT77, SLOW);
  }
}

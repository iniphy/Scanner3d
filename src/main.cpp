#include "PCF8574.h"
#include "control.h"
#include "defines.h"
#include "global_vars.h"
#include "interrupts.h"
#include <Arduino.h>

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

void send_response(byte cmd) {
  Serial.write(cmd);
  Serial.flush();
}

void send_float(float value) {
  union cvt {
    float val;
    unsigned char b[4];
  } x;
  x.val = value;
  Serial.write(x.b, 4);
  Serial.flush();
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
      // https://www.arduino.cc/reference/en/language/functions/communication/serial/ifserial/
  }

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
  dystans = 22000;
  t_limit = 10000; // timeout na skanerze
}

void loop() {
  // Performing scanning exludes any communication
  if (stan == SCANNING) {
    Time_current = millis();
    Time_diff = Time_current - Time_start;

    if (motor.enk1_cnt + motor.enk2_cnt >= dystans || Time_diff >= t_limit) {
      // Deccelerate the motor!
      for (int i = predkosc_max; i >= 0; i--) {
        set_motor_speed(i, RIGHT);
        delay(20);
        expander.write8(analogs.val_to_leds);
      }
      motor.enk1_cnt = 0;
      motor.enk2_cnt = 0;
      // send_response(SCAN_STOP);
      Time_diff = 0;
      Time_current = 0;
      Time_start = 0;
      deinit_motor();
      init_motor(LIMIT77, SLOW);
      stan = NOT_SCANNING;
    }
  } else if (Serial.available() > 0) {
    command = Serial.read();
    if (command == CHECK_IF_READY_TO_WORK) {
      if (stan == NOT_SCANNING) {
        send_response(RESPONSE_OK);
      } else {
        send_response(RESPONSE_ERROR);
      }
    } else if (command == MEASURE_BATTERY) {
      adc = analogRead(A0);
      float battery_voltage = float(adc) * 5.0 / 1024.0 * 8.02;
      if (battery_voltage >= 0.0 and battery_voltage <= 50.0) {
        send_response(RESPONSE_OK);
        send_float(battery_voltage);
      } else {
        send_response(RESPONSE_ERROR);
      }
    } else if (command == SCAN_START) {
      stan = SCANNING;
      send_response(RESPONSE_OK);
      delay(100);
      Time_start = millis();
      motor.enk1_cnt = 0;
      motor.enk2_cnt = 0;
      // Accelerate the motor!
      for (int i = 20; i <= predkosc_max; i++) {
        set_motor_speed(i, RIGHT); // max speed set_motor_speed(100, RIGHT)
        delay(20);
      }
    }
  }
}

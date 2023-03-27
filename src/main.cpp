#include <ArduinoRS485.h>
#include <ArduinoModbus.h>

#define DOOR_SENSOR_PIN 8
#define EXT_BUTTON_PIN 9
#define INT_BUTTON_PIN 10
#define INT_BUTTON_RED_LED_PIN 5
#define INT_BUTTON_GREEN_LED_PIN 6

#define DOOR_COIL_ADRESS 0x00
#define EXT_BUTTON_COIL_ADRESS 0x01
#define INT_BUTTON_COIL_ADRESS 0x02
#define INT_BUTTON_RED_LED_COIL_ADRESS 0x03
#define INT_BUTTON_GREEN_LED_COIL_ADRESS 0x04

#define BAUD_RATE 115200
#define SERIAL_MODE SERIAL_8N1
#define SLAVE_ID 42

const int numCoils = 3;
const int numHoldingRegisters = 2;


void setup() {
  pinMode(DOOR_SENSOR_PIN, INPUT_PULLUP);
  pinMode(EXT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(INT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(INT_BUTTON_GREEN_LED_PIN, OUTPUT);
  pinMode(INT_BUTTON_RED_LED_PIN, OUTPUT);

  if (!ModbusRTUServer.begin(SLAVE_ID, BAUD_RATE, SERIAL_MODE)) {
    while (1);
  }

  // configure coils at address 0x00
  ModbusRTUServer.configureCoils(0x00, numCoils);

  // configure holding registers at address 0x03
  ModbusRTUServer.configureHoldingRegisters(0x03, numHoldingRegisters);

  ModbusRTUServer.coilWrite(DOOR_COIL_ADRESS, 0);
  ModbusRTUServer.coilWrite(EXT_BUTTON_COIL_ADRESS, 1);
  ModbusRTUServer.coilWrite(INT_BUTTON_COIL_ADRESS, 0);
}



void loop() {

  int door_sensor= !digitalRead(DOOR_SENSOR_PIN);
  int ext_button = !digitalRead(EXT_BUTTON_PIN);
  int int_button = !digitalRead(INT_BUTTON_PIN);
  
  // poll for Modbus RTU requests
  ModbusRTUServer.poll();

  ModbusRTUServer.coilWrite(DOOR_COIL_ADRESS, door_sensor);
  ModbusRTUServer.coilWrite(EXT_BUTTON_COIL_ADRESS, ext_button);
  ModbusRTUServer.coilWrite(INT_BUTTON_COIL_ADRESS, int_button);

  long green_led_value = ModbusRTUServer.holdingRegisterRead(INT_BUTTON_GREEN_LED_COIL_ADRESS);
  analogWrite(INT_BUTTON_GREEN_LED_PIN, green_led_value & 0xFF);

  long red_led_value = ModbusRTUServer.holdingRegisterRead(INT_BUTTON_RED_LED_COIL_ADRESS);
  analogWrite(INT_BUTTON_RED_LED_PIN, red_led_value & 0xFF);
}
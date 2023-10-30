
#include <SPI.h>
#include <STBLE.h>

#include <Wire.h>
#include <SPI.h>
#include <TinyScreen.h>


TinyScreen display = TinyScreen(TinyScreenDefault);

#if defined (ARDUINO_ARCH_AVR)
#define SerialMonitorInterface Serial
#elif defined(ARDUINO_ARCH_SAMD)
#define SerialMonitorInterface SerialUSB
#endif


uint8_t ble_rx_buffer[21];
uint8_t ble_rx_buffer_len = 0;
uint8_t ble_connection_state = false;
#define PIPE_UART_OVER_BTLE_UART_TX_TX 0


void setup() {
  bluetoothsetup();
}

void loop() {
  bluetoothloop();
}

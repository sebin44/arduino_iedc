#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
// LoRaWAN NwkSKey, network session key
static const PROGMEM u1_t NWKSKEY[16] = { 0xEE, 0xE0, 0x27, 0xC6, 0x08, 0x67, 0x36, 0x9B, 0x39, 0x7D, 0x45, 0x75, 0x46, 0xEB, 0xFC, 0x80 };
// LoRaWAN AppSKey, application session key
static const u1_t PROGMEM APPSKEY[16] = { 0xEB, 0x88, 0x8D, 0xC9, 0x29, 0x16, 0xEB, 0xC2, 0x21, 0x23, 0x3B, 0x66, 0x39, 0x81, 0xB5, 0x32 };
// LoRaWAN end-device address (DevAddr)
static const u4_t DEVADDR = { 0x260B26AE };
// These callbacks are only used in over-the-air activation, so they are
// left empty here (we cannot leave them out completely unless
// DISABLE_JOIN is set in config.h, otherwise the linker will complain).
void os_getArtEui (u1_t* buf) { }
void os_getDevEui (u1_t* buf) { }
void os_getDevKey (u1_t* buf) { }
static uint8_t mydata[] = "HI,Lora World!";
static osjob_t sendjob;
// Schedule data trasmission in every this many seconds (might become longer due to duty
// cycle limitations).
// we set 10 seconds interval
const unsigned TX_INTERVAL = 10;
// Pin mapping according to Cytron LoRa Shield RFM
const lmic_pinmap lmic_pins = {
 .nss = 10,
 .rxtx = LMIC_UNUSED_PIN,
 .rst = 7,
 .dio = {2, 5, 6},
};
void onEvent (ev_t ev) {
 Serial.print(os_getTime());
 Serial.print(": ");
 switch(ev) {
 case EV_TXCOMPLETE:
 Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
 // Schedule next transmission
os_setTimedCallback(&sendjob, os_getTime()
+sec2osticks(TX_INTERVAL), do_send);
 break;
 default:
 Serial.println(F("Unknown event"));
break;
 }
}
void do_send(osjob_t* j){
 // Check if there is not a current TX/RX job running
 if (LMIC.opmode & OP_TXRXPEND) {
 Serial.println(F("OP_TXRXPEND, not sending"));
 } else {
 // Prepare upstream data transmission at the next possible time.
 LMIC_setTxData2(1, mydata, sizeof(mydata)-1, 0);
 Serial.println(F("Packet queued"));
 }
 // Next TX is scheduled after TX_COMPLETE event.
}
void setup() {
  Serial.begin(115200);
  Serial.println(F("Starting"));
 // LMIC init
 os_init();
 // Reset the MAC state. Session and pending data transfers will be discarded.
 LMIC_reset();
 // Set static session parameters. Instead of dynamically establishing a session
 // by joining the network, precomputed session parameters are be provided.
 uint8_t appskey[sizeof(APPSKEY)];
 uint8_t nwkskey[sizeof(NWKSKEY)];
 memcpy_P(appskey, APPSKEY, sizeof(APPSKEY));
 memcpy_P(nwkskey, NWKSKEY, sizeof(NWKSKEY));
 LMIC_setSession (0x1, DEVADDR, nwkskey, appskey);
 LMIC_selectSubBand(3);

 // Disable link check validation
 LMIC_setLinkCheckMode(0);
 // Disable ADR
 LMIC_setAdrMode(false);
 // TTN uses SF9 for its RX2 window.
 LMIC.dn2Dr = DR_SF9;
 // Set data rate and transmit power for uplink (note: txpow seems to be ignored by the library)
 LMIC_setDrTxpow(DR_SF7,14);
 // Start job
 do_send(&sendjob);
}
void loop() {
 os_runloop_once();
}

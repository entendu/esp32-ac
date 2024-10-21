/* esp32 AC Controller
 * 
 * Idea based on the work of Wolf Butler - https://lunardenlights.com/2021/02/frankenpower-mk-iii/
 * E131 Based on the library & example by @forkineye - https://github.com/forkineye/ESPAsyncE131
 * 
 * See README.md
 */

#include <WiFi.h>
#include <ESPAsyncE131.h>
#include <arduino_secrets.h> // WiFi credentials

#define UNIVERSE 1        // First DMX Universe to listen for.
#define UNIVERSE_COUNT 2  // Total number of Universes to listen for, starting at UNIVERSE.

// Wifi setup
const char ssid[] = WIFI_SSID;
const char passphrase[] = WIFI_PASSWORD;

// Channels map to output pins. Reference the pinout diagram for your board.
int channels[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
int outputs[] = { 14, 33, 25, 26, 27, 4, 5, 15 };

// ESPAsyncE131 instance with UNIVERSE_COUNT buffer slots.
ESPAsyncE131 e131(UNIVERSE_COUNT);

// DEBUG: for tracking state changes (see loop()).
// int last_state[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

void setup() {
  Serial.begin(115200);
  delay(10);

  // Make sure you're in station mode
  WiFi.mode(WIFI_STA);

  // Disable wifi sleep to preserve performance - see https://github.com/espressif/arduino-esp32/issues/1484
  WiFi.setSleep(WIFI_PS_NONE);

  Serial.println("");
  Serial.print(F("Connecting to "));
  Serial.print(ssid);

  if (passphrase != NULL)
    WiFi.begin(ssid, passphrase);
  else
    WiFi.begin(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print(F("Connected with IP: "));
  Serial.println(WiFi.localIP());

  if (e131.begin(E131_UNICAST))  // Listen via Unicast. For multicast, use `e131.begin(E131_MULTICAST, UNIVERSE, UNIVERSE_COUNT)`
    Serial.println(F("Listening for data..."));
  else
    Serial.println(F("*** e131.begin failed ***"));

  //Initialize the GPIOs and set the relays to off.
  for (int i = 0; i < 8; i++) {
    pinMode(outputs[i], OUTPUT);
    digitalWrite(outputs[i], LOW);
  }
}

void loop() {
  // Parse a packet
  if (!e131.isEmpty()) {
    e131_packet_t packet;
    e131.pull(&packet);  // Pull packet from ring buffer.

    for (int i = 0; i < 8; i++) {
      // DEBUG: only respond to changed values.
      //if (packet.property_values[i] != last_state[i-1]) {
      // Serial.printf("State change! CH%u was %u, now %u: ", i, last_state[i-1], packet.property_values[i]);
      // Serial.printf("Universe %u / %u Channels | Packet#: %u / Errors: %u / CH%u: %u\n",
      //               htons(packet.universe),                  // The Universe for this packet
      //               htons(packet.property_value_count) - 1,  // Start code is ignored, we're interested in dimmer data
      //               e131.stats.num_packets,                  // Packet counter
      //               e131.stats.packet_errors,                // Packet error counter
      //               i,
      //               packet.property_values[channels[i]]);  // Dimmer data for Channel 1
      // last_state[i-1] = packet.property_values[i];

      //Any DMX value under 127 is "Off", above 127 is "On".
      if (packet.property_values[channels[i]] > 127) {
        digitalWrite(outputs[i], HIGH); // SSR on
      } else {
        digitalWrite(outputs[i], LOW);  // SSR off
      }
      //}
    }
  }
}
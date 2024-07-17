
#include <RC_sbus.Easy.h>
void setup() { Serial.begin(115200); }

TangoII tangoII(&Serial1);
void loop() {
  if (tangoII.Read()) {
    Serial.print(tangoII.getThrottle(-UINT8_MAX, UINT8_MAX));
    Serial.print('\t');
    Serial.print(tangoII.getRudder());
    Serial.print('\t');
    Serial.print(tangoII.getElevator());
    Serial.print('\t');
    Serial.print(tangoII.getAlieron());
    Serial.print('\n');
  }
}

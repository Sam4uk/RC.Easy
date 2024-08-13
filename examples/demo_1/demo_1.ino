/**
 * @file demo_1.ino
 * @author Sam4uk
 * @brief
 * @version 0.1
 * @date 2024-08-13
 *
 * @copyright Sam4uk (c) 2024
 *
 */

#include <RC_sbus.Easy.h>

TangoII tangoII(&Serial1);

void setup() {
  Serial.begin(115200);

  tangoII.setMaping(TangoII::THROTTLE, TangoII::Chnl_1);
  tangoII.setMaping(TangoII::ALIERON, TangoII::Chnl_2);
  tangoII.setMaping(TangoII::ELEVATOR, TangoII::Chnl_3);
  tangoII.setMaping(TangoII::RUDDER, TangoII::Chnl_4);

  tangoII.setMaping(TangoII::SwA, TangoII::Chnl_5);
  tangoII.setMaping(TangoII::SwB, TangoII::Chnl_6);
  tangoII.setMaping(TangoII::SwC, TangoII::Chnl_7);
  tangoII.setMaping(TangoII::SwD, TangoII::Chnl_8);
  tangoII.setMaping(TangoII::SwE, TangoII::Chnl_9);
  tangoII.setMaping(TangoII::SwF, TangoII::Chnl_10);
}

void loop() {
  if (tangoII.Read()) {
    Serial.print(tangoII.getThrottle(-UINT8_MAX, UINT8_MAX));
    Serial.print('\t');
    Serial.print(tangoII.getRudder(-100, 100));
    Serial.print('\t');
    Serial.print(tangoII.getElevator());
    Serial.print('\t');
    Serial.print(tangoII.getAlieron());
    Serial.print('\t');
    Serial.print(tangoII.getNumber(TangoII::SwA, 2));
    Serial.print('\t');
    Serial.print(tangoII.getNumber(TangoII::SwB, TangoII::SWITCH_3));
    Serial.print('\t');
    Serial.print(tangoII.getChannel(TangoII::SwC));
    Serial.print('\t');
    Serial.print(tangoII.getBool(TangoII::SwD));
    Serial.print('\n');
  }
}

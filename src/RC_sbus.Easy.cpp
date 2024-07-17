/**
 * @file RC_sbus.Easy.cpp
 * @author Sam4uk
 * @brief
 * @version 0.0.1
 * @date 2024-07-16
 *
 * @copyright Sam4uk (c) 2024
 *
 */
#include "RC_sbus.Easy.h"

RemoteControl::RemoteControl() = default;

RemoteControl::~RemoteControl() {
  if (nullptr != sbus_rx)
    delete sbus_rx;
  if (nullptr != sbus_tx)
    delete sbus_tx;
};

namespace {
const signed                  //
    SkyDroidMinValue{1050},   ///<
    SkyDroidMaxValue{1950},   ///<
    SkyDroidSBUSmin{282},     ///<
    SkyDroidSBUSmax{1722},    ///<
    RadioMasterSBUSmin{172},  ///<
    RadioMasterSBUSmax{1810}; ///<
}

inline signed calculatePWM(signed value) {
  return map(value, SkyDroidMinValue, SkyDroidMaxValue, SkyDroidSBUSmin,
             SkyDroidSBUSmax);
}

void RemoteControl::setPWMminValue(signed value) {
  _min_value = calculatePWM(value);
}

void RemoteControl::setPWMmaxValue(signed value) {
  _max_value = calculatePWM(value);
}

void RemoteControl::setSignalMinValue(signed value) { _min_value = value; }

void RemoteControl::setSignalMaxValue(signed value) { _max_value = value; }

signed RemoteControl::getSignalMinValue() const { return _min_value; };

signed RemoteControl::getSignalMaxValue() const { return _max_value; };

signed RemoteControl::getSignalMedValue() const {
  return (_min_value + _max_value) / 2;
};

#if 0
  signed RemoteControl::getSignalTrirdMinValue() const{ ///< Мінімальна "третинка"
    return (_min_value + _max_value) / 3;
  };
  signed RemoteControl:: getSignalTrirdMaxValue()const { ///< Максимальна "третинка"
    return (_min_value + _max_value) / 3 * 2;
  };
#endif

bool RemoteControl::Read() {
  bool result{sbus_rx->Read()};
  if (result) {
    _data = sbus_rx->data();
// #define RC_DEBUG_MSG
#if defined(RC_DEBUG_MSG)

    for (int8_t i{}; i < _data.NUM_CH; i++) {
      Serial.print(_data.ch[i]);
      Serial.print("\t");
    }
    Serial.print(_data.lost_frame);
    Serial.print("\t");
    Serial.println(_data.failsafe);
#endif
  }
  return result;
};

bool RemoteControl::lost_frame() const { return _data.lost_frame; };

bool RemoteControl::failsafe() const { return _data.failsafe; };

bool RemoteControl::ch17() const { return _data.ch17; };

bool RemoteControl::ch18() const { return _data.ch18; };

bool RemoteControl::Connected() const {
  bool result{true};
  for (uint16_t i{}; i < _data.NUM_CH; i++) {
    // result = result && _data.ch[i];
    if (_data.ch[i] < 10)
      result = false;
    Serial.print(_data.ch[i]);
    Serial.print(" ");
    // if (1 <= _data.ch[i])
    //   return true;
  }
  // Serial.println(" ");

  return result;
};

uint16_t RemoteControl::getChannel(uint8_t channel) const {
  return _data.ch[_indexes[channel]];
};

signed RemoteControl::getThrottle(signed min_value, signed max_value) const {
  // Serial.println(__LINE__);
  return getValue(THROTTLE, min_value, max_value);
}

signed RemoteControl::getAlieron(signed min_value, signed max_value) const {
  return getValue(ALIERON, min_value, max_value);
};

signed RemoteControl::getElevator(signed min_value, signed max_value) const {
  return getValue(ELEVATOR, min_value, max_value);
};

signed RemoteControl::getRudder(signed min_value, signed max_value) const {
  return getValue(RUDDER, min_value, max_value);
};

inline bool RemoteControl::getBool(uint8_t channel) const {
  return getSignalMedValue() < _data.ch[_indexes[channel]];
};

int8_t RemoteControl::getNumber(int8_t channel, int8_t number) const {
  switch (number) {
  case INT8_MIN ...(-1):
    // Відсікаємо відємні числа
    return 00;
    break;

  case 00 ... 01:
    // Якщо кількість відрізків нуль або один повертаємо значення
    // типу якщо нуль то завжди вимкнено
    // якщо один то завжди увімкнено
    return number;
    break;

  case 02:
    // Якщо кількість відрізків два то це перемикач булевий
    return getBool(channel);
    break;

  default:
    // інакше
    const int16_t _step{(_max_value - _min_value) / number};
    for (int8_t it{number}; it >= 01; --it) {
      if (((it * _step) + _min_value) < _data.ch[_indexes[channel]])
        return it;
    }
    break;
  }
  return 00;
};

signed RemoteControl::getValue(int8_t channel, signed min_value,
                               signed max_value) const {
  return map(_data.ch[_indexes[channel]], _min_value, _max_value, min_value,
             max_value);
};

inline void RemoteControl::setMaping(uint8_t Channel, uint8_t value) {
  _indexes[value] = Channel;
};

CustomRemoteControl::CustomRemoteControl(HardwareSerial *bus, signed min,
                                         signed max) {
  setSignalMinValue(min);
  setSignalMaxValue(max);
  sbus_rx = new bfs::SbusRx(bus);
  sbus_tx = new bfs::SbusTx(bus);
  sbus_rx->Begin();
  sbus_tx->Begin();
};

RadioMaster::RadioMaster(HardwareSerial *bus)
    : CustomRemoteControl(bus, RadioMasterSBUSmin, RadioMasterSBUSmax){};

TangoII::TangoII(HardwareSerial *bus)
    : CustomRemoteControl(bus, RadioMasterSBUSmin, RadioMasterSBUSmax){};

SkyDroid::SkyDroid(HardwareSerial *bus)
    : CustomRemoteControl(bus, SkyDroidSBUSmin, SkyDroidSBUSmax){};

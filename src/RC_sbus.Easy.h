/**
 * @file RC_sbus.Easy.h
 * @author Sam4uk (you@domain.com)
 * @brief
 * @version 0.0.1
 * @date 2024-07-16
 *
 * @copyright Sam4uk (c) 2024
 *
 */
#if !defined(_RC_SBUS_HPP_)

#define _RC_SBUS_HPP_
#include <Tools.Easy.h>
#include <assert.h>
#include <sbus.h>

class RemoteControl {
public:
  enum { ERTA, TREA, EATR, TAER };

private:
  bfs::SbusData //
      _data;    ///<

public:
  enum {
    // Лімби
    THROTTLE = 0, ///< Другий джойстик
    ALIERON,      ///< Другий джойстик
    ELEVATOR,     ///< Перший джойстик
    RUDDER,       ///< Перший джойстик
  };

  enum //
  {
    DISABLED, ///< З каналу завжди читатиметься вимкнено
    ALWAYS_ON, ///< З каналу завжди ситатиметьтся увімкнено
    TOGGLE, ///< Каналом керує двох позиціний перемикач ON/OFF
    SWITCH_3, /// Каналом керує трьох позиціний перемикач
    SWITCH_6 = 06 ///< каналом керує 6 позиціний перемикач
  };
  /// @brief
  RemoteControl();

  /// @brief
  ~RemoteControl();

  /// @brief
  /// @param value
  void setPWMminValue(signed value);

  /// @brief
  /// @param value
  void setPWMmaxValue(signed value);

  /// @brief
  /// @param value
  void setSignalMinValue(signed value);

  /// @brief
  /// @param value
  void setSignalMaxValue(signed value);

  /// @brief
  /// @return
  signed getSignalMinValue() const;

  /// @brief
  /// @return
  signed getSignalMaxValue() const;

  /// @brief
  /// @return
  signed getSignalMedValue() const;
#if 0
  signed getSignalTrirdMinValue() const; ///< Мінімальна "третинка"
  signed getSignalTrirdMaxValue() const; ///< Максимальна "третинка"
#endif
  enum ///< Визначення деяких контсантних знасень
  {
    Chnl_1,  ///< Перший канал
    Chnl_2,  ///< Другий канал
    Chnl_3,  ///< Третій канал
    Chnl_4,  ///< Четвертий канал
    Chnl_5,  ///< П'ятий канал
    Chnl_6,  ///< Шостий канал
    Chnl_7,  ///< Сьомий канал
    Chnl_8,  ///< Восьмий канал
    Chnl_9,  ///< Дев'ятийканал
    Chnl_10, ///< Десятий канал
    Chnl_11, ///< Одинадцятий канал
    Chnl_12, ///< Дванадцятий канал
    Chnl_13, ///< Тринадцятий канал
    Chnl_14, ///< Чотирнадцятий канал
    Chnl_15, ///< П'ятнацятий канал
    Chnl_16, ///< Шінснадцятий канал

    MAX_CAN_NUM ///< Кількість каналів канал
  };

  static_assert(MAX_CAN_NUM == (bfs::SbusData::NUM_CH),
                "Перевіримо кількість каналів під час компіляції");

  //  protected:

  uint8_t                 //
      *_indexes{nullptr}; ///<

  bfs::SbusRx *sbus_rx;
  bfs::SbusTx *sbus_tx;

  signed                           //
      _min_value{},                ///<
      _max_value{maxBitValue(11)}; ///<

public:
  /// @brief
  /// @return
  bool Read();

  /// @brief Перевірка на втрачені кадри
  /// @return Істина якщо було втрачено фрейми команд
  bool lost_frame() const;

  /// @brief
  /// @return
  bool failsafe() const;

  /// @brief
  /// @return
  bool ch17() const;

  /// @brief
  /// @return
  bool ch18() const;

  /// @brief
  /// @return
  bool Connected() const;

  /// @brief Отримати команду з каналу
  /// @param channel номер каналу з якого ситаються значення
  /// @return Значення
  uint16_t getChannel(uint8_t channel) const;

  /// @brief
  /// @return
  signed getThrottle(signed min_value = -100, signed max_value = 100) const;

  /// @brief
  /// @return
  signed getAlieron(signed min_value = -100, signed max_value = 100) const;

  /// @brief
  /// @return
  signed getElevator(signed min_value = -100, signed max_value = 100) const;

  /// @brief
  /// @return
  signed getRudder(signed min_value = -100, signed max_value = 100) const;

  /// @brief
  /// @param channel
  /// @return
  inline bool getBool(uint8_t channel) const;

  /// @brief Повертає номер позиції за кількістю відрізків
  /// @param channel Номер каналу з якого читаєьбся
  /// @param number кількість відрізків
  /// @return
  int8_t getNumber(int8_t channel, int8_t number) const;

  signed getValue(int8_t channel, signed min_value = -100,
                  signed max_value = 100) const;

  void setMaping(short value, short Channel);
};

class CustomRemoteControl : public RemoteControl {
public:
  enum RemoteControlID { UNKNOWN, RADIOMASTER, TANGOII, SKYDROID, CUSTOM };
  virtual uint8_t getRCID() { return UNKNOWN; };
  CustomRemoteControl(HardwareSerial *bus, signed min, signed max,
                      uint8_t nums);
};

class RadioMaster : public CustomRemoteControl {
public:
  enum {
    // Лімби
    THROTTLE = Chnl_1, ///< Другий джойстик
    ALIERON,           ///< Другий джойстик
    ELEVATOR,          ///< Перший джойстик
    RUDDER,            ///< Перший джойстик

    // перемикачі
    SwA, ///< 3POT
    SwB, ///< 3POT
    SwC, ///< 3POT
    SwD, ///< 3POT
    SwE, ///< 3POT
    SwF, ///< 2POT
    SwG, ///< 3POT
    SwH, ///< 1POT

    // перемикач
    BUTTONS, ///< 6POT

    // Слайдери
    LSl, ///<
    RSl, ///<

    // регулятори
    S1, ///<
    S2, ///<
    COUNT
  };
  uint8_t getRCID() { return RADIOMASTER; };
  RadioMaster(HardwareSerial *bus);
};

////////////////////////////////////////////////////////////////////////////////
////   Класс Пульта Tango
////////////////////////////////////////////////////////////////////////////////
/// @brief
class TangoII : public CustomRemoteControl {
public:
  enum {
    // Лімби
    THROTTLE = Chnl_1, ///< Другий джойстик
    ALIERON,           ///< Другий джойстик
    ELEVATOR,          ///< Перший джойстик
    RUDDER,            ///< Перший джойстик

    // перемикачі
    SwA, ///<
    SwB, ///<
    SwC, ///<
    SwD, ///<
    SwE, ///<
    SwF, ///<
    COUNT
  };
  /// @brief
  /// @param bus
  uint8_t getRCID() { return TANGOII; };
  TangoII(HardwareSerial *bus);
};

class SkyDroid : public CustomRemoteControl {
public:
  enum //
  {
    // Лімби
    Y1 = Chnl_1, ///< Перший джойстик
    X1,          ///< Перший джойстик
    Y2,          ///< Другий джойстик
    X2,          ///< Другий джойстик

    // перемикачі
    Sw1,  ///< 3POT
    Sw2,  ///< 3POT
    Sw3,  ///< 3POT
    Sw4,  ///< 3POT
    A_F,  ///
    O,    //
    AUX1, //
    AUX2, //
    X3,
    Y3,   //
    PPM1, //
    PPM2,
    COUNT
  };
  uint8_t getRCID() { return SKYDROID; };
  SkyDroid(HardwareSerial *bus);
};

#endif
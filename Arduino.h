#ifndef ARDUINO_H
#define ARDUINO_H
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QDebug>


class Arduino
{
public:
  static void onLed();
  static void create();
  int num;
  static void readSerial();

private:
  static Arduino* ptrInstance;
  static Arduino& Instance(){
    if(!ptrInstance){
        ptrInstance = new Arduino();
      }
    return *ptrInstance;
  }
  ~Arduino();
  Arduino();

  QSerialPort *arduino;
  static const quint16 arduinoUnoVendorID = 9025;
  static const quint16 arduinoUnoProductID = 67;
  QString arduinoPortName;
  bool arduinoIsAvailable;
  void privOnLed();
};

#endif // ARDUINO_H

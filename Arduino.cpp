#include "Arduino.h"
#include <QByteArray>

Arduino* Arduino::ptrInstance = nullptr;

Arduino::Arduino(){
  arduinoIsAvailable = false;
  arduinoPortName = "";
  arduino = new QSerialPort;


  qDebug() << "Number of available ports: " << QSerialPortInfo::availablePorts().length();
  foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
      qDebug() << "Has vendor ID: " << serialPortInfo.hasVendorIdentifier();
      if(serialPortInfo.hasVendorIdentifier()){
          qDebug() << "Vendor ID: " << serialPortInfo.vendorIdentifier();
      }
      qDebug() << "Has Product ID: " << serialPortInfo.hasProductIdentifier();
      if(serialPortInfo.hasProductIdentifier()){
          qDebug() << "Product ID: " << serialPortInfo.productIdentifier();
      }
  }


  foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
      if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()){
          if(serialPortInfo.vendorIdentifier() == arduinoUnoVendorID){
              if(serialPortInfo.productIdentifier() == arduinoUnoProductID){
                  arduinoPortName = serialPortInfo.portName();
                  arduinoIsAvailable = true;
              }
          }
      }
  }

  if(arduinoIsAvailable){
      // open and configure the serialport
      arduino->setPortName(arduinoPortName);
      arduino->open(QSerialPort::ReadWrite);
      arduino->setBaudRate(QSerialPort::Baud9600);
      arduino->setDataBits(QSerialPort::Data8);
      arduino->setParity(QSerialPort::NoParity);
      arduino->setStopBits(QSerialPort::OneStop);
      arduino->setFlowControl(QSerialPort::NoFlowControl);
    }else{
      // give error message if not available
      qDebug() << "Port error", "Couldn't find the Arduino!";
  }
}

//Send a signal to turn on a led in protoboard
void Arduino::privOnLed(){

  if(arduino){
        arduino->write("255");
    }else{
        qDebug() << "Couldn't write to serial!";
    }

}
void Arduino::onLed(){
  Instance().privOnLed();
}

void Arduino::create(){
  Instance();
}
void Arduino::readSerial(){

}


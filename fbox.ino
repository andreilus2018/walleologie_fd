//подключение необходимых библиотек
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint64_t pipe = 0xF0F1F2F3F4LL; //труба передачи данных
RF24 radio_module(9,10);  //CE, CSN

#define pinMosfet 5 // пин с Mosfet-транзистором мотора
int data[2]; //массив для приема данных
int h_m = 0; //высота полёта

void setup() {
    pinMode(pinMosfet, OUTPUT);

    radio_module.begin();                   //включение радиомодуля
    Delay(7);                               //задержка
    radio_module.setChannel(7);             //установка канала
    radio_module.setDataRate(RF24_1MBPS);   //скорость передачи
    radio_module.setPALLevel(RF24_PA_HIGH); //мощность радиомодуля
    radio_module.openReadingPipe(pipe);     //открытие прослушивания
    radio_module.startListening();          //Начало прослушивания
}


void loop(){
    if(radio_module.available()) {                   //если радиосигнал доступен
        radio_module.read(&data, sizeof(data));      //приём сигнала
    }
    
}
//подключение необходимых библиотек
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint64_t pipe = 0xF0F1F2F3F4LL; //труба передачи данных
RF24 radio_module(9,10);  //CE, CSN

#define turn_on_led 12 //светодиод, загорающийся при включении
#define joystick_x 4   //пин с координатой x джойстика
#define joystick_y 5   //пин с координатой y джойстика
#define joystick_z 6   //пин с кнопкой джойстика (повышает высоту полета при удержании)
#define down_button 7  //пин с кнопкой для понижения высоты полета при удержании

bool was_jbutton_prssd = false; //была ли кнопка джойстика нажата
long jbutton_prssd_time;        //когда кнопка джойстика была нажата

bool was_dbutton_prssd = false; //была ли кнопка понижения высоты полета нажата
long dbutton_prssd_time;        //когда кнопка понижения высоты полета была нажата
byte motor_m = 0;               //мощность мотора

void setup() {
    pinMode(turn_on_led, OUTPUT);  //установка режима выхода на пин со светодиодом
    pinMode(joystick_x, OUTPUT);   //установка режима выхода на пин с координатами x джойстика
    pinMode(joystick_y, OUTPUT);   //установка режима выхода на пин с координатами y джойстика
    pinMode(joystick_z, OUTPUT);   //установка режима выхода на пин с кнопкой джойстика

    radio_module.begin();                    //включение радиомодуля
    Delay(7);                                //задержка
    radio_module.setChannel(7);              //установка канала
    radio_module.setDataRate(RF24_1MBPS);    //скорость передачи
    radio_module.setPALLevel(RF24_PA_HIGH);  //мощность радиомодуля
    radio_module.openWritingPipe(pipe);      //открытие радиоканала

    pinMode(turn_on_led, OUTPUT);    //установка режима выхода на пин со светодиодом
    digitalWrite(turn_on_led, HIGH); //включение светодиода
}

void loop() {
    if(digitalRead(joystick_z) && !was_jbutton_prssd) //если
    {
        jbutton_prssd_time = time();
        was_jbutton_prssd = true;
    }
    elseif (digitalRead(joystick_z) && was_jbutton_prssd)
    {
        was_jbutton_prssd = false;
    }


    if(digitalRead(down_button) && !was_dbutton_prssd)
    {
        dbutton_prssd_time = time();
        was_dbutton_prssd = true;
    }
    elseif (digitalRead(down_button) && was_dbutton_prssd)
    {
        was_dbutton_prssd = false;
    }

    motor_m = jbutton_prssd_time - dbutton_prssd_time;

    int data[2] = [
        analogRead(joystick_x), //значение координаты x джойстика
        analogRead(joystick_y), //значение координаты y джойстика
        motor_m                 //мощность мотора на л. аппарате
    ];
    radio_module.write(&data,typeof(data)); //отправка массива data
    Delay(30);
}
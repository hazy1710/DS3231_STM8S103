#include "i2c.h"
#include <intrinsics.h>
#include <iostm8s103f3.h>
#include <stm8s_type.h>

//Таймаут ожидания события I2C
static unsigned long int i2c_timeout;

//Задать таймаут в микросекундах
#define set_tmo_us(time)  i2c_timeout = (unsigned long int)(F_MASTER_MHZ * time)

//Задать таймаут в миллисекундах
#define set_tmo_ms(time)  i2c_timeout = (unsigned long int)(F_MASTER_MHZ * time * 1000)

#define tmo               i2c_timeout--

//Ожидание наступления события event
//в течении времени timeout в мс
#define wait_event(event, timeout) set_tmo_ms(timeout);\
                                   while(event && --i2c_timeout);\
                                   if(!i2c_timeout){ i2c_init(); return;}
                                   
                                   
//******************************************************************************
// Инициализация                            
//******************************************************************************                                   
void i2c_init(){
  unsigned long int ccr;
  
  I2C_CR2_SWRST=1;
  I2C_CR2_SWRST=0;
  //delay_ms(1000);
  
  //Настройка GPIO
  PB_DDR_bit.DDR4 = 0;
  PB_DDR_bit.DDR5 = 0;
  PB_ODR_bit.ODR5 = 1;  //SDA
  PB_ODR_bit.ODR4 = 1;  //SCL
  
  PB_CR1_bit.C14 = 0;
  PB_CR1_bit.C15 = 0;
  
  PB_CR2_bit.C24 = 0;
  PB_CR2_bit.C25 = 0;
  
  //Частота тактирования периферии MHz
  I2C_FREQR_FREQ = 16;
  //Отключаем I2C
  I2C_CR1_PE = 0;
  //Выбираем стандартный режим 
  I2C_CCRH_F_S = 0;
  //Для I2C=100000Гц
  //CCR = (1/100000гц)/(2*(1/16000000гц))=80
  ccr = 80;
  I2C_TRISER_TRISE = 16+1;
  I2C_CCRL = ccr & 0xFF;
  I2C_CCRH_CCR = (ccr >> 8) & 0x0F;
  //Включаем I2C
  I2C_CR1_PE = 1;
  //Разрешаем подтверждение в конце посылки
  I2C_CR2_ACK = 1;
}

//******************************************************************************
// Старт посылка
//******************************************************************************  
void i2c_start(){     
   
  wait_event(I2C_SR3_BUSY, 10);         //Ждем освобождения шины I2C
  I2C_CR2_START = 1;                    //Генерация СТАРТ-посылки
  wait_event(!I2C_SR1_SB, 1);           //Ждем установки бита SB
}

//******************************************************************************
// Стоп посылка
//******************************************************************************  
void i2c_stop(){     
  I2C_CR2_STOP = 1;                      //Посылаем СТОП-посылку
  wait_event(I2C_CR2_STOP, 1);          //Ждем выполнения условия СТОП
}

//******************************************************************************
// Адрес посылка
//******************************************************************************  
void i2c_send_adress(unsigned char address){     
  I2C_DR = address;       //Записываем в регистр данных адрес датчика 0x23 7bit
  wait_event(!I2C_SR1_ADDR, 1);         //Ждем подтверждения передачи адреса
  I2C_SR3;                              //Очистка бита ADDR чтением регистра SR3
}

//******************************************************************************
// Данные посылка
//******************************************************************************  
void i2c_send_data(unsigned char data){    
  wait_event(!I2C_SR1_TXE, 1);                  //Ждем освобождения регистра данных
  I2C_DR = data;                                //Отправляем данные
  wait_event(!(I2C_SR1_TXE && I2C_SR1_BTF), 1); //Ловим момент, когда DR освободился и данные попали в сдвиговый регистр
}

//******************************************************************************
// Данные прием
//******************************************************************************  
void i2c_get_data(u8 * data, u8 nak){
  if(nak) { I2C_CR2_ACK = 0;}
  wait_event(!I2C_SR1_RXNE, 1);
  *data= I2C_DR;                                //Получаем данные
}


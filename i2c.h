#include <iostm8s103f3.h>
#include <intrinsics.h>
#include <stm8s_type.h>

#define F_MASTER_MHZ    16UL
#define F_MASTER_HZ     16000000UL

// Инициализация шины                                
void i2c_init();// Инициализация    
void i2c_start(); // Старт посылка
void i2c_stop();// Стоп посылка
void i2c_send_adress(unsigned char address);// Адрес посылка
void i2c_send_data(unsigned char data);// Данные посылка
void i2c_get_data(u8 * data, u8 nak);// Данные прием
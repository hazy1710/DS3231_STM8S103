#include <iostm8s103f3.h>
#include <intrinsics.h>
#include <stm8s_type.h>

#define F_MASTER_MHZ    16UL
#define F_MASTER_HZ     16000000UL

// ������������� ����                                
void i2c_init();// �������������    
void i2c_start(); // ����� �������
void i2c_stop();// ���� �������
void i2c_send_adress(unsigned char address);// ����� �������
void i2c_send_data(unsigned char data);// ������ �������
void i2c_get_data(u8 * data, u8 nak);// ������ �����
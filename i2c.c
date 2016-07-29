#include "i2c.h"
#include <intrinsics.h>
#include <iostm8s103f3.h>
#include <stm8s_type.h>

//������� �������� ������� I2C
static unsigned long int i2c_timeout;

//������ ������� � �������������
#define set_tmo_us(time)  i2c_timeout = (unsigned long int)(F_MASTER_MHZ * time)

//������ ������� � �������������
#define set_tmo_ms(time)  i2c_timeout = (unsigned long int)(F_MASTER_MHZ * time * 1000)

#define tmo               i2c_timeout--

//�������� ����������� ������� event
//� ������� ������� timeout � ��
#define wait_event(event, timeout) set_tmo_ms(timeout);\
                                   while(event && --i2c_timeout);\
                                   if(!i2c_timeout){ i2c_init(); return;}
                                   
                                   
//******************************************************************************
// �������������                            
//******************************************************************************                                   
void i2c_init(){
  unsigned long int ccr;
  
  I2C_CR2_SWRST=1;
  I2C_CR2_SWRST=0;
  //delay_ms(1000);
  
  //��������� GPIO
  PB_DDR_bit.DDR4 = 0;
  PB_DDR_bit.DDR5 = 0;
  PB_ODR_bit.ODR5 = 1;  //SDA
  PB_ODR_bit.ODR4 = 1;  //SCL
  
  PB_CR1_bit.C14 = 0;
  PB_CR1_bit.C15 = 0;
  
  PB_CR2_bit.C24 = 0;
  PB_CR2_bit.C25 = 0;
  
  //������� ������������ ��������� MHz
  I2C_FREQR_FREQ = 16;
  //��������� I2C
  I2C_CR1_PE = 0;
  //�������� ����������� ����� 
  I2C_CCRH_F_S = 0;
  //��� I2C=100000��
  //CCR = (1/100000��)/(2*(1/16000000��))=80
  ccr = 80;
  I2C_TRISER_TRISE = 16+1;
  I2C_CCRL = ccr & 0xFF;
  I2C_CCRH_CCR = (ccr >> 8) & 0x0F;
  //�������� I2C
  I2C_CR1_PE = 1;
  //��������� ������������� � ����� �������
  I2C_CR2_ACK = 1;
}

//******************************************************************************
// ����� �������
//******************************************************************************  
void i2c_start(){     
   
  wait_event(I2C_SR3_BUSY, 10);         //���� ������������ ���� I2C
  I2C_CR2_START = 1;                    //��������� �����-�������
  wait_event(!I2C_SR1_SB, 1);           //���� ��������� ���� SB
}

//******************************************************************************
// ���� �������
//******************************************************************************  
void i2c_stop(){     
  I2C_CR2_STOP = 1;                      //�������� ����-�������
  wait_event(I2C_CR2_STOP, 1);          //���� ���������� ������� ����
}

//******************************************************************************
// ����� �������
//******************************************************************************  
void i2c_send_adress(unsigned char address){     
  I2C_DR = address;       //���������� � ������� ������ ����� ������� 0x23 7bit
  wait_event(!I2C_SR1_ADDR, 1);         //���� ������������� �������� ������
  I2C_SR3;                              //������� ���� ADDR ������� �������� SR3
}

//******************************************************************************
// ������ �������
//******************************************************************************  
void i2c_send_data(unsigned char data){    
  wait_event(!I2C_SR1_TXE, 1);                  //���� ������������ �������� ������
  I2C_DR = data;                                //���������� ������
  wait_event(!(I2C_SR1_TXE && I2C_SR1_BTF), 1); //����� ������, ����� DR ����������� � ������ ������ � ��������� �������
}

//******************************************************************************
// ������ �����
//******************************************************************************  
void i2c_get_data(u8 * data, u8 nak){
  if(nak) { I2C_CR2_ACK = 0;}
  wait_event(!I2C_SR1_RXNE, 1);
  *data= I2C_DR;                                //�������� ������
}


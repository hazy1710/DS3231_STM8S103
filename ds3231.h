#include <iostm8s103f3.h>
#include <intrinsics.h>
#include <stm8s_type.h>

// ������������� ����                                
void ds3231_init();// �������������    
void ds3231_read_time(u8 * data);// ������ �������
void ds3231_write_time(u8 * data);// ������ �������
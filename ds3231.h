#include <iostm8s103f3.h>
#include <intrinsics.h>
#include <stm8s_type.h>

// Инициализация шины                                
void ds3231_init();// Инициализация    
void ds3231_read_time(u8 * data);// Чтение времени
void ds3231_write_time(u8 * data);// Запись времени
#ifndef UTILS_H_
#define UTILS_H_
#include <stdint.h>
#include <stdbool.h>

bool is_leap_year(uint16_t year);
uint8_t get_days_from_month(uint8_t month, uint16_t year);

void clear_buffer();
int get_valid_option(int min, int max);
float get_float_input();
const char* get_string_input(int max_len);

#endif // UTILS_H_

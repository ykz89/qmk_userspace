// Stand-in for argos.h: Argos storage, where the module saves the trackball animation.
#pragma once
#include <stdint.h>
#define ARGOS_OFFSET_LED_MATRIX_CONFIG 0
void argos_read_eeprom(uint16_t offset, void *buf, uint16_t size);
void argos_write_eeprom(uint16_t offset, const void *buf, uint16_t size);

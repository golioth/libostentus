/*
 * Copyright (c) 2023 Golioth, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __OSTENTUS_WORK_H__
#define __OSTENTUS_WORK_H__
#include <zephyr/drivers/i2c.h>
#include <zephyr/device.h>

#define LED_USE 0x01
#define LED_GOL 0x02
#define LED_INT 0x04
#define LED_BAT 0x08
#define LED_POW 0x10

int clear_memory(void);
int show_splash(void);
int update_display(void);
int update_thickness(uint8_t thickness);
int update_font(uint8_t font);
int clear_text_buffer(void);
int clear_rectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
int slide_add(uint8_t id, char *str, uint8_t len);
int slide_set(uint8_t id, char *str, uint8_t len);
int slideshow(uint32_t setting);
int led_bitmask(uint8_t bitmask);
int led_power_set(uint8_t state);
int led_battery_set(uint8_t state);
int led_internet_set(uint8_t state);
int led_golioth_set(uint8_t state);
int led_user_set(uint8_t state);
int store_text(char *str, uint8_t len);
int write_text(uint8_t x, uint8_t y, uint8_t thickness);

#endif

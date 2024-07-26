/*
 * Copyright (c) 2023 Golioth, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __LIBOSTENTUS_H__
#define __LIBOSTENTUS_H__
#include <stdint.h>
#include <zephyr/device.h>

#define LED_USE 0x01
#define LED_GOL 0x02
#define LED_INT 0x04
#define LED_BAT 0x08
#define LED_POW 0x10

typedef int (*ostentus_cmd_t)(const struct device *dev);
typedef int (*ostentus_getval_8_t)(const struct device *dev, uint8_t *val);
typedef int (*ostentus_setval_8_t)(const struct device *dev, uint8_t val);
typedef int (*ostentus_setval_32_t)(const struct device *dev, uint32_t val);
typedef int (*ostentus_clear_rectangle_t)(const struct device *dev, uint8_t x, uint8_t y, uint8_t w,
					  uint8_t h);
typedef int (*ostentus_slide_t)(const struct device *dev, uint8_t id, char *str, uint8_t len);
typedef int (*ostentus_summary_title_t)(const struct device *dev, char *str, uint8_t len);
typedef int (*ostentus_buffer_op_t)(const struct device *dev, char *buf, uint8_t buf_len);
typedef int (*ostentus_write_text_t)(const struct device *dev, uint8_t x, uint8_t y,
				     uint8_t thickness);
typedef int (*ostentus_i2c_readbyte_t)(const struct device *dev, uint8_t reg, uint8_t *value);
typedef int (*ostentus_i2c_readarray_t)(const struct device *dev, uint8_t reg, uint8_t *read_reg,
					uint8_t read_len);

__subsystem struct ostentus_driver_api {
	ostentus_cmd_t ostentus_clear_memory;
	ostentus_cmd_t ostentus_show_splash;
	ostentus_cmd_t ostentus_update_display;
	ostentus_setval_8_t ostentus_update_thickness;
	ostentus_setval_8_t ostentus_update_font;
	ostentus_cmd_t ostentus_clear_text_buffer;
	ostentus_clear_rectangle_t ostentus_clear_rectangle;
	ostentus_slide_t ostentus_slide_add;
	ostentus_slide_t ostentus_slide_set;
	ostentus_buffer_op_t ostentus_summary_title;
	ostentus_setval_32_t ostentus_slideshow;
	ostentus_buffer_op_t ostentus_version_get;
	ostentus_getval_8_t ostentus_fifo_ready;
	ostentus_cmd_t ostentus_reset;
	ostentus_setval_8_t ostentus_led_bitmask;
	ostentus_setval_8_t ostentus_led_power_set;
	ostentus_setval_8_t ostentus_led_battery_set;
	ostentus_setval_8_t ostentus_led_internet_set;
	ostentus_setval_8_t ostentus_led_golioth_set;
	ostentus_setval_8_t ostentus_led_user_set;
	ostentus_buffer_op_t ostentus_store_text;
	ostentus_write_text_t ostentus_write_text;
	ostentus_i2c_readbyte_t ostentus_i2c_readbyte;
	ostentus_i2c_readarray_t ostentus_i2c_readarray;
};

int clear_memory(void);
int show_splash(void);
int update_display(void);
int update_thickness(uint8_t thickness);
int update_font(uint8_t font);
int clear_text_buffer(void);
int clear_rectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
int slide_add(uint8_t id, char *str, uint8_t len);
int slide_set(uint8_t id, char *str, uint8_t len);
int summary_title(char *str, uint8_t len);
int slideshow(uint32_t setting);
int ostentus_version_get(char *buf, uint8_t buf_len);
int ostentus_fifo_ready(uint8_t *slots_remaining);
int ostentus_reset(void);
int led_bitmask(uint8_t bitmask);
int led_power_set(uint8_t state);
int led_battery_set(uint8_t state);
int led_internet_set(uint8_t state);
int led_golioth_set(uint8_t state);
int led_user_set(uint8_t state);
int store_text(char *str, uint8_t len);
int write_text(uint8_t x, uint8_t y, uint8_t thickness);
int ostentus_i2c_readbyte(uint8_t reg, uint8_t *value);
int ostentus_i2c_readarray(uint8_t reg, uint8_t *read_reg, uint8_t read_len);

#endif

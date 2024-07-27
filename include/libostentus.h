/*
 * Copyright (c) 2023 Golioth, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __LIBOSTENTUS_H__
#define __LIBOSTENTUS_H__
#include <stdint.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>

struct ostentus_config {
	struct i2c_dt_spec i2c;
};

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

__syscall int clear_memory(const struct device *dev);
__syscall int show_splash(const struct device *dev);
__syscall int update_display(const struct device *dev);
__syscall int update_thickness(const struct device *dev, uint8_t thickness);
__syscall int update_font(const struct device *dev, uint8_t font);
__syscall int clear_text_buffer(const struct device *dev);
__syscall int clear_rectangle(const struct device *dev, uint8_t x, uint8_t y, uint8_t w, uint8_t h);
__syscall int slide_add(const struct device *dev, uint8_t id, char *str, uint8_t len);
__syscall int slide_set(const struct device *dev, uint8_t id, char *str, uint8_t len);
__syscall int summary_title(const struct device *dev, char *str, uint8_t len);
__syscall int slideshow(const struct device *dev, uint32_t setting);
__syscall int ostentus_version_get(const struct device *dev, char *buf, uint8_t buf_len);
__syscall int ostentus_fifo_ready(const struct device *dev, uint8_t *slots_remaining);
__syscall int ostentus_reset(const struct device *dev);
__syscall int led_bitmask(const struct device *dev, uint8_t bitmask);
__syscall int led_power_set(const struct device *dev, uint8_t state);
__syscall int led_battery_set(const struct device *dev, uint8_t state);
__syscall int led_internet_set(const struct device *dev, uint8_t state);
__syscall int led_golioth_set(const struct device *dev, uint8_t state);
__syscall int led_user_set(const struct device *dev, uint8_t state);
__syscall int store_text(const struct device *dev, char *str, uint8_t len);
__syscall int write_text(const struct device *dev, uint8_t x, uint8_t y, uint8_t thickness);
__syscall int ostentus_i2c_readbyte(const struct device *dev, uint8_t reg, uint8_t *value);
__syscall int ostentus_i2c_readarray(const struct device *dev, uint8_t reg, uint8_t *read_reg, uint8_t read_len);

#endif

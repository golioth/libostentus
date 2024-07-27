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

__syscall int ostentus_clear_memory(const struct device *dev);

static inline int z_impl_ostentus_clear_memory(const struct device *dev)
{
	const struct ostentus_driver_api *api = (const struct ostentus_driver_api *)dev->api;
	if (api->ostentus_clear_memory == NULL) {
		return -ENOSYS;
	}
	return api->ostentus_clear_memory(dev);
}

__syscall int ostentus_show_splash(const struct device *dev);

static inline int z_impl_ostentus_show_splash(const struct device *dev)
{
	const struct ostentus_driver_api *api = (const struct ostentus_driver_api *)dev->api;
	if (api->ostentus_show_splash == NULL) {
		return -ENOSYS;
	}
	return api->ostentus_show_splash(dev);
}

__syscall int ostentus_update_display(const struct device *dev);

static inline int z_impl_ostentus_update_display(const struct device *dev)
{
	const struct ostentus_driver_api *api = (const struct ostentus_driver_api *)dev->api;
	if (api->ostentus_update_display == NULL) {
		return -ENOSYS;
	}
	return api->ostentus_update_display(dev);
}

__syscall int ostentus_update_thickness(const struct device *dev, uint8_t thickness);

static inline int z_impl_ostentus_update_thickness(const struct device *dev, uint8_t thickness)
{
	const struct ostentus_driver_api *api = (const struct ostentus_driver_api *)dev->api;
	if (api->ostentus_update_thickness == NULL) {
		return -ENOSYS;
	}
	return api->ostentus_update_thickness(dev, thickness);
}

__syscall int ostentus_update_font(const struct device *dev, uint8_t font);

static inline int z_impl_ostentus_update_font(const struct device *dev, uint8_t font)
{
	const struct ostentus_driver_api *api = (const struct ostentus_driver_api *)dev->api;
	if (api->ostentus_update_font == NULL) {
		return -ENOSYS;
	}
	return api->ostentus_update_font(dev, font);
}

__syscall int ostentus_clear_text_buffer(const struct device *dev);

static inline int z_impl_ostentus_clear_text_buffer(const struct device *dev)
{
	const struct ostentus_driver_api *api = (const struct ostentus_driver_api *)dev->api;
	if (api->ostentus_clear_text_buffer == NULL) {
		return -ENOSYS;
	}
	return api->ostentus_clear_text_buffer(dev);
}

__syscall int ostentus_clear_rectangle(const struct device *dev, uint8_t x, uint8_t y, uint8_t w,
				       uint8_t h);

static inline int z_impl_ostentus_clear_rectangle(const struct device *dev, uint8_t x, uint8_t y,
						  uint8_t w, uint8_t h)
{
	const struct ostentus_driver_api *api = (const struct ostentus_driver_api *)dev->api;
	if (api->ostentus_clear_rectangle == NULL) {
		return -ENOSYS;
	}
	return api->ostentus_clear_rectangle(dev, x, y, w, h);
}

__syscall int ostentus_slide_add(const struct device *dev, uint8_t id, char *str, uint8_t len);

static inline int z_impl_ostentus_slide_add(const struct device *dev, uint8_t id, char *str,
					    uint8_t len)
{
	const struct ostentus_driver_api *api = (const struct ostentus_driver_api *)dev->api;
	if (api->ostentus_slide_add == NULL) {
		return -ENOSYS;
	}
	return api->ostentus_slide_add(dev, id, str, len);
}

__syscall int ostentus_slide_set(const struct device *dev, uint8_t id, char *str, uint8_t len);

static inline int z_impl_ostentus_slide_set(const struct device *dev, uint8_t id, char *str,
					    uint8_t len)
{
	const struct ostentus_driver_api *api = (const struct ostentus_driver_api *)dev->api;
	if (api->ostentus_slide_set == NULL) {
		return -ENOSYS;
	}
	return api->ostentus_slide_set(dev, id, str, len);
}

__syscall int ostentus_summary_title(const struct device *dev, char *str, uint8_t len);

static inline int z_impl_ostentus_summary_title(const struct device *dev, char *str, uint8_t len)
{
	const struct ostentus_driver_api *api = (const struct ostentus_driver_api *)dev->api;
	if (api->ostentus_summary_title == NULL) {
		return -ENOSYS;
	}
	return api->ostentus_summary_title(dev, str, len);
}

__syscall int ostentus_slideshow(const struct device *dev, uint32_t setting);

static inline int z_impl_ostentus_slideshow(const struct device *dev, uint32_t setting)
{
	const struct ostentus_driver_api *api = (const struct ostentus_driver_api *)dev->api;
	if (api->ostentus_slideshow == NULL) {
		return -ENOSYS;
	}
	return api->ostentus_slideshow(dev, setting);
}

__syscall int ostentus_version_get(const struct device *dev, char *buf, uint8_t buf_len);

static inline int z_impl_ostentus_version_get(const struct device *dev, char *buf, uint8_t buf_len)
{
	const struct ostentus_driver_api *api = (const struct ostentus_driver_api *)dev->api;
	if (api->ostentus_version_get == NULL) {
		return -ENOSYS;
	}
	return api->ostentus_version_get(dev, buf, buf_len);
}

__syscall int ostentus_fifo_ready(const struct device *dev, uint8_t *slots_remaining);

static inline int z_impl_ostentus_fifo_ready(const struct device *dev, uint8_t *slots_remaining)
{
	const struct ostentus_driver_api *api = (const struct ostentus_driver_api *)dev->api;
	if (api->ostentus_fifo_ready == NULL) {
		return -ENOSYS;
	}
	return api->ostentus_fifo_ready(dev, slots_remaining);
}

__syscall int ostentus_reset(const struct device *dev);

static inline int z_impl_ostentus_reset(const struct device *dev)
{
	const struct ostentus_driver_api *api = (const struct ostentus_driver_api *)dev->api;
	if (api->ostentus_reset == NULL) {
		return -ENOSYS;
	}
	return api->ostentus_reset(dev);
}

__syscall int ostentus_led_bitmask(const struct device *dev, uint8_t bitmask);

static inline int z_impl_ostentus_led_bitmask(const struct device *dev, uint8_t bitmask)
{
	const struct ostentus_driver_api *api = (const struct ostentus_driver_api *)dev->api;
	if (api->ostentus_led_bitmask == NULL) {
		return -ENOSYS;
	}
	return api->ostentus_led_bitmask(dev, bitmask);
}

__syscall int ostentus_led_power_set(const struct device *dev, uint8_t state);

static inline int z_impl_ostentus_led_power_set(const struct device *dev, uint8_t state)
{
	const struct ostentus_driver_api *api = (const struct ostentus_driver_api *)dev->api;
	if (api->ostentus_led_power_set == NULL) {
		return -ENOSYS;
	}
	return api->ostentus_led_power_set(dev, state);
}

__syscall int ostentus_led_battery_set(const struct device *dev, uint8_t state);

static inline int z_impl_ostentus_led_battery_set(const struct device *dev, uint8_t state)
{
	const struct ostentus_driver_api *api = (const struct ostentus_driver_api *)dev->api;
	if (api->ostentus_led_battery_set == NULL) {
		return -ENOSYS;
	}
	return api->ostentus_led_battery_set(dev, state);
}

__syscall int ostentus_led_internet_set(const struct device *dev, uint8_t state);

static inline int z_impl_ostentus_led_internet_set(const struct device *dev, uint8_t state)
{
	const struct ostentus_driver_api *api = (const struct ostentus_driver_api *)dev->api;
	if (api->ostentus_led_internet_set == NULL) {
		return -ENOSYS;
	}
	return api->ostentus_led_internet_set(dev, state);
}

__syscall int ostentus_led_golioth_set(const struct device *dev, uint8_t state);

static inline int z_impl_ostentus_led_golioth_set(const struct device *dev, uint8_t state)
{
	const struct ostentus_driver_api *api = (const struct ostentus_driver_api *)dev->api;
	if (api->ostentus_led_golioth_set == NULL) {
		return -ENOSYS;
	}
	return api->ostentus_led_golioth_set(dev, state);
}

__syscall int ostentus_led_user_set(const struct device *dev, uint8_t state);

static inline int z_impl_ostentus_led_user_set(const struct device *dev, uint8_t state)
{
	const struct ostentus_driver_api *api = (const struct ostentus_driver_api *)dev->api;
	if (api->ostentus_led_user_set == NULL) {
		return -ENOSYS;
	}
	return api->ostentus_led_user_set(dev, state);
}

__syscall int ostentus_store_text(const struct device *dev, char *str, uint8_t len);

static inline int z_impl_ostentus_store_text(const struct device *dev, char *str, uint8_t len)
{
	const struct ostentus_driver_api *api = (const struct ostentus_driver_api *)dev->api;
	if (api->ostentus_store_text == NULL) {
		return -ENOSYS;
	}
	return api->ostentus_store_text(dev, str, len);
}

__syscall int ostentus_write_text(const struct device *dev, uint8_t x, uint8_t y,
				  uint8_t thickness);

static inline int z_impl_ostentus_write_text(const struct device *dev, uint8_t x, uint8_t y,
					     uint8_t thickness)
{
	const struct ostentus_driver_api *api = (const struct ostentus_driver_api *)dev->api;
	if (api->ostentus_write_text == NULL) {
		return -ENOSYS;
	}
	return api->ostentus_write_text(dev, x, y, thickness);
}

__syscall int ostentus_i2c_readbyte(const struct device *dev, uint8_t reg, uint8_t *value);

static inline int z_impl_ostentus_i2c_readbyte(const struct device *dev, uint8_t reg,
					       uint8_t *value)
{
	const struct ostentus_driver_api *api = (const struct ostentus_driver_api *)dev->api;
	if (api->ostentus_i2c_readbyte == NULL) {
		return -ENOSYS;
	}
	return api->ostentus_i2c_readbyte(dev, reg, value);
}

__syscall int ostentus_i2c_readarray(const struct device *dev, uint8_t reg, uint8_t *read_reg,
				     uint8_t read_len);

static inline int z_impl_ostentus_i2c_readarray(const struct device *dev, uint8_t reg,
						uint8_t *read_reg, uint8_t read_len)
{
	const struct ostentus_driver_api *api = (const struct ostentus_driver_api *)dev->api;
	if (api->ostentus_i2c_readarray == NULL) {
		return -ENOSYS;
	}
	return api->ostentus_i2c_readarray(dev, reg, read_reg, read_len);
}

#include <syscalls/libostentus.h>

#endif

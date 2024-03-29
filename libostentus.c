/*
 * Copyright (c) 2023 Golioth, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(ostentus_wrapper, LOG_LEVEL_DBG);

#include <zephyr/drivers/i2c.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/byteorder.h>
#include <string.h>
#include <libostentus_regmap.h>

#define OSTENTUS_ADDR 0x12

const struct i2c_dt_spec ostentus_dev = {
	.bus = DEVICE_DT_GET(DT_ALIAS(click_i2c)),
	.addr = OSTENTUS_ADDR,
};

bool _uninitialized = true;
bool _is_present = false;

/**
 * @brief Confirm that the Ostentus board is present
 *
 * This function tests to see if the Ostentus board is present by issuing the
 * command to erase memory. If the device is not present to receive the command,
 * the global _is_present will be set to false and all subsequent operations
 * will not try to write to the i2c bus.
 *
 * This function may be run explicitly, or it will be run auotmatically upon the
 * first attempt to write to i2c if the _uninitialized varible is set to true.
 *
 * @return the new value of _is_present
 */
bool ostentus_i2c_init(void) {
	uint8_t byte = 0x00;

	_uninitialized = false;

	int err = i2c_write_dt(&ostentus_dev, &byte, 1);
	if (err) {
		LOG_ERR("Unable to communicate with Ostentus over i2c: %d", err);
		LOG_DBG("All future calls to Ostentus functions will not be sent.");
		_is_present = false;
	}
	else {
		LOG_INF("Ostentus present at i2c address: 0x%02X", ostentus_dev.addr);
		_is_present = true;
	}
	return _is_present;
}

int ostentus_i2c_write2(uint8_t reg,
			uint8_t *data1, uint8_t data1_len,
			uint8_t *data2, uint8_t data2_len) {

	struct i2c_msg msgs[] = {
		{
			.buf = &reg,
			.len = 1,
			.flags = I2C_MSG_WRITE,
		},
		{
			.buf = data1,
			.len = data1_len,
			.flags = I2C_MSG_WRITE,
		},
		{
			.buf = data2,
			.len = data2_len,
			.flags = I2C_MSG_WRITE | I2C_MSG_STOP,
		},
	};
	uint8_t num_msgs = ARRAY_SIZE(msgs);

	if (_uninitialized) {
		ostentus_i2c_init();
	}

	if (_is_present == false) {
		return -EFAULT;
	}

	/* Detect how many i2c messages there are and which is the last one */
	for (int i = 1; i < ARRAY_SIZE(msgs); i++) {
		if (!msgs[i].len) {
			msgs[i - 1].flags |= I2C_MSG_STOP;
			num_msgs = i;
		}
	}

	return i2c_transfer_dt(&ostentus_dev, msgs, num_msgs);
}

int ostentus_i2c_write1(uint8_t reg, uint8_t *data, uint8_t data_len) {
	return ostentus_i2c_write2(reg, data, data_len, NULL, 0);
}

int ostentus_i2c_write0(uint8_t reg) {
	return ostentus_i2c_write2(reg, NULL, 0, NULL, 0);
}

int ostentus_i2c_readbyte(uint8_t reg, uint8_t *value) {
  return i2c_write_read_dt(&ostentus_dev, &reg, 1, value, 1);
}

int ostentus_i2c_readarray(uint8_t reg, uint8_t *read_reg, uint8_t read_len) {
  int err;
  uint8_t write_reg[1] = { reg };
  err = i2c_write_read_dt(&ostentus_dev, write_reg, 1, read_reg, read_len);
  return err;
}

int clear_memory(void) {
	return ostentus_i2c_write0(OSTENTUS_CLEAR_MEM);
}

int show_splash(void) {
	return ostentus_i2c_write0(OSTENTUS_SPLASHSCREEN);
}

int update_display(void) {
	return ostentus_i2c_write0(OSTENTUS_REFRESH);
}

int update_thickness(uint8_t thickness) {
	return ostentus_i2c_write1(OSTENTUS_THICKNESS, &thickness, 1);
}

int update_font(uint8_t font) {
	return ostentus_i2c_write1(OSTENTUS_FONT, &font, 1);
}

int clear_text_buffer(void) {
	return ostentus_i2c_write0(OSTENTUS_CLEAR_TEXT);
}

int clear_rectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
	uint8_t xywh[] = { x, y, w, h };
	return ostentus_i2c_write1(OSTENTUS_CLEAR_RECT, xywh, sizeof(xywh));
}

int slide_add(uint8_t id, char *str, uint8_t len) {
	return ostentus_i2c_write2(OSTENTUS_SLIDE_ADD, &id, 1, str, len);
}

int slide_set(uint8_t id, char *str, uint8_t len) {
	return ostentus_i2c_write2(OSTENTUS_SLIDE_SET, &id, 1, str, len);
}

int summary_title(char *str, uint8_t len) {
	return ostentus_i2c_write1(OSTENTUS_SUMMARY_TITLE, str, len);
}

int slideshow(uint32_t setting) {
	union {
		uint32_t setting_le;
		uint8_t setting_buf[4];
	} slideshow_delay_u;

	slideshow_delay_u.setting_le = sys_cpu_to_le32(setting);
	return ostentus_i2c_write1(OSTENTUS_SLIDESHOW,
			slideshow_delay_u.setting_buf,
			sizeof(slideshow_delay_u.setting_buf));
}

int ostentus_version_get(char *buf, uint8_t buf_len) {
  uint8_t semver[3] = { 0 };
  int err = ostentus_i2c_readarray(OSTENTUS_GET_VERSION, semver, 3);
  snprintk(buf, buf_len, "v%d.%d.%d", semver[0], semver[1], semver[2]);
  return err;
}

int ostentus_fifo_ready(uint8_t *slots_remaining) {
  return ostentus_i2c_readbyte(OSTENTUS_FIFO_READY, slots_remaining);
}

int ostentus_reset(void) {
  uint8_t magic = OSTENTUS_RESET_MAGIC;
  return ostentus_i2c_write1(OSTENTUS_RESET, &magic, 1);
}

int led_bitmask(uint8_t bitmask) {
  return ostentus_i2c_write1(OSTENTUS_LED_BITMASK, &bitmask, 1);
}

int led_power_set(uint8_t state) {
	uint8_t byte = state ? 1 : 0;
	return ostentus_i2c_write1(OSTENTUS_LED_POW, &byte, 1);
}

int led_battery_set(uint8_t state) {
	uint8_t byte = state ? 1 : 0;
	return ostentus_i2c_write1(OSTENTUS_LED_BAT, &byte, 1);
}

int led_internet_set(uint8_t state) {
	uint8_t byte = state ? 1 : 0;
	return ostentus_i2c_write1(OSTENTUS_LED_INT, &byte, 1);
}

int led_golioth_set(uint8_t state) {
	uint8_t byte = state ? 1 : 0;
	return ostentus_i2c_write1(OSTENTUS_LED_GOL, &byte, 1);
}

int led_user_set(uint8_t state) {
	uint8_t byte = state ? 1 : 0;
	return ostentus_i2c_write1(OSTENTUS_LED_USE, &byte, 1);
}

int store_text(char *str, uint8_t len) {
	return ostentus_i2c_write1(OSTENTUS_STORE_TEXT, str, len);
}

int write_text(uint8_t x, uint8_t y, uint8_t thickness) {
	uint8_t data[] = { x, y, thickness };
	return ostentus_i2c_write1(OSTENTUS_WRITE_TEXT, data, sizeof(data));
}

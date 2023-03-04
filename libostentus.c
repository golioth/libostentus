/*
 * Copyright (c) 2023 Golioth, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(ostentus_wrapper, LOG_LEVEL_DBG);

#include <zephyr/drivers/i2c.h>
#include <zephyr/device.h>
#include <string.h>

#define OSTENTUS_ADDR 0x12

#define OSTENTUS_CLEAR_MEM 0x00
#define OSTENTUS_REFRESH 0x01
#define OSTENTUS_ADDR_X 0x02
#define OSTENTUS_ADDR_Y 0x03
#define OSTENTUS_SPLASHSCREEN 0x04
#define OSTENTUS_THICKNESS 0x05
#define OSTENTUS_FONT 0x06
#define OSTENTUS_WRITE_TEXT 0x07
#define OSTENTUS_CLEAR_TEXT 0x08
#define OSTENTUS_CLEAR_RECT 0x09
#define OSTENTUS_SLIDE_ADD 0x0A
#define OSTENTUS_SLIDE_SET 0x0B
#define OSTENTUS_SLIDESHOW 0x0C
#define OSTENTUS_SUMMARY_TITLE 0x0D
#define OSTENTUS_LED_USE 0x10
#define OSTENTUS_LED_GOL 0x11
#define OSTENTUS_LED_INT 0x12
#define OSTENTUS_LED_BAT 0x13
#define OSTENTUS_LED_POW 0x14
#define OSTENTUS_LED_BITMASK 0x18
#define OSTENTUS_STRING_0 0x20
#define OSTENTUS_STRING_1 0x21
#define OSTENTUS_STRING_2 0x22
#define OSTENTUS_STRING_3 0x23
#define OSTENTUS_STRING_4 0x24
#define OSTENTUS_STRING_5 0x25
#define OSTENTUS_STORE_TEXT 0x26

K_MUTEX_DEFINE(ostentus_mutex);

const struct device *ostentus_i2c_dev = DEVICE_DT_GET(DT_ALIAS(click_i2c));

bool _uninitialized = true;
bool _is_present = false;

#define BUF_SIZE 48
uint8_t _ostentus_buf[BUF_SIZE];


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
	LOG_DBG("Requesting Ostentus mutex lock");
	k_mutex_lock(&ostentus_mutex, K_FOREVER);
	LOG_DBG("Locked Ostentus mutex (lock count: %u)",
		ostentus_mutex.lock_count);

	_uninitialized = false;

	_ostentus_buf[0] = 0x00;
	int err = i2c_write(ostentus_i2c_dev, _ostentus_buf, 1, OSTENTUS_ADDR);
	if (err) {
		LOG_ERR("Unable to communicate with Ostentus over i2c: %d", err);
		LOG_DBG("All future calls to Ostentus functions will not be sent.");
		_is_present = false;
	}
	else {
		LOG_INF("Ostentus present at i2c address: 0x%02X", OSTENTUS_ADDR);
		_is_present = true;
	}

	k_mutex_unlock(&ostentus_mutex);
	LOG_DBG("Unlocked Ostentus mutex (lock count: %u)",
		ostentus_mutex.lock_count);

	return _is_present;
}

int ostentus_i2c_write(uint8_t reg, uint8_t data_len) {
	int err;

	LOG_DBG("Requesting Ostentus mutex lock");
	k_mutex_lock(&ostentus_mutex, K_FOREVER);
	LOG_DBG("Locked Ostentus mutex (lock count: %u)",
		ostentus_mutex.lock_count);

	if (_uninitialized) {
		ostentus_i2c_init();
	}

	if (_is_present == false) {
		LOG_DBG("Ostentus not present, command not sent.");
		return -EFAULT;
	}

	_ostentus_buf[0] = reg;
	//LOG_HEXDUMP_DBG(_ostentus_buf, data_len+1, "sending packet");
	err = i2c_write(ostentus_i2c_dev, _ostentus_buf, data_len+1, OSTENTUS_ADDR);

	k_mutex_unlock(&ostentus_mutex);
	LOG_DBG("Unlocked Ostentus mutex (lock count: %u)",
		ostentus_mutex.lock_count);
	
	return err;
}

int clear_memory(void) {
	return ostentus_i2c_write(OSTENTUS_CLEAR_MEM, 0);
}

int show_splash(void) {
	return ostentus_i2c_write(OSTENTUS_SPLASHSCREEN, 0);
}

int update_display(void) {
	return ostentus_i2c_write(OSTENTUS_REFRESH, 0);
}

int update_thickness(uint8_t thickness) {
	int err;

	LOG_DBG("Requesting Ostentus mutex lock");
	k_mutex_lock(&ostentus_mutex, K_FOREVER);
	LOG_DBG("Locked Ostentus mutex (lock count: %u)",
		ostentus_mutex.lock_count);

	_ostentus_buf[1] = thickness;
	err = ostentus_i2c_write(OSTENTUS_THICKNESS, 1);

	k_mutex_unlock(&ostentus_mutex);
	LOG_DBG("Unlocked Ostentus mutex (lock count: %u)",
		ostentus_mutex.lock_count);

	return err;
}

int update_font(uint8_t font) {
	int err;

	LOG_DBG("Requesting Ostentus mutex lock");
	k_mutex_lock(&ostentus_mutex, K_FOREVER);
	LOG_DBG("Locked Ostentus mutex (lock count: %u)",
		ostentus_mutex.lock_count);

	_ostentus_buf[1] = font;
	err = ostentus_i2c_write(OSTENTUS_FONT, 1);

	k_mutex_unlock(&ostentus_mutex);
	LOG_DBG("Unlocked Ostentus mutex (lock count: %u)",
		ostentus_mutex.lock_count);

	return err;
}

int clear_text_buffer(void) {
	return ostentus_i2c_write(OSTENTUS_CLEAR_TEXT, 0);
}

int clear_rectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
	int err;

	LOG_DBG("Requesting Ostentus mutex lock");
	k_mutex_lock(&ostentus_mutex, K_FOREVER);
	LOG_DBG("Locked Ostentus mutex (lock count: %u)",
		ostentus_mutex.lock_count);

	_ostentus_buf[1] = x;
	_ostentus_buf[2] = y;
	_ostentus_buf[3] = w;
	_ostentus_buf[4] = h;
	err = ostentus_i2c_write(OSTENTUS_CLEAR_RECT, 4);

	k_mutex_unlock(&ostentus_mutex);
	LOG_DBG("Unlocked Ostentus mutex (lock count: %u)",
		ostentus_mutex.lock_count);

	return err;
}

int slide_add(uint8_t id, char *str, uint8_t len) {
	int err;

	LOG_DBG("Requesting Ostentus mutex lock");
	k_mutex_lock(&ostentus_mutex, K_FOREVER);
	LOG_DBG("Locked Ostentus mutex (lock count: %u)",
		ostentus_mutex.lock_count);

	_ostentus_buf[1] = id;
	memcpy(_ostentus_buf+2, str, len);

	err = ostentus_i2c_write(OSTENTUS_SLIDE_ADD, len+1);

	k_mutex_unlock(&ostentus_mutex);
	LOG_DBG("Unlocked Ostentus mutex (lock count: %u)",
		ostentus_mutex.lock_count);

	return err;
}

int slide_set(uint8_t id, char *str, uint8_t len) {
	int err;

	LOG_DBG("Requesting Ostentus mutex lock");
	k_mutex_lock(&ostentus_mutex, K_FOREVER);
	LOG_DBG("Locked Ostentus mutex (lock count: %u)",
		ostentus_mutex.lock_count);

	_ostentus_buf[1] = id;
	memcpy(_ostentus_buf+2, str, len);
	err = ostentus_i2c_write(OSTENTUS_SLIDE_SET, len+1);

	k_mutex_unlock(&ostentus_mutex);
	LOG_DBG("Unlocked Ostentus mutex (lock count: %u)",
		ostentus_mutex.lock_count);

	return err;
}

int summary_title(char *str, uint8_t len) {
	int err;

	LOG_DBG("Requesting Ostentus mutex lock");
	k_mutex_lock(&ostentus_mutex, K_FOREVER);
	LOG_DBG("Locked Ostentus mutex (lock count: %u)",
		ostentus_mutex.lock_count);

	memcpy(_ostentus_buf+1, str, len);
	err = ostentus_i2c_write(OSTENTUS_SUMMARY_TITLE, len);

	k_mutex_unlock(&ostentus_mutex);
	LOG_DBG("Unlocked Ostentus mutex (lock count: %u)",
		ostentus_mutex.lock_count);

	return err;
}

int slideshow(uint32_t setting) {
	int err;

	LOG_DBG("Requesting Ostentus mutex lock");
	k_mutex_lock(&ostentus_mutex, K_FOREVER);
	LOG_DBG("Locked Ostentus mutex (lock count: %u)",
		ostentus_mutex.lock_count);

	memcpy(_ostentus_buf+1, &setting, 4);
	err = ostentus_i2c_write(OSTENTUS_SLIDESHOW, 4);

	k_mutex_unlock(&ostentus_mutex);
	LOG_DBG("Unlocked Ostentus mutex (lock count: %u)",
		ostentus_mutex.lock_count);

	return err;
}

int led_bitmask(uint8_t bitmask) {
	int err;

	LOG_DBG("Requesting Ostentus mutex lock");
	k_mutex_lock(&ostentus_mutex, K_FOREVER);
	LOG_DBG("Locked Ostentus mutex (lock count: %u)",
		ostentus_mutex.lock_count);

	_ostentus_buf[1] = bitmask;
	err = ostentus_i2c_write(OSTENTUS_LED_BITMASK, 1);

	k_mutex_unlock(&ostentus_mutex);
	LOG_DBG("Unlocked Ostentus mutex (lock count: %u)",
		ostentus_mutex.lock_count);

	return err;
}

int led_power_set(uint8_t state) {
	int err;

	LOG_DBG("Requesting Ostentus mutex lock");
	k_mutex_lock(&ostentus_mutex, K_FOREVER);
	LOG_DBG("Locked Ostentus mutex (lock count: %u)",
		ostentus_mutex.lock_count);

	_ostentus_buf[1] = state ? 1 : 0;
	err = ostentus_i2c_write(OSTENTUS_LED_POW, 1);

	k_mutex_unlock(&ostentus_mutex);
	LOG_DBG("Unlocked Ostentus mutex (lock count: %u)",
		ostentus_mutex.lock_count);

	return err;
}

int led_battery_set(uint8_t state) {
	int err;

	LOG_DBG("Requesting Ostentus mutex lock");
	k_mutex_lock(&ostentus_mutex, K_FOREVER);
	LOG_DBG("Locked Ostentus mutex (lock count: %u)",
		ostentus_mutex.lock_count);

	_ostentus_buf[1] = state ? 1 : 0;
	err = ostentus_i2c_write(OSTENTUS_LED_BAT, 1);

	k_mutex_unlock(&ostentus_mutex);
	LOG_DBG("Unlocked Ostentus mutex (lock count: %u)",
		ostentus_mutex.lock_count);

	return err;
}

int led_internet_set(uint8_t state) {
	int err;

	LOG_DBG("Requesting Ostentus mutex lock");
	k_mutex_lock(&ostentus_mutex, K_FOREVER);
	LOG_DBG("Locked Ostentus mutex (lock count: %u)",
		ostentus_mutex.lock_count);

	_ostentus_buf[1] = state ? 1 : 0;
	err = ostentus_i2c_write(OSTENTUS_LED_INT, 1);

	k_mutex_unlock(&ostentus_mutex);
	LOG_DBG("Unlocked Ostentus mutex (lock count: %u)",
		ostentus_mutex.lock_count);
	
	return err;
}

int led_golioth_set(uint8_t state) {
	int err;

	LOG_DBG("Requesting Ostentus mutex lock");
	k_mutex_lock(&ostentus_mutex, K_FOREVER);
	LOG_DBG("Locked Ostentus mutex (lock count: %u)",
		ostentus_mutex.lock_count);

	_ostentus_buf[1] = state ? 1 : 0;
	err = ostentus_i2c_write(OSTENTUS_LED_GOL, 1);

	k_mutex_unlock(&ostentus_mutex);
	LOG_DBG("Unlocked Ostentus mutex (lock count: %u)",
		ostentus_mutex.lock_count);

	return err;
}

int led_user_set(uint8_t state) {
	int err;

	LOG_DBG("Requesting Ostentus mutex lock");
	k_mutex_lock(&ostentus_mutex, K_FOREVER);
	LOG_DBG("Locked Ostentus mutex (lock count: %u)",
		ostentus_mutex.lock_count);

	_ostentus_buf[1] = state ? 1 : 0;
	err = ostentus_i2c_write(OSTENTUS_LED_USE, 1);

	k_mutex_unlock(&ostentus_mutex);
	LOG_DBG("Unlocked Ostentus mutex (lock count: %u)",
		ostentus_mutex.lock_count);

	return err;
}

int store_text(char *str, uint8_t len) {
	int err;

	LOG_DBG("Requesting Ostentus mutex lock");
	k_mutex_lock(&ostentus_mutex, K_FOREVER);
	LOG_DBG("Locked Ostentus mutex (lock count: %u)",
		ostentus_mutex.lock_count);

	memcpy(_ostentus_buf+1, str, len);
	return ostentus_i2c_write(OSTENTUS_STORE_TEXT, len);

	k_mutex_unlock(&ostentus_mutex);
	LOG_DBG("Unlocked Ostentus mutex (lock count: %u)",
		ostentus_mutex.lock_count);

	return err;
}

int write_text(uint8_t x, uint8_t y, uint8_t thickness) {
	int err;

	LOG_DBG("Requesting Ostentus mutex lock");
	k_mutex_lock(&ostentus_mutex, K_FOREVER);
	LOG_DBG("Locked Ostentus mutex (lock count: %u)",
		ostentus_mutex.lock_count);

	_ostentus_buf[1] = x;
	_ostentus_buf[2] = y;
	_ostentus_buf[3] = thickness;
	err = ostentus_i2c_write(OSTENTUS_WRITE_TEXT, 3);

	k_mutex_unlock(&ostentus_mutex);
	LOG_DBG("Unlocked Ostentus mutex (lock count: %u)",
		ostentus_mutex.lock_count);

	return err;
}

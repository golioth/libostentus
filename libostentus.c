/*
 * Copyright (c) 2023 Golioth, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT golioth_ostentus

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(ostentus_wrapper, LOG_LEVEL_DBG);

#include <zephyr/drivers/i2c.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/byteorder.h>
#include <string.h>
#include <libostentus.h>
#include <libostentus_regmap.h>

static int ostentus_i2c_write2(const struct device *dev, uint8_t reg, uint8_t *data1,
			       uint8_t data1_len, uint8_t *data2, uint8_t data2_len)
{
	const struct ostentus_config *config = dev->config;

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

	/* Detect how many i2c messages there are and which is the last one */
	for (int i = 1; i < ARRAY_SIZE(msgs); i++) {
		if (!msgs[i].len) {
			msgs[i - 1].flags |= I2C_MSG_STOP;
			num_msgs = i;
		}
	}

	return i2c_transfer_dt(&config->i2c, msgs, num_msgs);
}

static int ostentus_i2c_write1(const struct device *dev, uint8_t reg, uint8_t *data,
			       uint8_t data_len)
{
	return ostentus_i2c_write2(dev, reg, data, data_len, NULL, 0);
}

static int ostentus_i2c_write0(const struct device *dev, uint8_t reg)
{
	return ostentus_i2c_write2(dev, reg, NULL, 0, NULL, 0);
}

static int i2c_readbyte(const struct device *dev, uint8_t reg, uint8_t *value)
{
	const struct ostentus_config *config = dev->config;
	return i2c_write_read_dt(&config->i2c, &reg, 1, value, 1);
}

static int i2c_readarray(const struct device *dev, uint8_t reg, uint8_t *read_reg, uint8_t read_len)
{
	const struct ostentus_config *config = dev->config;
	int err;
	uint8_t write_reg[1] = {reg};
	err = i2c_write_read_dt(&config->i2c, write_reg, 1, read_reg, read_len);
	return err;
}

static int clear_memory(const struct device *dev)
{
	return ostentus_i2c_write0(dev, OSTENTUS_CLEAR_MEM);
}

static int show_splash(const struct device *dev)
{
	return ostentus_i2c_write0(dev, OSTENTUS_SPLASHSCREEN);
}

static int update_display(const struct device *dev)
{
	return ostentus_i2c_write0(dev, OSTENTUS_REFRESH);
}

static int update_thickness(const struct device *dev, uint8_t thickness)
{
	return ostentus_i2c_write1(dev, OSTENTUS_THICKNESS, &thickness, 1);
}

static int update_font(const struct device *dev, uint8_t font)
{
	return ostentus_i2c_write1(dev, OSTENTUS_FONT, &font, 1);
}

static int clear_text_buffer(const struct device *dev)
{
	return ostentus_i2c_write0(dev, OSTENTUS_CLEAR_TEXT);
}

static int clear_rectangle(const struct device *dev, uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
	uint8_t xywh[] = {x, y, w, h};
	return ostentus_i2c_write1(dev, OSTENTUS_CLEAR_RECT, xywh, sizeof(xywh));
}

static int slide_add(const struct device *dev, uint8_t id, char *str, uint8_t len)
{
	return ostentus_i2c_write2(dev, OSTENTUS_SLIDE_ADD, &id, 1, str, len);
}

static int slide_set(const struct device *dev, uint8_t id, char *str, uint8_t len)
{
	return ostentus_i2c_write2(dev, OSTENTUS_SLIDE_SET, &id, 1, str, len);
}

static int summary_title(const struct device *dev, char *str, uint8_t len)
{
	return ostentus_i2c_write1(dev, OSTENTUS_SUMMARY_TITLE, str, len);
}

static int slideshow(const struct device *dev, uint32_t setting)
{
	union {
		uint32_t setting_le;
		uint8_t setting_buf[4];
	} slideshow_delay_u;

	slideshow_delay_u.setting_le = sys_cpu_to_le32(setting);
	return ostentus_i2c_write1(dev, OSTENTUS_SLIDESHOW, slideshow_delay_u.setting_buf,
				   sizeof(slideshow_delay_u.setting_buf));
}

static int version_get(const struct device *dev, char *buf, uint8_t buf_len)
{
	uint8_t semver[3] = {0};
	int err = ostentus_i2c_readarray(dev, OSTENTUS_GET_VERSION, semver, 3);
	snprintk(buf, buf_len, "v%d.%d.%d", semver[0], semver[1], semver[2]);
	return err;
}

static int fifo_ready(const struct device *dev, uint8_t *slots_remaining)
{
	return ostentus_i2c_readbyte(dev, OSTENTUS_FIFO_READY, slots_remaining);
}

static int reset(const struct device *dev)
{
	uint8_t magic = OSTENTUS_RESET_MAGIC;
	return ostentus_i2c_write1(dev, OSTENTUS_RESET, &magic, 1);
}

static int led_bitmask(const struct device *dev, uint8_t bitmask)
{
	return ostentus_i2c_write1(dev, OSTENTUS_LED_BITMASK, &bitmask, 1);
}

static int led_power_set(const struct device *dev, uint8_t state)
{
	uint8_t byte = state ? 1 : 0;
	return ostentus_i2c_write1(dev, OSTENTUS_LED_POW, &byte, 1);
}

static int led_battery_set(const struct device *dev, uint8_t state)
{
	uint8_t byte = state ? 1 : 0;
	return ostentus_i2c_write1(dev, OSTENTUS_LED_BAT, &byte, 1);
}

static int led_internet_set(const struct device *dev, uint8_t state)
{
	uint8_t byte = state ? 1 : 0;
	return ostentus_i2c_write1(dev, OSTENTUS_LED_INT, &byte, 1);
}

static int led_golioth_set(const struct device *dev, uint8_t state)
{
	uint8_t byte = state ? 1 : 0;
	return ostentus_i2c_write1(dev, OSTENTUS_LED_GOL, &byte, 1);
}

static int led_user_set(const struct device *dev, uint8_t state)
{
	uint8_t byte = state ? 1 : 0;
	return ostentus_i2c_write1(dev, OSTENTUS_LED_USE, &byte, 1);
}

static int store_text(const struct device *dev, char *str, uint8_t len)
{
	return ostentus_i2c_write1(dev, OSTENTUS_STORE_TEXT, str, len);
}

static int write_text(const struct device *dev, uint8_t x, uint8_t y, uint8_t thickness)
{
	uint8_t data[] = {x, y, thickness};
	return ostentus_i2c_write1(dev, OSTENTUS_WRITE_TEXT, data, sizeof(data));
}

static const struct ostentus_driver_api ostentus_api = {
	.ostentus_clear_memory = &clear_memory,
	.ostentus_show_splash = &show_splash,
	.ostentus_update_display = &update_display,
	.ostentus_update_thickness = &update_thickness,
	.ostentus_update_font = &update_font,
	.ostentus_clear_text_buffer = &clear_text_buffer,
	.ostentus_clear_rectangle = &clear_rectangle,
	.ostentus_slide_add = &slide_add,
	.ostentus_slide_set = &slide_set,
	.ostentus_summary_title = &summary_title,
	.ostentus_slideshow = &slideshow,
	.ostentus_version_get = &version_get,
	.ostentus_fifo_ready = &fifo_ready,
	.ostentus_reset = &reset,
	.ostentus_led_bitmask = &led_bitmask,
	.ostentus_led_power_set = &led_power_set,
	.ostentus_led_battery_set = &led_battery_set,
	.ostentus_led_internet_set = &led_internet_set,
	.ostentus_led_golioth_set = &led_golioth_set,
	.ostentus_led_user_set = &led_user_set,
	.ostentus_store_text = &store_text,
	.ostentus_write_text = &write_text,
	.ostentus_i2c_readbyte = &i2c_readbyte,
	.ostentus_i2c_readarray = &i2c_readarray,
};

static int ostentus_init(const struct device *dev)
{
	const struct ostentus_config *config = dev->config;

	if (!device_is_ready(config->i2c.bus)) {
		LOG_ERR("I2C bus device not ready");
		return -ENODEV;
	}

	char buf[32];
	int err = version_get(dev, buf, 32);
	if (err) {
		LOG_ERR("Unable to communicate with Ostentus over i2c: %d", err);
		return err;
	} else {
		LOG_INF("Ostentus firmware version: %s", buf);
	}

	return 0;
}

#define OSTENTUS_DEFINE(inst)                                                                      \
	static const struct ostentus_config ostentus_config_##inst = {                             \
		.i2c = I2C_DT_SPEC_INST_GET(inst),                                                 \
	};                                                                                         \
                                                                                                   \
	DEVICE_DT_INST_DEFINE(inst, ostentus_init, NULL, NULL, &ostentus_config_##inst,            \
			      POST_KERNEL, 90, &ostentus_api);

DT_INST_FOREACH_STATUS_OKAY(OSTENTUS_DEFINE)

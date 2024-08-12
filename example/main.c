/*
 * Copyright (c) 2022-2023 Golioth, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(golioth_rd_template, LOG_LEVEL_DBG);

#include <libostentus.h>
#include <libostentus_regmap.h>

static const struct device *o_dev = DEVICE_DT_GET_ANY(golioth_ostentus);

int main(void) {
	char msg[32] = { 0 };

	ostentus_reset(o_dev);
	k_msleep(300); /* sleep at least 300ms for Ostentus to reboot */
	ostentus_version_get(o_dev, msg, sizeof(msg));
	LOG_INF("Ostentus firmare reports version: %s", msg);

	/* Update Ostentus LEDS using bitmask */
	ostentus_led_bitmask(o_dev, LED_USE | LED_GOL | LED_INT | LED_BAT | LED_POW);

	/* Show Golioth Logo */
	ostentus_show_splash(o_dev);


	k_sleep(K_MSEC(2000));
	ostentus_clear_memory(o_dev);

	/* Update Ostentus LEDS so only Power is on using bitmask */
	ostentus_led_bitmask(o_dev, 0x10);

	/* Write some text to the screen */
	ostentus_update_thickness(o_dev, 3); // Set pen thickness
	ostentus_update_font(o_dev, 0); // Choose sans font
	snprintk(msg, sizeof(msg), "%s", "Manually");
	ostentus_store_text(o_dev, msg, strlen(msg)); // Write message to data buffer
	ostentus_write_text(o_dev, 3, 60, 10); // Write data buffer text at x=3, y=60 scale=1.0

	snprintk(msg, sizeof(msg), "%s", "Show");
	ostentus_clear_text_buffer(o_dev);
	ostentus_store_text(o_dev, msg, strlen(msg)); // Write message to data buffer
	ostentus_write_text(o_dev, 3, 120, 17); // Write data buffer text at x=3, y=120 scale=1.7

	snprintk(msg, sizeof(msg), "%s", "Some Text");
	ostentus_clear_text_buffer(o_dev);
	ostentus_store_text(o_dev, msg, strlen(msg)); // Write message to data buffer
	ostentus_write_text(o_dev, 3, 180, 10); // Write data buffer text at x=3, y=180 scale=1.0

	ostentus_update_display(o_dev);
	k_sleep(K_MSEC(3000));

	/* Write different text over what was just written */
	ostentus_clear_rectangle(o_dev, 0, 100, 200, 40); // Clear last number from framebuffer
	snprintk(msg, sizeof(msg), "%s", "Rewrite");
	ostentus_clear_text_buffer(o_dev);
	ostentus_store_text(o_dev, msg, strlen(msg)); // Write message to data buffer
	ostentus_write_text(o_dev, 3, 120, 17); // Write data buffer text at x=3, y=120 scale=1.7

	ostentus_update_display(o_dev);
	k_sleep(K_MSEC(3000));

	/* Set up sensor display slides for slideshow */
	ostentus_slide_add(o_dev, 1, "Temperature", strlen("Temperature"));
	ostentus_slide_add(o_dev, 2, "Pressure", strlen("Pressure"));
	/* Set the title ofthe Ostentus summary slide (optional) */
	ostentus_summary_title(o_dev, "Weather:", strlen("Weather:"));
	ostentus_slideshow(o_dev, 30000);

	/* Simulated values */
	uint8_t whole = 26;
	uint8_t decimal = 0;
	uint8_t bitmask = LED_USE;
	while(1) {
		/* Write new LED values */
		ostentus_led_bitmask(o_dev, bitmask);

		/* Write number "##.#" to slide id=1 */
		snprintk(msg, 6, "%d.%d", whole, decimal);
		ostentus_slide_set(o_dev, 1, msg, strlen(msg));

		/* Increment the simulated sensor value */
		++decimal;
		if (decimal > 9) {
			decimal = 0;
			++whole;
		}

		/* Shift the LED mask */
		bitmask <<= 1;
		if (bitmask >= (1<<5)) {
			bitmask = 0x01;
		}

		k_sleep(K_SECONDS(1));
	}
}

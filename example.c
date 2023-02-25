/*
 * Copyright (c) 2023 Golioth, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdint.h>
#include "libostentus.h"

/* Add ostentus.c to the CMakeLists.txt */

/*
 * This demonstrates using the libostentus.c helper functions to write to the
 * Ostentus board.
 */
int main(void) {
	char msg[32];
	/* Update Ostentus LEDS using bitmask */
	led_bitmask(0x1F);
	
	/* Show Golioth Logo */
	show_splash();


	k_sleep(K_MSEC(2000));
	clear_memory();

	/* Update Ostentus LEDS so only Power is on using bitmask */
	led_bitmask(0x10);

	/* Write some text to the screen */
	update_thickness(3); // Set pen thickness
	update_font(0); // Choose sans font
	snprintk(msg, sizeof(msg), "%s", "Manually");
	store_text(msg, strlen(msg)); // Write message to data buffer
	write_text(3, 60, 10); // Write data buffer text at x=3, y=60 scale=1.0

	snprintk(msg, sizeof(msg), "%s", "Show");
	clear_text_buffer();
	store_text(msg, strlen(msg)); // Write message to data buffer
	write_text(3, 120, 17); // Write data buffer text at x=3, y=120 scale=1.7

	snprintk(msg, sizeof(msg), "%s", "Some Text");
	clear_text_buffer();
	store_text(msg, strlen(msg)); // Write message to data buffer
	write_text(3, 180, 10); // Write data buffer text at x=3, y=180 scale=1.0

	update_display();
	k_sleep(K_MSEC(3000));

	/* Write different text over what was just written */
	clear_rectangle(0, 100, 200, 40); // Clear last number from framebuffer
	snprintk(msg, sizeof(msg), "%s", "Rewrite");
	clear_text_buffer();
	store_text(msg, strlen(msg)); // Write message to data buffer
	write_text(3, 120, 17); // Write data buffer text at x=3, y=120 scale=1.7

	update_display();
	k_sleep(K_MSEC(3000));

	/* Set up sensor display slides for slideshow */
	slide_add(1, "Temperature", strlen("Temperature"));
	slide_add(2, "Pressure", strlen("Pressure"));
	/* Set the title ofthe Ostentus summary slide (optional) */
	summary_title("Weather:", strlen("Weather:"));
	slideshow(30000);

	/* Simulated values */
	uint8_t whole = 26;
	uint8_t decimal = 0;
	uint8_t bitmask = 0x01;
	while(1) {
		/* Write new LED values */
		led_bitmask(bitmask);

		/* Write number "##.#" to slide id=1 */
		snprintk(msg, 6, "%d.%d", whole, decimal);
		slide_set(1, msg, strlen(msg));

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

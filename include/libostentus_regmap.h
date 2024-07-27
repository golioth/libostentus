/*
 * Copyright (c) 2023 Golioth, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __LIBOSTENTUS_REGMAP_H__
#define __LIBOSTENTUS_REGMAP_H__

#define LED_USE 0x01
#define LED_GOL 0x02
#define LED_INT 0x04
#define LED_BAT 0x08
#define LED_POW 0x10

#define OSTENTUS_CLEAR_MEM     0x00
#define OSTENTUS_REFRESH       0x01
#define OSTENTUS_ADDR_X	       0x02
#define OSTENTUS_ADDR_Y	       0x03
#define OSTENTUS_SPLASHSCREEN  0x04
#define OSTENTUS_THICKNESS     0x05
#define OSTENTUS_FONT	       0x06
#define OSTENTUS_WRITE_TEXT    0x07
#define OSTENTUS_CLEAR_TEXT    0x08
#define OSTENTUS_CLEAR_RECT    0x09
#define OSTENTUS_SLIDE_ADD     0x0A
#define OSTENTUS_SLIDE_SET     0x0B
#define OSTENTUS_SLIDESHOW     0x0C
#define OSTENTUS_SUMMARY_TITLE 0x0D
#define OSTENTUS_LED_USE       0x10
#define OSTENTUS_LED_GOL       0x11
#define OSTENTUS_LED_INT       0x12
#define OSTENTUS_LED_BAT       0x13
#define OSTENTUS_LED_POW       0x14
#define OSTENTUS_LED_BITMASK   0x18
#define OSTENTUS_STRING_0      0x20
#define OSTENTUS_STRING_1      0x21
#define OSTENTUS_STRING_2      0x22
#define OSTENTUS_STRING_3      0x23
#define OSTENTUS_STRING_4      0x24
#define OSTENTUS_STRING_5      0x25
#define OSTENTUS_STORE_TEXT    0x26
#define OSTENTUS_GET_VERSION   0x30
#define OSTENTUS_FIFO_READY    0x31
#define OSTENTUS_RESET	       0x3F

/* Magic number to verify reset command was intentional */
#define OSTENTUS_RESET_MAGIC 0xA5

#endif

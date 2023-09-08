/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#include "dmx512.h"

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS 500

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

static struct dmx_universe * mydmx;

struct point{
	uint32_t x;
	uint32_t y;
};

void main(void)
{
	int ret;
	int i;
	uint32_t * u32_ptr;

	// struct point pt;

	// pt.x = 5;
	// pt.y = 99;

	// printf("point x: %d   y: %d\n", pt.x, pt.y);
	// pt.x++;
	// pt.y++;

	// printf("point x: %d   y: %d\n", pt.x, pt.y);



	printf("Starting DMX Gadget...\n");

	// struct dmx_universe dmx_local;
	// dmx_local.max_slot = 13;
	// printf("dmx local.max slots %d\n", dmx_local.max_slot);

	// u32_ptr=  malloc(15);
	// dmx_local.slot_data = u32_ptr;
	// dmx_set_slot(&dmx_local, 7, 129);

	// dmx_print(&dmx_local);

	// struct dmx_universe * dmx_ptr_local;
	// dmx_ptr_local = malloc(sizeof(struct dmx_universe));
    // dmx_ptr_local->max_slot = 13;
	// u32_ptr = malloc(10);
	// dmx_ptr_local->slot_data = u32_ptr;
	// dmx_set_slot(dmx_ptr_local, 5, 127);
	// dmx_print(dmx_ptr_local);


	ret = dmx_create(24, &mydmx);

	if (ret != 0) {
		return;
	}

	dmx_print(mydmx);

	for (i = 0; i < 24; i++) {
		dmx_set_slot(mydmx, i, i * 2);
	}
	
	dmx_print(mydmx);

	ret = dmx_destroy(mydmx);

	if (ret != 0) {
		return;
	}

	if (!gpio_is_ready_dt(&led))
	{
		return;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0)
	{
		return;
	}

	while (1)
	{
		ret = gpio_pin_toggle_dt(&led);
		if (ret < 0)
		{
			return;
		}
		printf("led toggle\n");
		k_msleep(SLEEP_TIME_MS);
	}
}

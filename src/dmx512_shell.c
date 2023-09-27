// Copyright 2023 Michael Bradley
// This file implements a DMX512 Universe zephyr shell

#include <stdint.h>

#include <zephyr/shell/shell.h>

#include "dmx512.h"

extern struct dmx_universe * mydmx;

static int cmd_dmx_init(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	// shell_print(sh, "Printing DMX512 Universe");
	return dmx_create(&mydmx, 24);
}

static int cmd_dmx_print(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	int32_t ret;

	ret = dmx_print(mydmx);
	
	if (ret) {
		shell_print(sh, "Error %d", ret);
	}

	return ret;
}

static int cmd_dmx_slot(const struct shell *sh, size_t argc, char **argv)
{
    int32_t ret = 0;
    uint32_t slot_value_read;
    uint8_t slot_value;
    uint16_t slot;

    switch(argc) {
        case 3:
            slot_value_read = atoi(argv[2]);
            if (slot_value_read > 255) {
                ret = -EINVAL;
                shell_print(sh, "Error %d", ret);
                return (ret);
            }
            slot_value = (uint8_t)slot_value_read;
            // fall through is required here
        case 2:
            slot = atoi(argv[1]);
            break;

        default:
            shell_print(sh, " Expected syntax:\n  slot <slot number>  [write value]");
            return (-EINVAL);
    }

    if (argc == 2) {
        // Read and display the slot value
        ret = dmx_get_slot(mydmx, slot, &slot_value);
        if (!ret) {
            shell_print(sh, "%3d", slot_value);
        }
    } else if (argc == 3) {
        // Write slot to value
        ret = dmx_set_slot(mydmx, slot, slot_value);
    }

    if (ret) {
		shell_print(sh, "Error %d", ret);
	}

	return ret;
}

static int cmd_dmx_del(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	// shell_print(sh, "Printing DMX512 Universe");
	return dmx_destroy(&mydmx);
}

SHELL_STATIC_SUBCMD_SET_CREATE(dmx_cmds,
	SHELL_CMD(init, NULL, "Create DMX512 Universe", cmd_dmx_init),
	SHELL_CMD(print, NULL, "Print DMX512 Universe", cmd_dmx_print),
    SHELL_CMD(slot, NULL, "Set/Get a slot value", cmd_dmx_slot),
	SHELL_CMD(del, NULL, "Destroy DMX512 Universe", cmd_dmx_del),
	SHELL_SUBCMD_SET_END /* Array terminated. */
);
SHELL_CMD_REGISTER(dmx, &dmx_cmds, "DMX512 commands", NULL);
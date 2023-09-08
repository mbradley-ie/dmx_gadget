// Copyright 2023 Michael Bradley
// This header defines the DMX512 Universe public API

#include <stdint.h>

// A DMX512 universe consists of 
// A start code/slot 0 that defines the type of data/package being sent
// 0 or more byte values for the up to 512 slots of data, typically lighting channels.
//   Note: the DMX512 standard at the protocol level requires a minimum packet length
//   (typically 24 slots) but that is not enforced in this data structure.
// struct is considered private, so no members defined here

//struct dmx_universe;
// TODO remove this later
struct dmx_universe {
    uint32_t start_code;
    uint32_t max_slot;
    uint32_t * slot_data;
};

// Set a slot to a value, ensuring slot is valid
int32_t dmx_set_slot(struct dmx_universe * dmx_uni, uint16_t slot, uint8_t value);

// prints a dmx universe to the console
int32_t dmx_print(struct dmx_universe * dmx_uni);

// Creates a DMX512 universe
int32_t dmx_create(uint16_t slot_count, struct dmx_universe ** dmx_uni );

// Destroy a DMX512 universe
int32_t dmx_destroy(struct dmx_universe * dmx_uni);

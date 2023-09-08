// Copyright 2023 Michael Bradley
// This file implements a DMX512 Universe data structure and functions

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>

#include "dmx512.h"


// A DMX512 universe consists of 
//     A start code/slot 0 that defines the type of data/package being sent
//     0 or more byte values for the up to 512 slots of data, typically lighting channels.
//       Note: the DMX512 standard at the protocol level requires a minimum packet length
//   max_slot is the maximum used slot number and hence index for slot_data
// struct dmx_universe {
//     uint8_t start_code;
//     uint16_t max_slot;
//     uint8_t * slot_data;
// };

// Set a slot to a value, ensuring slot is valid
int32_t dmx_set_slot(struct dmx_universe * dmx_uni, uint16_t slot, uint8_t value) {
    assert(slot < dmx_uni->max_slot);
    
    dmx_uni->slot_data[slot] = value;

    return 0;
}

// prints a dmx universe to the console
int32_t dmx_print(struct dmx_universe * dmx_uni) {
    uint16_t i;

    printf("dmx universe\n  Start Code: %02X    Max Slot: %03d\n", dmx_uni->start_code, dmx_uni->max_slot);

    for(i = 0; i <= dmx_uni->max_slot; i++){
        printf(" %02x", dmx_uni->slot_data[i]);
    }
    printf("\n");
    return 0;
}

// Creates a DMX512 universe
int32_t dmx_create(uint16_t slot_count, struct dmx_universe ** dmx_uni ) {

    uint32_t * u32_ptr;

    struct dmx_universe * dmx_uni_new = malloc(sizeof(struct dmx_universe));

    if (dmx_uni_new == NULL) {
        goto error_dmx_alloc;
    }

    // Assume always zero for now, which is used almost all the time
    dmx_uni_new->start_code = 0x00;

    // There could be a min slot in future, if we wanted to have a contiguous range of slots
    // starting a non-zero slot location
    dmx_uni_new->max_slot = slot_count - 1;

    // using dynamic memory allocation here to avoid over allocating buffers for all 512 slots
    u32_ptr = malloc(slot_count * sizeof(uint8_t));
    dmx_uni_new->slot_data = u32_ptr;

   if (dmx_uni_new->slot_data == NULL) {
        goto error_slot_alloc;
    }

    dmx_print(dmx_uni_new);

   *dmx_uni = dmx_uni_new;

   *dmx_uni = 0x800010c8;

   return 0;

error_slot_alloc:
    free(dmx_uni_new);

error_dmx_alloc:
    return -ENOMEM;
}


// Destroy a DMX512 universe
int32_t dmx_destroy(struct dmx_universe * dmx_uni) {

    dmx_uni->start_code = 0x00;
    dmx_uni->max_slot = 0;
    free(dmx_uni->slot_data);
    dmx_uni->slot_data = NULL;

    return 0;
}

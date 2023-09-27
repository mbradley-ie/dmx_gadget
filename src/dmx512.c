// Copyright 2023 Michael Bradley
// This file implements a DMX512 Universe data structure and functions

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>

#include "dmx512.h"

// Set a slot to a value, ensuring slot is valid
int32_t dmx_set_slot(struct dmx_universe * dmx_uni, uint16_t slot, uint8_t value) {
    if (!dmx_uni || slot > dmx_uni->max_slot) {
        return -EINVAL;
    };
    
    dmx_uni->slot_data[slot] = value;

    return 0;
}

// Get a slot to a value, ensuring slot is valid
int32_t dmx_get_slot(struct dmx_universe * dmx_uni, uint16_t slot, uint8_t * value) {
    if (!dmx_uni || slot > dmx_uni->max_slot) {
        return -EINVAL;
    };
    
    *value = dmx_uni->slot_data[slot];

    return 0;
}
// prints a dmx universe to the console
int32_t dmx_print(struct dmx_universe * dmx_uni) {
    uint16_t i;

    if (!dmx_uni) {
        return -EINVAL;
    }

    printf("dmx universe @0x%p\n Start Code: 0x%02X    Max Slot: %0d\n", dmx_uni, dmx_uni->start_code, dmx_uni->max_slot);

    for(i = 0; i <= dmx_uni->max_slot; i++){
        printf(" %3d", dmx_uni->slot_data[i]);
    }
    printf("\n");
    return 0;
}

// Creates a DMX512 universe
int32_t dmx_create(struct dmx_universe ** dmx_uni, uint16_t slot_count) {

    struct dmx_universe * dmx_uni_new;
    dmx_uni_new = (struct dmx_universe *) malloc(sizeof(struct dmx_universe));

    if (dmx_uni_new == NULL) {
        goto error_dmx_alloc;
    }

    // Assume always zero for now, which is used almost all the time
    dmx_uni_new->start_code = 0x00;

    // There could be a min slot in future, if we wanted to have a contiguous range of slots
    // starting a non-zero slot location
    dmx_uni_new->max_slot = slot_count - 1;

    // using dynamic memory allocation here to avoid over allocating buffers for all 512 slots
    // TODO will be dyanmic in a while
    // u32_ptr = malloc(slot_count * sizeof(uint8_t));
    // dmx_uni_new->slot_data = u32_ptr;

    // will use calloc to create zero value array but for now
    for (int i = 0; i < slot_count; i++) {
        dmx_uni_new->slot_data[i] = 0x00;
    }

//    if (dmx_uni_new->slot_data == NULL) {
//         goto error_slot_alloc;
//     }
    printf("--Created DMX Universe\n");
    dmx_print(dmx_uni_new);

   *dmx_uni = dmx_uni_new;

   return 0;

// error_slot_alloc:
//     free(dmx_uni_new);

error_dmx_alloc:
    printf("Error creating dmx universe\n");
    return -ENOMEM;
}

// Destroy a DMX512 universe
int32_t dmx_destroy(struct dmx_universe ** dmx_uni) {

    (*dmx_uni)->start_code = 0xFF;
    (*dmx_uni)->max_slot = 0;
    //free(dmx_uni->slot_data);
    // TODO dmx_uni->slot_data = NULL;
    free(*dmx_uni);
    *dmx_uni = NULL;

    return 0;
}

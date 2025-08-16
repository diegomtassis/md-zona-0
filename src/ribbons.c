/*
 * Filename: ribbons.c
 * Created Date: August 16th 2025
 * Author: diegomtassis
 */

#include "ribbons.h"

#include "fwk/vdp_utils.h"
#include "gfx_grid.h"

#include <tools.h>

static RibbonSegment *createRibbonSegment(Ribbon *ribbon, u8 direction, V2s16 mapPosBegin);
static void releaseRibbonSegment(RibbonSegment *segment);

u16 ribbonVramBaseTile;

void RIBBONS_init(u16 vramBaseTile) {
    ribbonVramBaseTile = vramBaseTile;
    VDP_loadTileSet(&tileset_ribbons, vramBaseTile, DMA);
}

void RIBBONS_initRibbon(Ribbon *ribbon, RibbonType type, u8 direction, V2s16 mapPos) {
    if (!ribbon) {
        return;
    }

    ribbon->baseTile = 0; // Will be set by grid unit
    ribbon->type = type;  // Set the ribbon type during initialization
    DLL_init(&ribbon->segments);

    // Create the first ribbon segment
    RibbonSegment *firstSegment = createRibbonSegment(ribbon, direction, mapPos);
    if (firstSegment) {
        DLL_add(&ribbon->segments, firstSegment);
    }
}

void RIBBONS_clearRibbon(Ribbon *ribbon) {
    if (!ribbon) {
        return;
    }

    // Release all ribbon segments
    DLLNode *node = ribbon->segments.first;
    while (node) {
        releaseRibbonSegment((RibbonSegment *)node->e);
        node = node->next;
    }

    DLL_release(&ribbon->segments);

    // Reset the ribbon structure
    ribbon->baseTile = 0;
    ribbon->type = JETWALL_FLASH;
}

static RibbonSegment *createRibbonSegment(Ribbon *ribbon, u8 direction, V2s16 mapPosBegin) {

    RibbonSegment *segment = MEM_alloc(sizeof(RibbonSegment));
    if (!segment) {
        return NULL;
    }

    segment->ribbon = ribbon;
    segment->direction = direction;
    segment->mapPosBegin = mapPosBegin;
    segment->mapPosEnd = mapPosBegin; // Initially same as begin
    DLL_init(&segment->steps);

    return segment;
}

static void releaseRibbonSegment(RibbonSegment *segment) {
    if (!segment) {
        return;
    }

    // Release all steps in the segment
    DLLNode *node = segment->steps.first;
    while (node) {
        RIBBONS_releaseRibbonStep((RibbonStep *)node->e);
        node = node->next;
    }

    DLL_release(&segment->steps);
    MEM_free(segment);
}

RibbonSegment *RIBBONS_beginSegment(Ribbon *ribbon, u8 direction, V2s16 mapPosBegin) {
    if (!ribbon) {
        return NULL;
    }

    // If there are existing segments, update the mapPosEnd of the last segment
    if (ribbon->segments.last) {
        RibbonSegment *lastSegment = (RibbonSegment *)ribbon->segments.last->e;
        lastSegment->mapPosEnd = mapPosBegin;
    }

    // Create the new segment with the provided mapPosBegin and ribbon pointer
    RibbonSegment *newSegment = createRibbonSegment(ribbon, direction, mapPosBegin);
    if (newSegment) {
        DLL_add(&ribbon->segments, newSegment);
    }

    return newSegment;
}

RibbonStep *RIBBONS_createRibbonStep(RibbonSegment *segment, V2s16 mapPos, bool first) {
    RibbonStep *step = MEM_alloc(sizeof(RibbonStep));
    if (!step) {
        return NULL;
    }

    step->segment = segment;
    step->mapPos = mapPos;
    step->first = first;

    DLL_add(&segment->steps, step);

    return step;
}

void RIBBONS_releaseRibbonStep(RibbonStep *step) {
    if (step) {
        MEM_free(step);
    }
}

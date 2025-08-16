/*
 * Filename: ribbons.h
 * Created Date: August 16th 2025
 * Author: diegomtassis
 */

#ifndef INC_RIBBONS_H_
#define INC_RIBBONS_H_

#include <genesis.h>

#include "fwk/doubly_linked_list.h"

// Forward declaration
typedef struct Ribbon Ribbon;
typedef struct RibbonSegment RibbonSegment;

typedef enum {
    JETWALL_RED = 0,
    JETWALL_GREEN = 1,
    JETWALL_FLASH = 2
} RibbonType;

extern u16 ribbonVramBaseTile;

typedef struct {
    RibbonSegment* segment;
    V2s16 mapPos;
    bool first;
} RibbonStep;

struct RibbonSegment {
    Ribbon* ribbon;
    u8 direction;
    V2s16 mapPosBegin;
    V2s16 mapPosEnd;
    DLL steps;
};

struct Ribbon {
    u16 baseTile;
    RibbonType type;
    DLL segments;
    RibbonStep* stepTZero;
    RibbonStep* stepTMinus;
};

// Ribbon management functions
void RIBBONS_init(u16 vramBaseTile);
void RIBBONS_initRibbon(Ribbon *ribbon, RibbonType type, u8 direction, V2s16 mapPos);
void RIBBONS_clearRibbon(Ribbon *ribbon);
RibbonSegment* RIBBONS_beginSegment(Ribbon *ribbon, u8 direction, V2s16 mapPosBegin);

// Ribbon step functions
RibbonStep *RIBBONS_createRibbonStep(RibbonSegment* segment, V2s16 mapPos, bool first);
void RIBBONS_releaseRibbonStep(RibbonStep *step);

#endif /* INC_RIBBONS_H_ */

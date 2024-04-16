#ifndef __MEMORY_REPORTER_H__
#define __MEMORY_REPORTER_H__

#include <Arduino.h>  // Include to get the definition of const __FlashStringHelper*

/*
------------------------
__heap_start        : A Seperator, which address is at the top of heap(about 454), which value is 12;
__malloc_heap_start : A Static Variable, which address is at 258 (Static Variable Section),
                    which value is the top of heap(about 454);
__brkval            : A Counter, which address is at 450(2 uint16_t before the top of the heap 454),
                    which value is initially 0, then becomes the bottom of the heap(468 in this example)
                    after a new[] or malloc() operation.
__flp               : A Counter, which address is at 452(1 uint16_t before the top of the heap 454),
                    which value is initially 0, then becomes the top of the heap(454 in this example),
                    and it reassigned to 0 after all dynaArrays delete[]d,
                    however, __brkval won't reassigned to 0 after all dynaArrays delete[]d, but 454(Heap Top).
------------------------
*/

extern int stopOptimization;
extern char *__brkval;
extern char *__flp;
extern char *__heap_start;

// ATmega 328P Microcontroller's SRAM starts at 256, first available being [256]
#define GetStaticTop() (256)
unsigned int GetStaticBottom();
unsigned int GetHeapTop();
unsigned int GetHeapBottom();
unsigned int GetStackTop();
// ATmega 328P Microcontroller's SRAM ends at 2304, last available being [2303]
#define GetStackBottom() (2304)

// Parameter: an F("") string, normal one won't do.
void ReportMemoryStatus(const __FlashStringHelper* msg);

#endif

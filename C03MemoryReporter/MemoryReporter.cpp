#include "MemoryReporter.h"

unsigned int GetStaticBottom(){
  // Return the &__heap_start as Static Bottom.
  return (unsigned int)&__heap_start;
}

unsigned int GetHeapTop() {
  if (__flp == 0) {
    // When no malloc() or new[] was called, __brkval could be 0 (which means FALSE).
    return (unsigned int)&__heap_start;
  }
  else {
    // Return the __flp as Heap Top.
    return (unsigned int) __flp;
  }
}

unsigned int GetHeapBottom() {
  if (__brkval == 0) {
    // When no malloc() or new[] was called, __brkval could be 0 (which means FALSE).
    return (unsigned int)&__heap_start;
  }
  else {
    // Return the __brkval as Heap Bottom.
    return (unsigned int) __brkval;
  }
}

unsigned int GetStackTop() {
  // Latest Local Variable in Lastest Called Function = Stack Top
  char stackTop;

  // Must subtract an external int to disable compiler optimization.
  return &stackTop - stopOptimization;
}

/*
** ReportMemoryStatus()
** Input: a message string to signify the current state, will be printed to Serial for inspection.
**      : MUST BE an F(""), normal string won't work.
*/

void ReportMemoryStatus(const __FlashStringHelper* msg){
  unsigned int top, bottom;

  // Identify Message
  Serial.println(msg);

  // Static Section
  top = GetStaticTop();
  bottom = GetStaticBottom();
  Serial.print(F("Static:\t\t"));
  Serial.print(top);
  Serial.print(F("\t=>\t"));
  Serial.print(bottom);
  Serial.print(F("\t\t@\t"));
  Serial.print(bottom - top);

  // Heap Section
  top = GetHeapTop();
  bottom = GetHeapBottom();
  Serial.print(F("\nHeap:\t\t"));
  Serial.print(top);
  Serial.print(F("\t=>\t"));
  Serial.print(bottom);
  Serial.print(F("\t\t@\t"));
  Serial.print(bottom - top);

  // Free Section
  top = GetStackTop();
  // bottom = GetHeapBottom(); // Still
  Serial.print(F("\nFree:\t\t"));
  Serial.print(bottom);
  Serial.print(F("\t=>\t"));
  Serial.print(top);
  Serial.print(F("\t\t@\t"));
  Serial.print(top - bottom);

  // Stack Section
  // top = GetStackTop();  // Still
  bottom = GetStackBottom();
  Serial.print(F("\nStack:\t\t"));
  Serial.print(top);
  Serial.print(F("\t=>\t"));
  Serial.print(bottom);
  Serial.print(F("\t\t@\t"));
  Serial.print(bottom - top);
  Serial.print(F("\n"));

  // Seperator
  for(int i = 0; i < 80; i++){
    Serial.print(F("-"));
  }
  Serial.print(F("\n"));
}

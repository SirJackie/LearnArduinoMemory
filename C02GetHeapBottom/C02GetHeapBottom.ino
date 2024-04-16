extern int stopOptimization;
extern char *__brkval;
extern char *__flp;
extern char *__heap_start;

// ATmega 328P Microcontroller's SRAM starts at 256, first available being [256]
#define GetStaticTop() (256)

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

// ATmega 328P Microcontroller's SRAM ends at 2304, last available being [2303]
#define GetStackBottom() (2304)

void ReportMemoryStatus(){
  unsigned int top, bottom;

  // Static Section
  top = GetStaticTop();
  bottom = GetStaticBottom();
  Serial.print(F("Static Section: from "));
  Serial.print(top);
  Serial.print(F(" to "));
  Serial.print(bottom);
  Serial.print(F(", Total: "));
  Serial.print(bottom - top);

  // Heap Section
  top = GetHeapTop();
  bottom = GetHeapBottom();
  Serial.print(F("\nHeap Section: from "));
  Serial.print(top);
  Serial.print(F(" to "));
  Serial.print(bottom);
  Serial.print(F(", Total: "));
  Serial.print(bottom - top);

  // Free Section
  top = GetStackTop();
  // bottom = GetHeapBottom(); // Still
  Serial.print(F("\nFree Section: from "));
  Serial.print(bottom);
  Serial.print(F(" to "));
  Serial.print(top);
  Serial.print(F(", Total: "));
  Serial.print(top - bottom);

  // Stack Section
  // top = GetStackTop();  // Still
  bottom = GetStackBottom();
  Serial.print(F("\nStack Section: from "));
  Serial.print(top);
  Serial.print(F(" to "));
  Serial.print(bottom);
  Serial.print(F(", Total: "));
  Serial.print(bottom - top);

  Serial.println(F("\n---------------------------------------------------------"));
}

void setup() {
  Serial.begin(115200);

  ReportMemoryStatus();

  int* dynaArray = new int[6];

  for (int i = 0; i < 6; i++){  // No less than 6, otherwise compiler optimization will be triggered.
    dynaArray[i] = 127;
  }

  int* dynaArray2 = new int[6];

  for (int i = 0; i < 6; i++){  // No less than 6, otherwise compiler optimization will be triggered.
    dynaArray2[i] = 127;
  }

  ReportMemoryStatus();

  delete[] dynaArray;

  ReportMemoryStatus();

  delete[] dynaArray2;

  ReportMemoryStatus();

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
}

void loop() {

}

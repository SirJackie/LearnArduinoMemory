extern int stopOptimization;
extern char *__brkval;
extern char *__flp;
extern char *__heap_start;

unsigned int GetStackTop() {
  // Latest Local Variable in Lastest Called Function = Stack Top
  char stackTop;

  // Must subtract an external int to disable compiler optimization.
  return &stackTop - stopOptimization;
}

unsigned int GetHeapBottom() {
  return (unsigned int) __brkval;
}

unsigned int GetHeapTop() {
  return (unsigned int) __flp;
}

unsigned int GetStaticBottom(){
  return (unsigned int)&__heap_start;
}

void setup() {
  Serial.begin(115200);

  Serial.println(GetStaticBottom());
  Serial.println(GetHeapBottom());
  Serial.println(GetHeapTop());
  Serial.println(F("------------------------"));

  int* dynaArray = new int[6];

  for (int i = 0; i < 6; i++){  // No less than 6, otherwise compiler optimization will be triggered.
    dynaArray[i] = 127;
  }

  int* dynaArray2 = new int[6];

  for (int i = 0; i < 6; i++){  // No less than 6, otherwise compiler optimization will be triggered.
    dynaArray2[i] = 127;
  }

  Serial.println(GetStaticBottom());
  Serial.println(GetHeapBottom());
  Serial.println(GetHeapTop());
  Serial.println(F("------------------------"));

  delete[] dynaArray;

  Serial.println(GetStaticBottom());
  Serial.println(GetHeapBottom());
  Serial.println(GetHeapTop());
  Serial.println(F("------------------------"));

  delete[] dynaArray2;

  Serial.println(GetStaticBottom());
  Serial.println(GetHeapBottom());
  Serial.println(GetHeapTop());
  Serial.println(F("------------------------"));

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

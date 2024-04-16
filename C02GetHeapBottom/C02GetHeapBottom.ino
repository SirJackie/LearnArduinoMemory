extern int stopOptimization;
extern char *__brkval;
extern char *__malloc_heap_start;
extern char *__heap_start;
extern char *__flp;

int GetStackTop() {
  // Latest Local Variable in Lastest Called Function = Stack Top
  char stackTop;

  // Must subtract an external int to disable compiler optimization.
  return &stackTop - stopOptimization;
}

int GetHeapBottom() {
  return (unsigned int) __brkval;
}

void setup() {
  Serial.begin(115200);

  Serial.println((int)&__heap_start);
  Serial.println((int) __brkval);
  Serial.println((int) __flp);
  Serial.println((int)&__flp);
  Serial.println(F("------------------------"));

  int* dynaArray = new int[6];

  for (int i = 0; i < 6; i++){  // No less than 6, otherwise compiler optimization will be triggered.
    dynaArray[i] = 127;
  }

  int* dynaArray2 = new int[6];

  for (int i = 0; i < 6; i++){  // No less than 6, otherwise compiler optimization will be triggered.
    dynaArray2[i] = 127;
  }

  Serial.println((int)&__heap_start);
  Serial.println((int) __brkval);
  Serial.println((int) __flp);
  Serial.println((int)&__flp);
  Serial.println(F("------------------------"));

  delete[] dynaArray;

  Serial.println((int)&__heap_start);
  Serial.println((int) __brkval);
  Serial.println((int) __flp);
  Serial.println((int)&__flp);
  Serial.println(F("------------------------"));

  delete[] dynaArray2;

  Serial.println((int)&__heap_start);
  Serial.println((int) __brkval);
  Serial.println((int) __flp);
  Serial.println((int)&__flp);
  Serial.println(F("------------------------"));

  /*
  ** Result:
  (int)&__heap_start
  (int) __brkval
  (int) __flp
  (int)&__flp
  ------------------------
  454
  0
  0
  452
  ------------------------
  454
  482
  0
  452
  ------------------------
  454
  482
  454
  452
  ------------------------
  454
  454
  0
  452
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

extern int stopOptimization;
extern char *__brkval;
extern char *__malloc_heap_start;
extern char *__heap_start;

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
  // put your setup code here, to run once:
  Serial.begin(115200);
  // Serial.println(GetStackTop());
  // Serial.println(GetHeapBottom());
  // int* dynaVar = new int;
  // *dynaVar = 1234;
  // Serial.println(*dynaVar);
  // Serial.println(GetStackTop());
  // Serial.println((unsigned int) __brkval);

  Serial.println((int)&__heap_start);
  Serial.println((int) __brkval);

  int* dynaArray = new int[6];

  for (int i = 0; i < 6; i++){  // No less than 6, otherwise compiler optimization will be triggered.
    dynaArray[i] = 127;
  }

  // int* dynaVariable = new int;
  // *dynaVariable = 1234;
  // *dynaVariable = *dynaVariable * stopOptimization;

  Serial.println((int)&__heap_start);
  Serial.println((int) __brkval);
}

void loop() {
  // put your main code here, to run repeatedly:

}

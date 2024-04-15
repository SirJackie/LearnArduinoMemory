extern char *__malloc_heap_start;  // Initial Heap Top (Compiler's Inexplicit Variable).
// extern void *__brkval;  // Current Heap Top, remains 0 if no malloc() is called.
extern int __heap_start, *__brkval;

unsigned int GetHeapTop(){
  if ((int)__brkval == 0) {
    // If no malloc() is called, return the Static Top as the Heap Top.
    return (unsigned int)&__malloc_heap_start;
  } else {
    // If malloc() IS called, return the last malloc() address as the Heap Top.
    return (unsigned int)__brkval;
  }
}

unsigned int GetStackBottom(){
  char top = 'F';  // Newly Assigned Local Variable, SHOULD BE at the Stack Bottom.
  return (unsigned int)&top;  // Return Stack Bottom Address.
}

unsigned int GetMemoryStatus() {
  return GetHeapTop() - GetStackBottom();
}

int freeRam() {
  extern int __heap_start,*__brkval;
  int v;
  return (int)&v - (__brkval == 0  
    ? (int)&__heap_start : (int) __brkval);  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  // Serial.println(GetHeapTop());
  // Serial.println(GetStackBottom());
  // Serial.println(GetMemoryStatus());

  // Serial.println((__brkval == 0 ? (int)&__heap_start : (int) __brkval));




  // Serial.println((int)&__heap_start);
  // Serial.println((int) __brkval);

  // int* dynaArray = new int[256];

  // for (int i = 0; i < 256; i++){
  //   dynaArray[i] = 127;
  // }

  // Serial.println((int)&__heap_start);
  // Serial.println((int) __brkval);

  Serial.println(GetStackBottom());
}

void loop() {
  // put your main code here, to run repeatedly:

}

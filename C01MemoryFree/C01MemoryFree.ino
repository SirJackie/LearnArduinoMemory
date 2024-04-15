extern unsigned int __heap_start;  // Initial Heap Top (Compiler's Inexplicit Variable).
extern void *__brkval;  // Current Heap Top, remains 0 if no malloc() is called.

unsigned int GetHeapTop(){
  if ((int)__brkval == 0) {
    // If no malloc() is called, return the Static Top as the Heap Top.
    return (unsigned int)&__heap_start;
  } else {
    // If malloc() IS called, return the last malloc() address as the Heap Top.
    return (unsigned int)__brkval;
  }
}

unsigned int GetStackBottom(){
  char top;  // Newly Assigned Local Variable, SHOULD BE at the Stack Bottom.
  return (unsigned int)&top;  // Return Stack Bottom Address.
}

unsigned int GetMemoryStatus() {
  return GetHeapTop() - GetStackBottom();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(GetHeapTop());
  Serial.println(GetStackBottom());
  Serial.println(GetMemoryStatus());
}

void loop() {
  // put your main code here, to run repeatedly:

}

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int freeMemory() {
  char top;
  __brkval = 0;  // IF COMMENTED THIS LINE, RETURN VALUE: 2299 => 0!!! (Compiler Optimization maybe.)
  return &top - __brkval;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(freeMemory());
}

void loop() {
  // put your main code here, to run repeatedly:

}

extern int stopOptimization;

int freeMemory() {
  char top;
  return &top - stopOptimization;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(freeMemory());
}

void loop() {
  // put your main code here, to run repeatedly:

}

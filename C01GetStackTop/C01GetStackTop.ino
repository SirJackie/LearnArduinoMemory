extern int stopOptimization;

int GetStackTop() {
  // Latest Local Variable in Lastest Called Function = Stack Top
  char stackTop;

  // Must subtract an external int to disable compiler optimization.
  return &stackTop - stopOptimization;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(GetStackTop());
}

void loop() {
  // put your main code here, to run repeatedly:

}

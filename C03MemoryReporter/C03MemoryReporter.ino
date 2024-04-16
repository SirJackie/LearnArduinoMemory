#include "MemoryReporter.h"

void setup() {
  Serial.begin(115200);
  ReportMemoryStatus(F("Begin:"));

  int* dynaArray = new int[6];
  for (int i = 0; i < 6; i++){  // No less than 6, otherwise compiler optimization will be triggered.
    dynaArray[i] = 127;
  }
  int* dynaArray2 = new int[6];
  for (int i = 0; i < 6; i++){  // No less than 6, otherwise compiler optimization will be triggered.
    dynaArray2[i] = 127;
  }
  ReportMemoryStatus(F("New[]ed:"));

  delete[] dynaArray;
  ReportMemoryStatus(F("Delete[]d No.1:"));

  delete[] dynaArray2;
  ReportMemoryStatus(F("Delete[]d No.2:"));
}

void loop() {

}

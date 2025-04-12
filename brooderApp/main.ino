/**
 * @file      main.ino
 * @author    Ibrahim Hroob (ibrahim.hroub7@gmail.com)
 * @copyright Copyright (c) 2024  Dicam Technology Ltd
 * @date      2024-12-25
 *
 */


#include "D2BrooderAlarm/src/system.h"

void setup() {
  Serial.begin(115200);
  MonitorSystem.Init();
}

void loop() { 
  MonitorSystem.Run();
}
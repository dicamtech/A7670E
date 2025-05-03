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

// python $IDF_PATH/components/espcoredump/espcoredump.py   --port /dev/ttyACM0 info_corefile   /home/ibrahim/dicam/A7670E/.pio/build/T-A7670X/firmware.elf

/*
#include <Arduino.h>
#include <ctype.h>               // for isprint()
#include "esp_partition.h"

void setup() {
  Serial.begin(115200);
  const esp_partition_t* part = esp_partition_find_first(
      ESP_PARTITION_TYPE_DATA,
      ESP_PARTITION_SUBTYPE_DATA_COREDUMP,
      "coredump"
  );
  if (!part) {
    Serial.println("Partition not found");
    return;
  }

  const size_t PART_SIZE = part->size;
  const size_t CHUNK     = 1024;
  uint8_t buffer[CHUNK];
  size_t offset = 0;

  while (offset < PART_SIZE) {
    size_t to_read = min(CHUNK, PART_SIZE - offset);
    if (esp_partition_read(part, offset, buffer, to_read) != ESP_OK) {
      Serial.printf("Read error at 0x%06X\n", offset);
      break;
    }
    // Option A: bulk write as chars
    // Serial.write((const char*)buffer, to_read);

    // Option B: per-byte with filtering
    
    for (size_t i = 0; i < to_read; ++i) {
      char c = (char)buffer[i];
      if (isprint(c)) {
        Serial.print(c);
      } else {
        // Serial.print('.');
      }
    }
    

    offset += to_read;
  }
  Serial.println("\nDone");
}

void loop() {
  // nothing here
}
*/
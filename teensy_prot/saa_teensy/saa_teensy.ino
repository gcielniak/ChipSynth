#include "SAA1099.h"

SoundChip sound_chip;

void setup() {
  Serial.begin(57600);

  sound_chip.Init();

  sound_chip.Reset();

// short test tone procedure  
//  sound_chip.Write(0x00, 0xFF);//volume0 left F right F
//  sound_chip.Write(0x10, 0x04);//octave0 6
//  sound_chip.Write(0x14, 0x01);//all freq enabled

}

char string_buffer[256];
uint16_t data_buffer[256];

void loop() {
  char *s_start = string_buffer, *s_end;
  unsigned short bytes_read;
  
  while (Serial.available()) {
    bytes_read = 0;
    Serial.readBytesUntil('\n', string_buffer, 256);

    while (true) {
      int value = strtol(s_start, &s_end, 16);
      if (s_end == s_start) //stop parsing if conversion error
        break;

      data_buffer[bytes_read++] = value;
      s_start = s_end;
    }

    if (bytes_read < 4)
      return;

    //write to chip
    int saddr = data_buffer[2];
    for (int i = 3; i < bytes_read; i++) {
      sound_chip.Write(data_buffer[2]+i-3, data_buffer[i]);
    }

    Serial.println(bytes_read);

    for (int i = 0; i < bytes_read; i++) {
      Serial.print(data_buffer[i]);
      Serial.print(' ');
    }
    Serial.println();
      
    bytes_read = 0;
  }
}

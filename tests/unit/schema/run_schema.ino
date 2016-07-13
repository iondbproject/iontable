#include <SPI.h>
#include <SD.h>
#include "test_schema.h"

void
setup(
) {
	SPI.begin();
	SD.begin(SD_CS_PIN);
	Serial.begin(BAUD_RATE);
	run_all_tests_schema();
}

void
loop(
) {}

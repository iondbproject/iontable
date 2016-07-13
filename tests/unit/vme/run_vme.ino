#include <SPI.h>
#include <SD.h>
#include "test_vme.h"

void
setup(
) {
	SPI.begin();
	SD.begin(SD_CS_PIN);
	Serial.begin(BAUD_RATE);
	run_all_tests_vme();
}

void
loop(
) {}

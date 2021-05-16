#include <Arduino.h>

// include the SD library:
#include <SD.h>
#include <SPI.h>

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;

// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
// Teensy audio board: pin 10
// Teensy 3.5 & 3.6 on-board: BUILTIN_SDCARD
// Wiz820+SD board: pin 4
// Teensy 2.0: pin 0
// Teensy++ 2.0: pin 20
const int chipSelect = BUILTIN_SDCARD;

void readFile(char *filePath);

void readFileLineByLine(char *filePath);

void writeFile(char *filePath, String data);

void setup() {
    //UNCOMMENT THESE TWO LINES FOR TEENSY AUDIO BOARD:
    //  SPI.setMOSI(7);  // Audio shield has MOSI on pin 7
    // SPI.setSCK(14);  // Audio shield has SCK on pin 14

    // Open serial communications and wait for port to open:
    Serial.begin(9600);

    SD.begin(chipSelect);

    while (!Serial) { ; // wait for serial port to connect. Needed for Leonardo only
    }


    Serial.print("\nSD Card okunuyor...");

    // we'll use the initialization code from the utility libraries
    // since we're just testing if the card is working!
    if (!card.init(SPI_HALF_SPEED, chipSelect)) {
        Serial.println("Okuma basarisiz! Sunları kontrol edin:");
        Serial.println("* card takili mi?");
        Serial.println("* card dogru baglandi mi?");
        Serial.println("* chipSelect secimi dogru mu?");
        return;
    } else {
        Serial.println("SD Card okumasi basarili.");
    }

    // print the type of card
    Serial.print("\nCard type: ");

    switch (card.type()) {
        case SD_CARD_TYPE_SD1:
            Serial.println("SD1");
            break;
        case SD_CARD_TYPE_SD2:
            Serial.println("SD2");
            break;
        case SD_CARD_TYPE_SDHC:
            Serial.println("SDHC");
            break;
        default:
            Serial.println("Unknown");
    }

    // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
    if (!volume.init(card)) {
        Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
        return;
    }


    // print the type and size of the first FAT-type volume
    uint32_t volumesize;
    Serial.print("\nVolume type is FAT");
    Serial.println(volume.fatType(), DEC);
    Serial.println();

    volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
    volumesize *= volume.clusterCount();       // we'll have a lot of clusters
    if (volumesize < 8388608ul) {
        Serial.print("Volume size (bytes): ");
        Serial.println(volumesize * 512);        // SD card blocks are always 512 bytes
    }
    Serial.print("Volume size (Kbytes): ");
    volumesize /= 2;
    Serial.println(volumesize);
    Serial.print("Volume size (Mbytes): ");
    volumesize /= 1024;
    Serial.println(volumesize);
    Serial.print("Volume size (Gbytes): ");
    volumesize /= 1024;
    Serial.println(volumesize);


    Serial.println("\nFiles found on the card (name, date and size in bytes): ");
    root.openRoot(volume);

    // list all files in the card with date and size
    root.ls(LS_R | LS_DATE | LS_SIZE);
    root.close();

//     Full read
    readFile("DATA.TXT");

//     Read Line by Line
    readFileLineByLine("DATA.TXT");

    // Write Line
    writeFile("DATA.TXT", "400\t401\t402\t403\t404\t405\n");

    // Read Line by Line
    readFileLineByLine("DATA.TXT");
}


void loop(void) {

}

void readFile(char *filePath) {
    Serial.print("--> ");
    Serial.print(filePath);
    Serial.println(" okunuyor...");

    // Dosya okuma
    File sdCardFile = SD.open(filePath, FILE_READ);

    while (sdCardFile.available()) {
        String data = sdCardFile.readString();
        Serial.println(data);
    }

    sdCardFile.close();
}

void readFileLineByLine(char *filePath) {
    Serial.print("--> ");
    Serial.print(filePath);
    Serial.println(" okunuyor...");

    // Dosya okuma
    File sdCardFile = SD.open(filePath, FILE_READ);

    int line = 0;
    while (sdCardFile.available()) {
        String data = sdCardFile.readStringUntil('\n');

        Serial.print("Line ");
        Serial.print(line);
        Serial.print(": ");
        Serial.println(data);
        line++;

    }

    sdCardFile.close();
    delay(20);
}

void writeFile(char *filePath, String data) {
    Serial.print("--> ");
    Serial.print(filePath);
    Serial.println(" yaziliyor...");

    // Dosya okuma
    File sdCardFile = SD.open(filePath, FILE_WRITE);

    Serial.println(data);
    sdCardFile.print(data);

    sdCardFile.close();

    Serial.print("--> ");
    Serial.print(filePath);
    Serial.println(" yazildi.");
}


#include <Arduino.h>
#include <File.h>
#include <Flash.h>

File myFile; /**< File object */ 

/**
 * @brief Write to the file and read from the file.
 * 
 * @details The file is located on the SD card.
 */
void setup() {

  /* Open serial communications and wait for port to open */
  Serial.begin(115200);
  while (!Serial) {
    ; /* wait for serial port to connect. Needed for native USB port only */
  }

  /* Create a new directory */
  Flash.mkdir("dir/");

  /* Open the file. Note that only one file can be open at a time,
     so you have to close this one before opening another. */
  myFile = Flash.open("dir/test.txt", FILE_WRITE);

  /* If the file opened okay, write to it */
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    /* Close the file */
    myFile.close();
    Serial.println("done.");
  } else {
    /* If the file didn't open, print an error */
    Serial.println("error opening test.txt");
  }

  /* Re-open the file for reading */
  myFile = Flash.open("dir/test.txt");

  if (myFile) {
    Serial.println("test.txt:");

    /* Read from the file until there's nothing else in it */
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    /* Close the file */
    myFile.close();
  } else {
    /* If the file didn't open, print an error */
    Serial.println("error opening test.txt");
  }
}

/**
 * @brief Run repeatedly.
 * 
 * @details Does not do anything.
 */
void loop() {

}

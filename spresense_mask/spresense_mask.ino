
#include <Camera.h>
#include <Flash.h>
#include <DNNRT.h>
#include <File.h>
#include <SDHCI.h>
#include <stdio.h>  /* for sprintf */

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET 4
Adafruit_SSD1306 display (SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BAUDRATE 115200
#define OFFSET_X  48
#define OFFSET_Y  8
#define BOX_WIDTH  224
#define BOX_HEIGHT 224
#define DNN_IMAGE_WIDTH 28
#define DNN_IMAGE_HEIGHT 28

DNNRT dnnrt;
SDClass SD;
DNNVariable input (DNN_IMAGE_WIDTH*DNN_IMAGE_HEIGHT);
String gStrResult = "";

void CamCB(CamImage img) {
  if (!img.isAvailable())return;
  img.convertPixFormat (CAM_IMAGE_PIX_FMT_RGB565);
  Serial.println("start clip and resize");
  gStrResult = "";
  int x;
  int index = 0;
  static uint8_t label[9]={0,1,2,3,4,5,6,7,8};

  CamImage small;
  CamErr camErr = img.clipAndResizeImageByHW(small,OFFSET_X, OFFSET_Y,OFFSET_X+BOX_WIDTH-1,OFFSET_Y+BOX_HEIGHT-1,DNN_IMAGE_WIDTH, DNN_IMAGE_HEIGHT);
  if (!small.isAvailable()){
    Serial.println("Error Making a Target Image");
    if (camErr) Serial.println("CamErr" + String(camErr));
    return;
  }
  uint16_t* buf = (uint16_t*)small.getImgBuff();
  float*input_buffer = input.data();
  for (int i = 0; i<DNN_IMAGE_WIDTH * DNN_IMAGE_HEIGHT; ++i, ++buf) {
    input_buffer [i] = (float) (((*buf & 0x07E0)>>9)<<2);
  }
  Serial.println("DNN Forward");
  dnnrt.inputVariable (input,0);
  dnnrt.forward();
  DNNVariable output = dnnrt.outputVariable(0);
  float max_value = 0.0;
  for (int i=0; output.size()>i; ++i) {
    if (output[i]>max_value){
      max_value=output[i];
      index=i;
    }
  }
  Serial.print ("Result: ");
  Serial.println(label [index] +" (" + String (output[index])+")");
  if (label[index]!=10){
    gStrResult += String(label[index]);
  }else {
    gStrResult += String (" ");
  }

  /*display.clearDisplay();
  display.setTextSize(3);
  display.setCursor(0,0);
  display.cp437(true);*/

  switch (index) {
    case 0: display.println("Bird");break;
    case 1: display.println("Human");break;
    case 2: display.println("Horse");break;
    case 3: display.println("-----");break;
  }
  /*display.display();*/
  Serial.println("Result:"+ gStrResult);
}



void setup() {
  Serial.begin(BAUDRATE);
  /*if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 Allocation Failed"));
    for(;;);
  }
  display.display();
  delay (2000);
  display.clearDisplay();
  display.drawPixel (10,10,SSD1306_WHITE);
  display.display();
  delay(2000);

  display.setTextSize (1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.cp437(true);

  for (int16_t i=0; i<256; i++) {
    if (i=='\n') display.write (' ');
    else display.write(i);
  }
  display.display();*/
  Serial.println("Loading Network Model");
  File nnbfile = SD.open ("model.nnb", FILE_READ);
  if (!nnbfile) {
    Serial.println("nnb not found");
    while(1);
  }
  Serial.println("Initialize DNNRT");
  int ret= dnnrt.begin(nnbfile);
  if (ret<0) {
    Serial.println("DNNRT initialize error");
    while(1);
  }
  theCamera.begin();
  theCamera.startStreaming(true,CamCB);
    
  }
    
void loop() {
  // put your setup code here, to run once:

}
  // put your main code here, to run repeatedly:

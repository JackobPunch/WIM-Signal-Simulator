#include <Adafruit_MCP4728.h>
#include <Wire.h>

Adafruit_MCP4728 mcp;

const int speedReference = 50;
const int speedChoosen = 50;
const int delayChannel = 2; //delay between channels given as a number of Samples
int delaySample1 = 421; //delay between Samples in microseconds
int delaySample2 = 400; //delay between Samples in microseconds
int buffer[delayChannel]; // Buffer for the second DAC
int DelayPeak[] = {206000,325000,47000,47000,0}; //delay between fluctations
int Samples1[] = {1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,0};
int Samples2[] = {2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,0};
int Samples3[] = {1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,0};
int Samples4[] = {1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,0};
int Samples5[] = {1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,0};


int relay1 = 5; //petla 1
int relay2 = 4; // petla 2
int delayRelay1 = 300000;
int delayRelay2 = 100000;


void adjustSpeed(int* DelayPeak, int arraySize) {
  int adjustment = delayChannel * 1200; // 1.2 milliseconds in microseconds
  for (int i = 0; i < arraySize-1; i++) {
    DelayPeak[i] -= adjustment;
  }
  int scale = speedChoosen/speedReference;
  for (int i = 0; i < arraySize; i++) {
    DelayPeak[i] *= scale;
  }
  delaySample1 *= scale;
  delaySample2 *= scale;
  delayRelay1 *= scale;
  delayRelay2 *= scale;
}

void GeneratePeak(int* Samples, int arraySize, int delayPeakValue) {
  if (delayChannel == 0) {
    for (int i = 0; i < arraySize; i++) {
      mcp.setChannelValue(MCP4728_CHANNEL_A, Samples[i]);
      mcp.setChannelValue(MCP4728_CHANNEL_B, Samples[i]);
      delayMicroseconds(delaySample1);
    }
  } else {
    int bufferIndex = 0;
    int buffer[delayChannel]; // Buffer for the second DAC
    for (int i = 0; i < arraySize + delayChannel; i++) {
      if (i < arraySize) {
        mcp.setChannelValue(MCP4728_CHANNEL_A, Samples[i]);
      } else {
        mcp.setChannelValue(MCP4728_CHANNEL_A, Samples[arraySize - 1]);
      }
      if (i >= delayChannel) {
        mcp.setChannelValue(MCP4728_CHANNEL_B, buffer[bufferIndex]);
      } else {
        mcp.setChannelValue(MCP4728_CHANNEL_B, 0); // Default value if within delay
      }
      if (i < arraySize) {
        buffer[bufferIndex] = Samples[i];
      }
      bufferIndex = (bufferIndex + 1) % delayChannel;
      delayMicroseconds(delaySample2);
    }
  }
    delayMicroseconds(delayPeakValue);
}

void setup() {
  Serial.begin(9600);

  if (!mcp.begin()) {
    Serial.println("Failed to find MCP4728 chip");
    while (1) {
      delay(10);
    }
  }

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  mcp.setChannelValue(MCP4728_CHANNEL_A, 0);
  mcp.setChannelValue(MCP4728_CHANNEL_B, 0);

  adjustSpeed(DelayPeak, sizeof(DelayPeak)/sizeof(DelayPeak[0]));
}

void loop() {
  //digitalWrite(relay1,HIGH);
  delayMicroseconds(delayRelay1);
  //digitalWrite(relay2,HIGH);
  delayMicroseconds(delayRelay2);
  GeneratePeak(Samples1, sizeof(Samples1)/sizeof(Samples1[0]), DelayPeak[0]);
  GeneratePeak(Samples2, sizeof(Samples2)/sizeof(Samples2[0]), DelayPeak[1]);
  GeneratePeak(Samples3, sizeof(Samples3)/sizeof(Samples3[0]), DelayPeak[2]);
  GeneratePeak(Samples4, sizeof(Samples4)/sizeof(Samples4[0]), DelayPeak[3]);
  GeneratePeak(Samples5, sizeof(Samples5)/sizeof(Samples5[0]), DelayPeak[4]);
  delayMicroseconds(delayRelay2);
  //digitalWrite(relay1,LOW);
  delayMicroseconds(delayRelay1);
  //digitalWrite(relay2,LOW);
  delay(2000);
}
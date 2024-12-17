#include <Adafruit_MCP4728.h>
#include <Wire.h>

Adafruit_MCP4728 mcp;

const int speedReference = 50;
const int speedChoosen = 50;
const int delayChannel = 2; //delay between channels given as a number of Samples
int delaySample1 = 434; //delay between Samples in microseconds
int delaySample2 = 412; //delay between Samples in microseconds
int buffer[delayChannel]; // Buffer for the second DAC
int DelayPeak[] = {206000,325000,47000,47000,0}; //delay between fluctations
int Samples1[] = {415,472,547,660,749,884,1008,1160,1262,1358,1435,1461,1493,1518,1533,1549,1557,1558,1565,1555,1526,1466,1343,1217,1098,962,833,695,589,512,474,417,380};
int Samples2[] = {412,486,624,764,944,1164,1409,1573,1719,1805,1879,1906,1927,1926,1952,1956,2004,2043,2064,2088,2056,1946,1800,1603,1394,1135,925,758,642,514,445,407,380};
int Samples3[] = {415,469,554,620,782,881,1015,1152,1248,1332,1380,1426,1438,1452,1455,1462,1475,1488,1502,1507,1478,1424,1339,1230,1088,938,793,680,594,504,442,403,380};
int Samples4[] = {420,467,516,578,641,758,903,997,1128,1241,1373,1408,1485,1474,1480,1491,1518,1551,1567,1574,1582,1518,1449,1370,1261,1177,1004,868,755,645,553,482,380};
int Samples5[] = {437,478,540,618,714,837,965,1125,1243,1344,1414,1461,1496,1519,1533,1547,1551,1573,1587,1591,1585,1553,1488,1394,1270,1136,980,838,726,588,507,449,380};

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
        mcp.setChannelValue(MCP4728_CHANNEL_B, 380); // Default value if within delay
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
  mcp.setChannelValue(MCP4728_CHANNEL_A, 380);
  mcp.setChannelValue(MCP4728_CHANNEL_B, 380);

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
#include <Adafruit_MCP4728.h>
#include <Wire.h>

int relay3 = 5; //petla 1
int relay4 = 4; // petla 2

Adafruit_MCP4728 mcp;

int microSnail=600; //delay between values in microseconds
int shifftt=5; //delay between channels given as a number of values
int snaill[]={201,318,41,44,300}; //delay between fluctations
int values1[]={0,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,0,0,0,0,0,0,0,0,0,0};
int values2[]={0,0,0,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,2200,0,0,0,0,0,0,0,0,0,0,0};
int values3[]={0,0,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,0,0,0,0,0,0};
int values4[]={0,0,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,0,0,0,0,0,0};
int values5[]={0,0,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,1100,0,0,0,0,0,0};


void processValues(int* values, int size, int shifftt, int snailll) {
  for(int i = 0; i < size + shifftt; i++) {
    if(i >= size){
      mcp.setChannelValue(MCP4728_CHANNEL_A, values[size - 1]);
    } else {
      mcp.setChannelValue(MCP4728_CHANNEL_A, values[i]); 
    }
    if(i >= shifftt) {
      mcp.setChannelValue(MCP4728_CHANNEL_B, values[i - shifftt]);
    }
    delayMicroseconds(microSnail);
  }
  delay(snailll);
}

void setup() {
  Serial.begin(9600);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  if (!mcp.begin()) {
    Serial.println("Failed to find MCP4728 chip");
    while (1) {
      delay(10);
    }
  }
}

void loop() {
  digitalWrite(relay3,HIGH);
  delay(300);
  digitalWrite(relay4,HIGH);
  delay(100);
  processValues(values1, sizeof(values1)/sizeof(values1[0]), shifftt, snaill[0]);
  processValues(values2, sizeof(values2)/sizeof(values2[0]), shifftt, snaill[1]);
  processValues(values3, sizeof(values3)/sizeof(values3[0]), shifftt, snaill[2]);
  processValues(values4, sizeof(values4)/sizeof(values4[0]), shifftt, snaill[3]);
  processValues(values5, sizeof(values5)/sizeof(values5[0]), shifftt, snaill[4]);
  delay(100);
  digitalWrite(relay3,LOW);
  delay(300);
  digitalWrite(relay4,LOW);
  delay(2000);
}

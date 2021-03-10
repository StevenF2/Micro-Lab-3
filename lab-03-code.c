#define SEGMENT_A     2
#define SEGMENT_B     3
#define SEGMENT_C     4
#define SEGMENT_D     5
#define SEGMENT_E     6
#define SEGMENT_F     7
#define SEGMENT_G     8
#define SEGMENT_DP    9

#define DIGIT_0       13
#define DIGIT_1       12
#define DIGIT_2       11
#define DIGIT_3       10

#define ANALOG_INPUT  A0
#define PB            A2

uint8_t segment_pins[8] = {SEGMENT_A, SEGMENT_B, SEGMENT_C, SEGMENT_D, SEGMENT_E, SEGMENT_F, SEGMENT_G, SEGMENT_DP};
uint8_t digit_cathodes[4] = {DIGIT_0,DIGIT_1,DIGIT_2,DIGIT_3};
uint8_t counter = 0;

int display7c[10]= {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
int display7cdp[6]= {0xBF,0x86,0xDB,0xCF,0xE6,0xED};
int state = 0;
int verificar = 0;

bool ver = false;
bool iniciar = false;

uint8_t dig1 = 0;
uint8_t dig2 = 0;
uint8_t dig3 = 0;
uint8_t dig4 = 0;

void ssd_output_code(uint8_t code, uint8_t digit){
  
  bool segments[8];
  segments[0] = (code & (1 << 0)) != 0;
  segments[1] = (code & (1 << 1)) != 0;
  segments[2] = (code & (1 << 2)) != 0;
  segments[3] = (code & (1 << 3)) != 0;
  segments[4] = (code & (1 << 4)) != 0;
  segments[5] = (code & (1 << 5)) != 0;
  segments[6] = (code & (1 << 6)) != 0;
  segments[7] = (code & (1 << 7)) != 0;
  
  bool digits[4];
  digits[0] = HIGH;
  digits[1] = HIGH;
  digits[2] = HIGH;
  digits[3] = HIGH;
  digits[digit] = LOW;
  
  digitalWrite(SEGMENT_A, segments[0]);
  digitalWrite(SEGMENT_B, segments[1]);
  digitalWrite(SEGMENT_C, segments[2]);
  digitalWrite(SEGMENT_D, segments[3]);
  digitalWrite(SEGMENT_E, segments[4]);
  digitalWrite(SEGMENT_F, segments[5]);
  digitalWrite(SEGMENT_G, segments[6]);
  digitalWrite(SEGMENT_DP, segments[7]);
  
  digitalWrite(DIGIT_0, digits[0]);
  digitalWrite(DIGIT_1, digits[1]);
  digitalWrite(DIGIT_2, digits[2]);
  digitalWrite(DIGIT_3, digits[3]);
  
}

int digito(uint8_t sensor){
  
  for(int i=0;i<10;i++){
  if(sensor==i){
    return display7c[i];
  }
  }
}

int digitodp(uint8_t sensor){
  
  for(int i=0;i<6;i++){
  if(sensor==i){
    return display7cdp[i];
  }
  }
}


void setup(){
  pinMode(SEGMENT_A, OUTPUT);
  pinMode(SEGMENT_B, OUTPUT);
  pinMode(SEGMENT_C, OUTPUT);
  pinMode(SEGMENT_D, OUTPUT);
  pinMode(SEGMENT_E, OUTPUT);
  pinMode(SEGMENT_F, OUTPUT);
  pinMode(SEGMENT_G, OUTPUT);
  pinMode(SEGMENT_DP, OUTPUT);
  
  pinMode(DIGIT_0, OUTPUT);
  pinMode(DIGIT_1, OUTPUT);
  pinMode(DIGIT_2, OUTPUT);
  pinMode(DIGIT_3, OUTPUT);
  
  Serial.begin(9600);
}

void loop(){
  
  int btn = digitalRead(PB);
  uint16_t sensor_read = analogRead(ANALOG_INPUT);
  uint16_t voltaje = 0;
  double voltaje1 = 0;
  
  
  if(btn != state){
    if(btn==1 && ver){
    if(verificar==2){
      verificar = 0;
    }
    else{
      verificar += 1;
    }
    ver = false;
  }
    
  }
  state = btn;
  
  
  
  if(verificar==0){
  	voltaje = ((sensor_read*5.000)/1024)*1000;
  	dig1 = digitodp(voltaje/1000);
  	dig2 = digito((voltaje % 1000) / 100);
  	dig3 = digito(((voltaje % 1000) % 100) / 10);
  	dig4 = digito(((voltaje % 1000) % 100) % 10);
    ver = true;
  }
  
  if(verificar==1){
  	dig1 = digitodp(5);
  	dig2 = digito(0);
  	dig3 = digito(0);
  	dig4 = digito(0);
    ver = true;
  }
  
  else if(verificar==2){
  	dig1 = digito(sensor_read/1000);
  	dig2 = digito((sensor_read % 1000) / 100);
  	dig3 = digito(((sensor_read % 1000) % 100) / 10);
  	dig4 = digito(((sensor_read % 1000) % 100) % 10);
    ver = true;
  }
  
  if(counter==25){
    if(sensor_read>1000){
    	Serial.print(sensor_read);
  	}
  	else{
    	Serial.print(" ");
    	Serial.print(sensor_read);
  	}
  
  	voltaje1 = (sensor_read*5.000)/1024;
  	Serial.print('\t');
  	Serial.print(voltaje1,3);
  	Serial.print('\t');
  	Serial.println(" 5.000 Kohm");
    counter = 0;
  }
  
  ssd_output_code(dig1,3);
  delay(10);
  ssd_output_code(dig2,2);
  delay(10);
  ssd_output_code(dig3,1);
  delay(10);
  ssd_output_code(dig4,0);
  delay(10);
  counter += 1;
  
}
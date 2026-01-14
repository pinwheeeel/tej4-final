#include <LiquidCrystal_I2C.h>

#define IORQ 11
#define MREQ 12
#define CLK 13
#define RD 14
#define WR 15
#define BUSACK 16
#define WAIT 17
#define BUSREQ 18
#define RESET 19
#define M1 20
#define RFSH 21

#define CE 23
#define OE 25
#define WE 27

const int D[8] = { 2, 3, 4, 5, 6, 7, 8, 9 };
const int A[11] = { 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49 };

LiquidCrystal_I2C lcd(0x27, 16, 2);

const uint8_t program[] = {
  // bytes array
  0x3E,254,31,3,89,5,67,9,0
};

void setup() {
  pinMode(CE, OUTPUT);
  digitalWrite(CE, HIGH);
  pinMode(OE, OUTPUT);
  digitalWrite(OE, HIGH);
  pinMode(WE, OUTPUT);
  digitalWrite(WE, HIGH);

  pinMode(IORQ, INPUT);
  pinMode(MREQ, INPUT);
  pinMode(CLK, OUTPUT);
  pinMode(RD, INPUT);
  pinMode(WR, INPUT);
  pinMode(BUSACK, INPUT);
  pinMode(WAIT, OUTPUT);
  pinMode(BUSREQ, OUTPUT);
  pinMode(RESET, OUTPUT);
  pinMode(M1, INPUT);
  pinMode(RFSH, INPUT);

  Serial.begin(9600);
  load_program();
  delay(100);
  digitalWrite(RESET, HIGH);
}

void loop() {
  digitalWrite(CLK, LOW);
  delay_custom();
  digitalWrite(CLK, HIGH);
  delay_custom();
  if(digitalRead(MREQ)==LOW){ //Mem request
    digitalWrite(CE, LOW);
    if (digitalRead(RD)==LOW){ // Read data from ram
      digitalWrite(OE, LOW);
    } else if (digitalRead(WR)==LOW){  // Write data to ram
      digitalWrite(WE,LOW);
    }
    delay_custom();
    while (digitalRead(MREQ)==LOW){
      digitalWrite(CLK, LOW);
      delay_custom();
      digitalWrite(CLK, HIGH);
      delay_custom();
    }
    digitalWrite(OE, HIGH);
    digitalWrite(WE, HIGH);
    digitalWrite(CE, HIGH);
    delay_custom();
  } else if(digitalRead(IORQ)==LOW){ //Io request
    if (digitalRead(RD)==LOW){ // Read data from ram
      digitalWrite(OE, LOW);
    } else if (digitalRead(WR)==LOW){  // Write data to ram
      digitalWrite(WE,LOW);
    }
    delay_custom();
    while (digitalRead(MREQ)==LOW){
      digitalWrite(CLK, LOW);
      delay_custom();
      digitalWrite(CLK, HIGH);
      delay_custom();
    }
    digitalWrite(OE, HIGH);
    digitalWrite(WE, HIGH);
    delay_custom();
  } 


}

void delay_custom() {
  delay(1);
}

// DEBUG: reading ram

void read_ram() {
  set_address_bus_output();
  set_data_bus_input();

  for(int i=0; i<sizeof(program); i++) {
    write_address_bus(i);
    delay_custom();
    // chip enable low
    digitalWrite(CE, LOW);
    // orite enable low
    digitalWrite(OE, LOW);
    delay_custom();

    Serial.println(read_data_bus());
    delay_custom();

    // chip enable high
    digitalWrite(OE, HIGH);
    // orite enable high
    digitalWrite(CE, HIGH);
    delay_custom();
  }
}

// LOAD PROGRAM

void load_program() {
  digitalWrite(CLK, HIGH);
  digitalWrite(RESET, LOW);
  // "RESET must be active for a minimum of three full clock cycles
  // before a reset operation is complete."
  for (int i = 0; i < 4; i++) {
    digitalWrite(CLK, LOW);
    delay_custom();
    digitalWrite(CLK, HIGH);
    delay_custom();
  }

  set_address_bus_output();
  set_data_bus_output();
  for(int i=0; i<sizeof(program); i++) {
    // write addresses
    write_address_bus(i);
    // write data
    write_data_bus(program[i]);
    delay_custom();

    // chip enable low
    digitalWrite(CE, LOW);
    // write enable low
    digitalWrite(WE, LOW);
    delay_custom(); // >190 ns

    // chip enable high
    digitalWrite(WE, HIGH);
    // write enable high
    digitalWrite(CE, HIGH);
    delay_custom();
  }

  set_data_bus_input();
  set_address_bus_input();


}


// DATA BUS FUNCTIONS

void set_data_bus_input() {
  for (int i = 0; i < 8; i++)
    pinMode(D[i], INPUT);
}

void set_data_bus_output() {
  for (int i = 0; i < 8; i++)
    pinMode(D[i], OUTPUT);
}

int read_data_bus() {
  int value = 0;
  //Serial.println("Read Data Bus");
  for (int i = 0; i < 8; i++){
    value |= digitalRead(D[i]) << i;
    //Serial.print(digitalRead(D[i]));
  }
  // Serial.println();
  return value;
}

void write_data_bus(int value) {
  // Serial.print("Write Data Bus: ");
  // Serial.println(value);
  for (int i = 0; i < 8; i++) {
    digitalWrite(D[i], (value >> i) & 1);
    // Serial.print((value>>i) & 1);
  }
  // Serial.println();
}

// ADDRESS BUS FUNCTIONS

void set_address_bus_input() {
  for (int i = 0; i < 8; i++)
    pinMode(A[i], INPUT);
}

void set_address_bus_output() {
  for (int i = 0; i < 8; i++)
    pinMode(A[i], OUTPUT);
}

int read_address_bus() {
  int value = 0;
  // Serial.println("Read Address Bus");
  for (int i = 0; i < 8; i++){
    value |= digitalRead(A[i]) << i;
    // Serial.print(digitalRead(A[i]));
  }
  // Serial.println();

  return value;
}

void write_address_bus(int value) {
  // Serial.print("Write Address Bus: ");
  // Serial.println(value);
  for (int i = 0; i < 11; i++){
    digitalWrite(A[i], (value >> i) & 1);
    // Serial.print((value>>i) & 1);
  }
  // Serial.println();
    
}

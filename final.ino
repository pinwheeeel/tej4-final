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
};

void setup() {
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

  pinMode(CE, OUTPUT);
  pinMode(OE, OUTPUT);
  pinMode(WE, OUTPUT);
}

void loop() {
}

void delay_custom() {
  delay(1);
}

// LOAD PROGRAM

void load_program() {
  digitalWrite(RESET, LOW);
  // "RESET must be active for a minimum of three full clock cycles
  // before a reset operation is complete."
  for (int i = 0; i < 3; i++) {
    digitalWrite(CLK, LOW);
    delay_custom();
    digitalWrite(CLK, HIGH);
    delay_custom();
  }

  set_data_bus_output();
  for(int i=0; i<sizeof(program); i++) {
    // write addresses
    // write data
    
    delay_custom();

    // chip enable low
    // write enable low
    digitalWrite(CE, LOW);
    digitalWrite(WE, LOW);
    delay_custom(); // 190 ns

    // 

    // chip enable high
    // write enable high
  }

  set_data_bus_input();

  // busrequest and busack?
  // digitalWrite(BUSREQ, LOW);f
  // while(digitalRead(BUSACK == HIGH)); // wait for busack
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
  for (int i = 0; i < 8; i++)
    value |= digitalRead(D[i]) << i;
  return value;
}

void write_data_bus(int value) {
  for (int i = 0; i < 8; i++)
    digitalWrite(D[i], (value >> i) & 1);
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
  for (int i = 0; i < 8; i++)
    value |= digitalRead(A[i]) << i;
  return value;
}

void write_address_bus(int value) {
  for (int i = 0; i < 8; i++)
    digitalWrite(A[i], (value >> i) & 1);
}

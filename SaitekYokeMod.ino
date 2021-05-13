#include <RotaryEncoder.h>

#include <Keypad.h>
#include <Joystick.h>
#include <RotaryEncoder.h>

const byte ROWS = 3;
const byte COLS = 5;
char keys[ROWS][COLS] = {
  {0, 1, 2, 3, 4},
  {5, 6, 7, 8, 64}, // last one not connected
  {9, 10, 11, 12, 13},
};
const int SWITCH_DELAY = 500;
const int SWITCH_DEBOUNCE = 125;
const int ROTARY_DELAY = 100;

// 26 joystick buttons from matrix
const int JOY_IGN_DOWN = 26;
const int JOY_IGN_UP = 27;
const int JOY_IGN_BTN = 28;
const int PIN_IGN_PUSH = 8;

const byte ACTIVE_BUTTONS = 31;

byte rowPins[ROWS] = {14, 15, 2}; // 3, 4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {16, 18, 19, 20, 21}; //connect to the column pinouts of the keypad

RotaryEncoder encoder(9, 10, RotaryEncoder::LatchMode::FOUR3);
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);
Joystick_ Joystick(
   JOYSTICK_DEFAULT_REPORT_ID, 
   JOYSTICK_TYPE_JOYSTICK, 
   ACTIVE_BUTTONS, 
   0, // # of HATs
   false, // include X-axis
   false, // include Y-axis
   false, // include Z-axis
   false, // include Rx-axis
   false, // include Ry-axis
   false, // include Rz-axis
   false, // include Rudder
   false, // include Throttle
   false, // include Accelerator
   false, // include Brake
   false // include Steering
);

// First switch - btn 0 - to off , btn1 to on
// Second switch - btn 2 - to off, btn3 to on
// and so on
int matrix_to_joy(char c, bool on) {
  if (c>=64) return -1;
  return (c*2) + int(on);  
}

void setup(){
  Serial.begin(115200);
  keypad.setDebounceTime(SWITCH_DEBOUNCE);
  keypad.addEventListener(keypadEvent);
  encoder.setPosition(0);
  Joystick.begin();
}

void keypadEvent(KeypadEvent key){
  int active_idx = keypad.findInList(key);
  int state = keypad.key[active_idx].kstate;
  if (state == PRESSED) {
    Serial.println("ON");
    Serial.println(keypad.key[active_idx].kchar, HEX);
    int joy_id = matrix_to_joy(key, true);
    Joystick.pressButton(joy_id);
    delay(SWITCH_DELAY);
    Joystick.releaseButton(joy_id);
  }
  if (state == RELEASED) {
    Serial.println("OFF");
    Serial.println(keypad.key[active_idx].kchar, HEX);
    int joy_id = matrix_to_joy(key, false);
    Joystick.pressButton(joy_id);
    delay(SWITCH_DELAY);
    Joystick.releaseButton(joy_id);
  }
}

int change_ign(int diff) {
  static int ign = 0;
  if (diff < 0 && ign >0) {
    ign += diff;
    Serial.println("-");
    Joystick.pressButton(JOY_IGN_DOWN);
    delay(100);
    Joystick.releaseButton(JOY_IGN_DOWN);
  }
  if (diff > 0 && ign <=4) {
    ign += diff;
    Serial.println("+");
    Joystick.pressButton(JOY_IGN_UP);
    delay(ROTARY_DELAY);
    Joystick.releaseButton(JOY_IGN_UP);
  }
}
void check_ignition() {
  static bool btn = true;
  static int pos = 0;
  encoder.tick();
  int new_pos = encoder.getPosition();
  if (new_pos != pos){
    change_ign(new_pos-pos);
  }
  pos = new_pos;

  bool btn_new = digitalRead(PIN_IGN_PUSH);
  if (btn_new != btn) {
    if (btn_new){
      Serial.println("btn rlse");
      Joystick.releaseButton(JOY_IGN_BTN);
    }else{
      Serial.println("btn push");
      Joystick.pressButton(JOY_IGN_BTN);
    }
  }
  btn = btn_new;
}

void loop(){
  keypad.getKeys();
  check_ignition();  
  delay(10);
}

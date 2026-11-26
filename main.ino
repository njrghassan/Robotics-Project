#include <Servo.h>
// #include <Keypad.h>
#include <LiquidCrystal.h>

// Servo motor
const int LOCK_MOTOR_PIN = 3;

// Ultrasonic
const int TRIG_PIN = 12;
const int ECHO_PIN = 13;

// PIR sensor
const int PIR_SENSOR_PIN = 7;

// Buzzzzzzzzzzzzzzzz
const int PIEZO_BUZZER_PIN = A0;

// keypad configuration
const int keypadRows = 4;
const int keypadCols = 4;

// char keys[keypadRows][keypadCols] = {
//     {'1', '2', '3'},
//     {'4', '5', '6'},
//     {'7', '8', '9'},
//     {'*', '0', '#'}
// };

// byte rowPins[keypadRows] = {10, 9, 8, 7}; // Connect to the row pinouts of the keypad
// byte colPins[keypadCols] = {11, 12, 13}; // Connect to the column pinouts of the keypad

// Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, keypadRows, keypadCols); //AI

// Variable to keep track of the last time the distance was updated
unsigned long lastUpdateTime = 0;

// THe servo oobject for the locking mechnism
Servo lockMotor;

// Initialize the LCD with the correct pin numbers    
LiquidCrystal lcd(22, 23, 24, 25, 26, 27); 

// functions 
// initUltrasonicSensor(int trigPin, int echoPin);
// timeToUpdate(unsigned long &lastUpdateTime, unsigned long intervalMs);
// measureDistance(int trigPin, int echoPin);


void setup() {
    Serial.begin(9600);

    // LCD to say hello
    lcd.begin(16, 2); 
    lcd.setCursor(0, 0);
    lcd.print("Hello, World!");
    lcd.setCursor(0, 1);
    lcd.print("Arduino Uno");

    // Attach the servo to the specified pin
    lockMotor.attach(LOCK_MOTOR_PIN);

    // Set the piezo buzzer pin as an output
    pinMode(PIEZO_BUZZER_PIN, OUTPUT);

    // Initialize the ultrasonic sensor
    initUltrasonicSensor(TRIG_PIN, ECHO_PIN);

    // Initialize the PIR sensor
    pinMode(PIR_SENSOR_PIN, INPUT);
}

void loop() {
    // Checking the keypad for any key presses
    // char key = keypad.getKey();

    // if (timeToUpdate(lastUpdateTime, 20UL) && key){
    //     Serial.print("Key Pressed: ");
    //     Serial.println(key);
    // }
   
    if (timeToUpdate(lastUpdateTime, 1000)) {
        float distance = measureDistance(TRIG_PIN, ECHO_PIN);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Distance: ");
        if (distance < 0) {
            lcd.print("Out of range");
        } else {
            lcd.print(distance);
            lcd.print(" cm");
        }
    }

    if (digitalRead(PIR_SENSOR_PIN) == HIGH) {
        lcd.setCursor(0, 1);
        lcd.print("Motion Detected!");

        digitalWrite(PIEZO_BUZZER_PIN, HIGH); // Turn on the piezo buzzer
        lockMotor.write(90); // Lock the mechanism

        Serial.println("Motion detected! Locking the mechanism.");
    } 
    else {
        lcd.setCursor(0, 1);
        lcd.print("No Motion       ");

        lockMotor.write(0); // Unlock the mechanism
        Serial.println("No motion detected. Unlocking the mechanism.");
    }

    digitalWrite(PIEZO_BUZZER_PIN, LOW); // Turn off the piezo buzzer
}

bool timeToUpdate(unsigned long &lastUpdateTime, unsigned long intervalMs) {
    unsigned long currentTime = millis();
    if (currentTime - lastUpdateTime >= intervalMs) {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

void initUltrasonicSensor(int trigPin, int echoPin) {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    digitalWrite(trigPin, LOW);
}

float measureDistance(int trigPin, int echoPin) {
    const unsigned long ECHO_TIMEOUT_US = 30000;

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    unsigned long pulseUs = pulseIn(echoPin, HIGH, ECHO_TIMEOUT_US);
    if (pulseUs == 0) {
        return -1.0f;
    }

    float distanceCm = pulseUs * 0.0343f / 2.0f;
    if (distanceCm < 2.0f || distanceCm > 400.0f) {
        return -1.0f;
    }
    return distanceCm;
}
#include <Arduino.h>

// ============================================================================
// PIN ASSIGNMENT MACROS
// Socket Pins 1-6 & 8-13 connect through 2.2k ohm resistors to Arduino pins
// Pin 7 = GND, Pin 14 = +5V VCC
// ============================================================================
#define SOCKET_PIN_1 5
#define SOCKET_PIN_2 6
#define SOCKET_PIN_3 7
#define SOCKET_PIN_4 8
#define SOCKET_PIN_5 9
#define SOCKET_PIN_6 10
// Pin 7 is connected to GND

#define SOCKET_PIN_8 A0
#define SOCKET_PIN_9 A1
#define SOCKET_PIN_10 A2
#define SOCKET_PIN_11 A3
#define SOCKET_PIN_12 A4
#define SOCKET_PIN_13 A5
// Pin 14 is connected to +5V (VCC)

// ============================================================================
// EXPECTED HARDCODED SIGNATURES FOR GENERIC QUAD CHIPS
// Signature = (bit_00 << 0) + (bit_01 << 1) + (bit_10 << 2) + (bit_11 << 3)
// ============================================================================
const uint8_t SIG_AND_74HC08  = (0 << 0) + (0 << 1) + (0 << 2) + (1 << 3); // 8
const uint8_t SIG_NAND_74HC00 = (1 << 0) + (1 << 1) + (1 << 2) + (0 << 3); // 7
const uint8_t SIG_OR_74HC32   = (0 << 0) + (1 << 1) + (1 << 2) + (1 << 3); // 14

// Stimulus sequence 00, 01, 10, 11
const uint8_t TEST_PAIRS[4][2] = {
    {0, 0},
    {0, 1},
    {1, 0},
    {1, 1}};

// ============================================================================
// PIN CONFIGURATION FUNCTIONS
// ============================================================================

// High-impedance safety state between tests
void setAllPinsSafe()
{
    const uint8_t pins[12] = {
        SOCKET_PIN_1, SOCKET_PIN_2, SOCKET_PIN_3, SOCKET_PIN_4, SOCKET_PIN_5, SOCKET_PIN_6, SOCKET_PIN_8, SOCKET_PIN_9, SOCKET_PIN_10, SOCKET_PIN_11, SOCKET_PIN_12, SOCKET_PIN_13};
    for (uint8_t i = 0; i < 12; i++)
    {
        pinMode(pins[i], INPUT);
    }
}

// 1. Configure pins for 74HC04 (Hex Inverter)
// Inverters: 1->2, 3->4, 5->6, 9->8, 11->10, 13->12
void configure74HC04()
{
    setAllPinsSafe();
    // Inputs to chip (Arduino outputs)
    pinMode(SOCKET_PIN_1, OUTPUT);
    pinMode(SOCKET_PIN_3, OUTPUT);
    pinMode(SOCKET_PIN_5, OUTPUT);
    pinMode(SOCKET_PIN_9, OUTPUT);
    pinMode(SOCKET_PIN_11, OUTPUT);
    pinMode(SOCKET_PIN_13, OUTPUT);

    // Outputs from chip (Arduino inputs)
    pinMode(SOCKET_PIN_2, INPUT);
    pinMode(SOCKET_PIN_4, INPUT);
    pinMode(SOCKET_PIN_6, INPUT);
    pinMode(SOCKET_PIN_8, INPUT);
    pinMode(SOCKET_PIN_10, INPUT);
    pinMode(SOCKET_PIN_12, INPUT);
}

// 2. Configure pins for 74HC02 (Quad 2-input NOR gate)
// Inverted pinout: Out=1,InA=2,InB=3 | Out=4,InA=5,InB=6 | InA=8,InB=9,Out=10 | InA=11,InB=12,Out=13
void configure74HC02()
{
    setAllPinsSafe();
    // Inputs to chip (Arduino outputs)
    pinMode(SOCKET_PIN_2, OUTPUT);
    pinMode(SOCKET_PIN_3, OUTPUT);
    pinMode(SOCKET_PIN_5, OUTPUT);
    pinMode(SOCKET_PIN_6, OUTPUT);
    pinMode(SOCKET_PIN_8, OUTPUT);
    pinMode(SOCKET_PIN_9, OUTPUT);
    pinMode(SOCKET_PIN_11, OUTPUT);
    pinMode(SOCKET_PIN_12, OUTPUT);

    // Outputs from chip (Arduino inputs)
    pinMode(SOCKET_PIN_1, INPUT);
    pinMode(SOCKET_PIN_4, INPUT);
    pinMode(SOCKET_PIN_10, INPUT);
    pinMode(SOCKET_PIN_13, INPUT);
}

// 3. Configure pins for generic quad chips (74HC00, 74HC08, 74HC32)
// InA=1,InB=2,Out=3 | InA=4,InB=5,Out=6 | Out=8,InA=9,InB=10 | Out=11,InA=12,InB=13
void configure74HC_generic()
{
    setAllPinsSafe();
    // Inputs to chip (Arduino outputs)
    pinMode(SOCKET_PIN_1, OUTPUT);
    pinMode(SOCKET_PIN_2, OUTPUT);
    pinMode(SOCKET_PIN_4, OUTPUT);
    pinMode(SOCKET_PIN_5, OUTPUT);
    pinMode(SOCKET_PIN_9, OUTPUT);
    pinMode(SOCKET_PIN_10, OUTPUT);
    pinMode(SOCKET_PIN_12, OUTPUT);
    pinMode(SOCKET_PIN_13, OUTPUT);

    // Outputs from chip (Arduino inputs)
    pinMode(SOCKET_PIN_3, INPUT);
    pinMode(SOCKET_PIN_6, INPUT);
    pinMode(SOCKET_PIN_8, INPUT);
    pinMode(SOCKET_PIN_11, INPUT);
}

// ============================================================================
// TEST STEPS
// ============================================================================

// Step 1: Check 74HC04 (Hex Inverter)
bool test74HC04()
{
    configure74HC04();
    Serial.println("Trying 74HC04");

    const uint8_t inPins[6]  = {SOCKET_PIN_1, SOCKET_PIN_3, SOCKET_PIN_5, SOCKET_PIN_9, SOCKET_PIN_11, SOCKET_PIN_13};
    const uint8_t outPins[6] = {SOCKET_PIN_2, SOCKET_PIN_4, SOCKET_PIN_6, SOCKET_PIN_8, SOCKET_PIN_10, SOCKET_PIN_12};

    // 1. Apply 1 to all inputs -> all outputs should be 0
    for (uint8_t i = 0; i < 6; i++)
    {
        digitalWrite(inPins[i], HIGH);
    }
    delayMicroseconds(10);
    for (uint8_t i = 0; i < 6; i++)
    {
        if (digitalRead(outPins[i]) != LOW)
        {
            return false; // Fail, try next gate
        }
    }

    // 2. Apply 0 to all inputs -> all outputs should be 1
    for (uint8_t i = 0; i < 6; i++)
    {
        digitalWrite(inPins[i], LOW);
    }
    delayMicroseconds(10);
    for (uint8_t i = 0; i < 6; i++)
    {
        if (digitalRead(outPins[i]) != HIGH)
        {
            return false; // Fail, try next gate
        }
    }

    return true; // Pass
}

// Step 2: Check 74HC02 (Quad NOR Gate)
bool test74HC02()
{
    configure74HC02();
    Serial.println("Trying 74HC02");

    const uint8_t inA[4]  = {SOCKET_PIN_2, SOCKET_PIN_5, SOCKET_PIN_8, SOCKET_PIN_11};
    const uint8_t inB[4]  = {SOCKET_PIN_3, SOCKET_PIN_6, SOCKET_PIN_9, SOCKET_PIN_12};
    const uint8_t outY[4] = {SOCKET_PIN_1, SOCKET_PIN_4, SOCKET_PIN_10, SOCKET_PIN_13};

    // Expected output for NOR gate across sequence:
    // 00 -> 1
    // 01 -> 0
    // 10 -> 0
    // 11 -> 0
    const uint8_t expectedNOR[4] = {HIGH, LOW, LOW, LOW};

    for (uint8_t step = 0; step < 4; step++)
    {
        for (uint8_t g = 0; g < 4; g++)
        {
            digitalWrite(inA[g], TEST_PAIRS[step][0]);
            digitalWrite(inB[g], TEST_PAIRS[step][1]);
        }
        delayMicroseconds(10);

        for (uint8_t g = 0; g < 4; g++)
        {
            if (digitalRead(outY[g]) != expectedNOR[step])
            {
                return false; // Mismatch, go to next gate
            }
        }
    }

    return true; // Pass
}

// Step 3: Check Generic Quad Gate (74HC00, 74HC08, 74HC32)
void testGeneric()
{
    configure74HC_generic();
    Serial.println("Trying 74HC generic");

    const uint8_t inA[4]  = {SOCKET_PIN_1, SOCKET_PIN_4, SOCKET_PIN_9, SOCKET_PIN_12};
    const uint8_t inB[4]  = {SOCKET_PIN_2, SOCKET_PIN_5, SOCKET_PIN_10, SOCKET_PIN_13};
    const uint8_t outY[4] = {SOCKET_PIN_3, SOCKET_PIN_6, SOCKET_PIN_8, SOCKET_PIN_11};

    uint8_t signature = 0;

    for (uint8_t step = 0; step < 4; step++)
    {
        // Apply stimulus to all 4 gates
        for (uint8_t g = 0; g < 4; g++)
        {
            digitalWrite(inA[g], TEST_PAIRS[step][0]);
            digitalWrite(inB[g], TEST_PAIRS[step][1]);
        }
        delayMicroseconds(10);

        // Read first gate output
        uint8_t firstBit = digitalRead(outY[0]);

        // Check all outputs look the same
        for (uint8_t g = 1; g < 4; g++)
        {
            if (digitalRead(outY[g]) != firstBit)
            {
                Serial.println(F("UNKNOWN"));
                return;
            }
        }

        // Save output as the corresponding bit of the signature variable
        signature += (firstBit << step);
    }

    // Compare calculated signature with hardcoded values
    if (signature == SIG_AND_74HC08)
    {
        Serial.println(F("74HC08 - AND"));
    }
    else if (signature == SIG_NAND_74HC00)
    {
        Serial.println(F("74HC00 - NAND"));
    }
    else if (signature == SIG_OR_74HC32)
    {
        Serial.println(F("74HC32 - OR"));
    }
    else
    {
        Serial.println(F("UNKNOWN"));
    }
}

void scanChip()
{
    // 1. Try HEX inverter (74HC04)
    if (test74HC04())
    {
        Serial.println(F("74HC04 - NOT"));
        setAllPinsSafe();
        return;
    }

    // 2. Try NOR gate (74HC02)
    if (test74HC02())
    {
        Serial.println(F("74HC02 - NOR"));
        setAllPinsSafe();
        return;
    }

    // 3. Try Generic Quad Gates (74HC00, 74HC08, 74HC32)
    testGeneric();
    setAllPinsSafe();
}

void setup()
{
    Serial.begin(115200);
    scanChip();
}

void loop()
{
    // Empty - scan runs once upon boot
}

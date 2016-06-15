#include <avr/pgmspace.h>

#include "std/initializer_list.h"

namespace HWP {
namespace SegmentDisplay {

const PROGMEM uint8_t DECODER[] = {
    //.gfedcba
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111, // 9
    0b00000000, // *
    0b00000000, // *
    0b00000000, // *
    0b00000000, // *
    0b00000000, // *
    0b00000000, // *
    0b00000000, // EMPTY
    0b01000000, // DASH
};

const size_t EMPTY = 16;
const size_t DASH = 17;

template<size_t DIGITS>
class Driver {
    public:
        Driver(std::initializer_list<uint8_t> digits, std::initializer_list<uint8_t> pins) {
            size_t i = 0;
            for(auto pin : digits) {
                if(i >= DIGITS) break;
                this->digits[i++] = pin;
                pinMode(pin, OUTPUT);
            }
            i = 0;
            for(auto pin : pins) {
                if(i >= 7) break;
                segments[i++] = pin;
                pinMode(pin, OUTPUT);
            }
        }

        void setState(std::initializer_list<uint8_t> state) {
            size_t i = 0;
            for(auto digitState : state) {
                setDigit(i++, digitState);
            }
        }

        void setState(size_t digit, uint8_t state) {
            if(digit >= DIGITS || state > 17) return;
            this->state[digit] = pgm_read_byte_near(DECODER + state);
        }

        void refresh(unsigned int lightTime = 0) {
            size_t i = 0;
            for(auto segment : segments) {
                digitalWrite(segment, HIGH);

                size_t j = 0;
                for(auto digitState : state) {
                    digitalWrite(digits[j++],(digitState & 1 << i) == 0);
                }

                // Give the segment some time to light up
                delayMicroseconds(lightTime);

                j = 0;
                for(auto digitState : state) {
                    digitalWrite(digits[j++], HIGH);
                }

                digitalWrite(segment, LOW);
                i += 1;
            }
        }

    private:
        uint8_t segments[7] = {-1};
        uint8_t digits[DIGITS] = {-1};

        uint8_t state[DIGITS] = {EMPTY};
};

} // namespace SegmentDisplay
} // namespace HWP

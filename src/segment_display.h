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

template<size_t NUM_DIGITS>
class Driver {
    public:
        /// Turn all segments off.
        const static size_t EMPTY = 16;
        /// Only enable the segment in the middle (`G`).
        const static size_t DASH = 17;

        Driver(std::initializer_list<uint8_t> digits, std::initializer_list<uint8_t> pins) {
            size_t i = 0;
            for(auto pin : digits) {
                if(i >= NUM_DIGITS) break;
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

        /// Change the digits to the given ones.
        ///
        /// \see setDigit()
        /// \param digits A list of exactly NUM_DIGITS digits.
        void setDigits(std::initializer_list<uint8_t> digits) {
            size_t i = 0;
            for(auto digit : digits) {
                setDigit(i++, digit);
            }
        }

        /// Change the digit at index num to the given one.
        ///
        /// All digits passed through the seven segment decoder, so you can
        /// pass decimal digits and the constants defined in this class.
        ///
        /// \param num The index of the digit to change
        /// \param digit The new digit
        void setDigit(size_t num, uint8_t digit) {
            if(index >= DIGITS || digit > 17) return;
            this->state[num] = pgm_read_byte_near(DECODER + digit);
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
        uint8_t digits[NUM_DIGITS] = {-1};

        uint8_t state[NUM_DIGITS] = {EMPTY};
};

} // namespace SegmentDisplay
} // namespace HWP

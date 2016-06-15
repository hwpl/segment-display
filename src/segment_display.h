#ifndef _SEGMENT_DISPLAY
#define _SEGMENT_DISPLAY

#include <avr/pgmspace.h>

#include "std/initializer_list.h"

namespace SegmentDisplay {

// Simple seven segment decimal decoder
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

/// %Driver for a multiplexed segment display.
///
/// \tparam NUM_DIGITS Specifies the number of digits of the segment display.
///
/// ## Example
///
/// ```
/// using SegmentDisplay::Driver;
///
/// Driver<4> seg({2, 3, 4, 5}, {6, 7, 8, 9, 10, 11, 12});
///
/// seg.setDigits({Driver::DASH, 4, 2, Driver::DASH});
///
/// void loop() {
///     seg.refresh();
/// }
/// ```
template<size_t NUM_DIGITS>
class Driver {
    public:
        /// Turn all segments off.
        const static size_t EMPTY = 16;
        /// Only enable the segment in the middle (`G`).
        const static size_t DASH = 17;

        /// Create a new driver instance using the given pins
        ///
        /// \param digit_pins **Exactly** NUM_DIGITS pins to use for the led
        ///                   cathodes of each digit.
        /// \param segment_pins 7 or 8 pins to use for the segments (a to g/h).
        Driver(std::initializer_list<uint8_t> digit_pins, std::initializer_list<uint8_t> segment_pins) {
            size_t i = 0;
            for(auto pin : digit_pins) {
                if(i >= NUM_DIGITS) break;
                this->digit_pins[i++] = pin;
                pinMode(pin, OUTPUT);
            }
            i = 0;
            for(auto pin : segment_pins) {
                if(i >= 8) break;
                this->segment_pins[i++] = pin;
                pinMode(pin, OUTPUT);
            }

            for(i = 0; i < NUM_DIGITS; ++i) {
                setDigit(i, EMPTY);
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

        /// Refresh on the display
        ///
        /// Turns on the needed segments of all digits for a short amount of
        /// time. This *must* be called repeatedly in your main loops to trick
        /// the human brain into believing the display is constantly lit.
        ///
        /// \param light_time The amount of time each segment is lit up. The
        ///                   default of `0` is sufficient for most cases.
        void refresh(unsigned int light_time = 0) {
            size_t i = 0;
            for(auto segment : segment_pins) {
                // If only 7 segments were specified
                if(segment < 0) continue;

                digitalWrite(segment, HIGH);

                size_t j = 0;
                for(auto digitState : state) {
                    digitalWrite(digit_pins[j++],(digitState & 1 << i) == 0);
                }

                // Give the segment some time to light up
                delayMicroseconds(light_time);

                j = 0;
                for(auto digitState : state) {
                    digitalWrite(digit_pins[j++], HIGH);
                }

                digitalWrite(segment, LOW);
                i += 1;
            }
        }

    private:
        uint8_t segment_pins[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
        uint8_t digit_pins[NUM_DIGITS];

        uint8_t state[NUM_DIGITS];
};

} // namespace SegmentDisplay

#endif // _SEGMENT_DISPLAY

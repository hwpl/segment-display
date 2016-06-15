# segment-display

Arduino library to drive a multiplexed segment display. Includes a simple
decimal decoder.

## Example

```cpp
using SegmentDisplay::Driver;

Driver<4> seg({2, 3, 4, 5}, {6, 7, 8, 9, 10, 11, 12});

seg.setDigits({Driver::DASH, 4, 2, Driver::DASH});

void loop() {
    seg.refresh();
}
```

## Documentation

The [api documentation] can be generated using `doxygen`.

## Licence

This library by Klemens Schölhorn is licenced under the terms of the GPLv3.

[api documentation]: https://hwpl.github.io/segment-display/

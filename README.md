# segment-display

Arduino library to drive a multiplexed segment display. Includes a simple
decimal decoder.

## Example

```cpp
using Seg = SegmentDisplay::Driver<4>;

Seg seg({2, 3, 4, 5}, {6, 7, 8, 9, 10, 11, 12});

seg.setDigits({Seg::DASH, 4, 2});
seg.setSegments(3, 0b00001000); // underscore

void loop() {
    seg.refresh();
}
```

## Documentation

The [api documentation] can be generated using `doxygen`.

## Licence

This library by Klemens Schölhorn is licenced under the terms of the GPLv3.

[api documentation]: https://hwpl.github.io/segment-display/

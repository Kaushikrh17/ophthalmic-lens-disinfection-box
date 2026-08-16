# Wiring Guide

## Arduino Uno

### Lid / metal-contact switch

```text
Metal contact 1  ---- GND
Metal contact 2  ---- D5
```

D5 is configured as:

```cpp
pinMode(D5, INPUT_PULLUP);
```

So the contacts behave as:

```text
OPEN   -> HIGH
CLOSED -> LOW
```

## LEDs

Use appropriate current-limiting resistors for discrete LEDs.

```text
D6 -> resistor -> RED LED -> GND
D2 -> resistor -> GREEN LED -> GND
D4 -> resistor -> BLUE LED 1 -> GND
D3 -> resistor -> BLUE LED 2 -> GND
```

## Buzzer

For a small active/passive buzzer compatible with the Arduino output:

```text
D7 -> Buzzer -> GND
```

For a higher-current buzzer, use a transistor driver.

## Blue LED demonstration output

The blue demonstration LED is controlled from D8. If a future version uses a higher-power light source, do not connect that load directly to an Arduino I/O pin.

Recommended conceptual connection:

```text
Arduino D8
    |
    v
MOSFET / transistor driver
    |
    v
blue demonstration LED
    |
   GND
```

For a future UV implementation, the actual driver must be selected according to the UV source voltage/current.

## I2C LCD

For an Arduino Uno:

```text
LCD VCC -> 5V
LCD GND -> GND
LCD SDA -> A4
LCD SCL -> A5
```

The sketch assumes LCD address `0x27`.

## Power

The original prototype used a battery/power supply arrangement. For a rebuilt version, the power system should be designed around the actual UV source and current requirements.

Do not attempt to power a high-current UV lamp from the Arduino 5 V rail.

## Physical arrangement

The prototype concept shown in the development photographs includes:

- enclosure/box
- foam/cushioning inside the box for lens positioning
- UV light positioned inside the upper portion of the enclosure
- LEDs and buzzer connected to the Arduino
- metal contacts positioned so they meet when the lid is closed

The lens holder should keep the optical surfaces appropriately positioned relative to the intended light source while preventing scratching or mechanical damage.

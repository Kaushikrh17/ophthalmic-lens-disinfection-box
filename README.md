# Ophthalmic Lens Disinfection Box Prototype

A low-cost Arduino Uno based prototype for timed disinfection of ophthalmic contact lenses, specifically an **applanation prism (AT prism)** and **gonioscopy lens**.

The project was developed as a prototype for **ACEi Hackathon '25**, organized by the Aravind Centre for Eye Care Innovation at Aravind Eye Hospital, Pondicherry, from January 3–5, 2025.

## Problem

Applanation prisms and gonioscopy lenses are contact devices used routinely in glaucoma clinics. Cleaning between patients can be cumbersome, and inconsistent cleaning practices can increase the risk of cross-contamination.

The hackathon challenge focused on developing a practical, low-cost system that could support cleaning/disinfection of these ophthalmic devices.

## Prototype Implementation Note

The intended concept was based on UV disinfection. However, a UV source was **not available during the hackathon prototype build**, so a **blue LED was used as a visual/demonstration substitute**.

Therefore, this prototype demonstrates the enclosure, lid-contact detection, timed control, indicators, buzzer and LCD workflow. The blue LED **must not be described as providing UV disinfection or sterilization**.

## Prototype Concept

The prototype uses a compact enclosure with:

- Arduino Uno as the controller
- Blue LED mounted inside the lid/enclosure as a demonstration light source
- Cushioning/foam insert to hold the ophthalmic lens/prism
- Two metal contacts on the enclosure acting as a lid-closed switch
- Red LED for an active disinfection cycle
- Green LED for cycle completion
- Blue LEDs as additional UV/process indicators
- Buzzer for start/completion alerts
- 16x2 I2C LCD for status display
- Battery/power supply
- Transistor/MOSFET/driver stage for switching the UV load

### Basic workflow

```text
          BOX OPEN
             |
             v
       System READY
             |
       Close the lid
             |
     Metal contacts meet
             |
             v
      Arduino detects LOW
             |
             v
       Start timed cycle
        /      |       \
       /       |        \
      v        v         v
   UV ON    Red LED    LCD status
             |
             v
       Timer completes
             |
             v
          UV OFF
             |
      Green LED + buzzer
             |
             v
        Open the lid
             |
             v
        System RESET
```

## Hardware

| Component | Purpose |
|---|---|
| Arduino Uno | Main controller |
| Blue LED | Demonstration light source |
| 9 V battery / suitable power source | Prototype power |
| Red LED | Cycle active indication |
| Green LED | Cycle complete indication |
| 2 × Blue LEDs | Additional process indication |
| Buzzer | Audible status indication |
| 16×2 I2C LCD | Status and timer display |
| Metal contacts | Lid closed/open detection |
| Foam/cushioning | Holds and protects the lens |
| Enclosure | Houses the prototype |
| Transistor/MOSFET/driver | Switches the UV load |

## Arduino Pin Configuration

| Arduino Uno pin | Connected component | Function |
|---|---|---|
| D2 | Green LED | Cycle complete |
| D3 | Blue LED 2 | Process indicator |
| D4 | Blue LED 1 | Process indicator |
| D5 | Metal contacts | Lid detection |
| D6 | Red LED | Cycle active |
| D7 | Buzzer | Audible alert |
| D8 | UV driver | UV ON/OFF control |
| A4 | LCD SDA | I2C data |
| A5 | LCD SCL | I2C clock |

### Lid detection

The prototype uses the metal ends of the enclosure as a simple normally-open mechanical contact.

One contact is connected to **GND** and the other to **D5**.

The Arduino uses:

```cpp
pinMode(CLAMP_PIN, INPUT_PULLUP);
```

Therefore:

- **Lid open → HIGH**
- **Lid closed → LOW**

When the contacts meet, the circuit is completed and the Arduino starts the cycle.

## Control Logic

The current prototype uses a **10-second demonstration timer**.

### When the box is closed

1. Arduino detects the closed contacts.
2. Blue LED turns ON as the demonstration output.
3. Red LED turns ON.
4. Blue indicator LEDs turn ON.
5. LCD displays the active cycle and remaining time.
6. Buzzer provides a short start alert.

### When the timer finishes

1. Blue LED turns OFF.
2. Blue LEDs turn OFF.
3. Red LED turns OFF.
4. Green LED turns ON.
5. LCD displays `CYCLE COMPLETE`.
6. Buzzer provides a completion alert.

### If the box is opened

The system immediately:

- turns OFF the UV output
- turns OFF the process LEDs
- stops the buzzer
- resets the cycle
- returns the LCD to `BOX OPEN / READY`

This prevents the UV output from continuing after the enclosure has been opened.

## LCD

The code assumes a common **16×2 I2C LCD** with address:

```cpp
0x27
```

If the LCD uses another I2C address, change:

```cpp
LiquidCrystal_I2C lcd(0x27, 16, 2);
```

For example, some modules use `0x3F`.

## Software Requirements

Arduino IDE with:

- Arduino Uno board package
- `Wire.h`
- `LiquidCrystal_I2C` library

The `Wire` library is included with the Arduino environment. Install a compatible `LiquidCrystal_I2C` library through the Arduino Library Manager if it is not already installed.

## Running the Prototype

1. Open `arduino/UV_Lens_Disinfection_Box.ino` in Arduino IDE.
2. Select **Arduino Uno**.
3. Select the appropriate processor for your Nano if required.
4. Connect the hardware according to the pin table.
5. Install the `LiquidCrystal_I2C` library.
6. Upload the sketch.
7. Power the circuit.
8. The LCD should display `UV LENS BOX / READY`.
9. Close the box so that the metal contacts meet.
10. The timed prototype cycle begins automatically.

## Important Electrical Note

The Arduino Uno should **not drive a high-power UV lamp directly from an I/O pin**.

D8 should control a suitable transistor, MOSFET, relay module, or dedicated driver stage appropriate for the UV source and its current/voltage requirements.

The exact driver and power arrangement used in the physical prototype should be verified before recreating the circuit.

## Safety and Validation

This repository represents a **hackathon proof-of-concept**, not a clinically validated sterilization system.

UV-C radiation can cause serious eye and skin injury. The enclosure must prevent UV leakage during operation, and the device should not be used with people exposed to the UV source.

A real clinical product would require, at minimum:

- UV irradiance measurement with an appropriate validated UV source
- dose verification at all relevant surfaces
- microbiological efficacy testing
- validation for the specific lens/prism materials
- enclosure UV-leakage testing
- electrical and thermal safety testing
- validated cleaning/disinfection protocol
- appropriate medical-device/regulatory assessment

The 10-second duration in this repository is a **demonstration timer from the hackathon prototype** and has no clinical disinfection significance.

## Project Structure

```text
UV_Lens_Disinfection_Hackathon_Repo/
│
├── README.md
├── LICENSE
│
├── arduino/
│   └── UV_Lens_Disinfection_Box.ino
│
├── docs/
│   └── WIRING.md
│
└── images/
    ├── prototype_sketch.jpeg
    ├── prototype_hardware_1.jpg
    └── prototype_hardware_2.jpg
```

## Prototype Images

The `images/` directory contains the original prototype sketch and hardware photographs from the hackathon development.

## Future Improvements

Potential next iterations include:

- 3D-printed enclosure with dedicated lens receptacles
- Separate compartments for cleaning/disinfection fluids and waste water
- More robust lid interlock
- Validated UV source, leakage detection and shielding
- Adjustable validated exposure dose
- Real-time UV intensity monitoring
- Rechargeable battery system
- Improved LCD/UI
- Modular lens holders
- Automated cleaning/rinsing stages
- Data logging for cycle history
- Clinical and microbiological validation

## Disclaimer

This project is an educational/hackathon prototype intended to demonstrate an automated enclosure and timed-control concept. It is not a substitute for an approved clinical disinfection or sterilization device.

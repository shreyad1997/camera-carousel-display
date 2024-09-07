**Project: Vintage Camera Digital Photo Viewer**

This project repurposes a vintage camera into a modern digital photo viewer, allowing users to display photos from a microSD card on a small LCD screen. The photos can be viewed through the camera’s original lens, maintaining a retro aesthetic while adding modern functionality. The device is powered by a rechargeable LiPo battery, and all components are controlled by an Arduino Nano.

### Features:
- Display stored photos on an LCD screen
- View photos through the camera's lens
- Powered by a rechargeable LiPo battery
- Managed by an Arduino Nano with microSD card storage
- Battery charge-discharge management

### Components:
- Arduino Nano
- 1.28" LCD Display Module
- MicroSD card reader module
- LiPo battery (3.7V)
- Charge-Discharge module
- Jumper wires
- Breadboard for initial testing
- MicroSD card with photos

### Assembly Overview:
1. **Power Connections**: Connect the LiPo battery to the charge-discharge module and power rails on the breadboard.
2. **Arduino Nano**: Connect the 5V pin to the positive power rail and GND pin to the ground rail.
3. **MicroSD and LCD Display**: Connect both modules' power pins to the power rails. Use jumper wires to connect their communication pins (SPI) to the Arduino Nano.
4. **Programming**: Upload code to the Arduino Nano to read photos from the microSD card and display them on the LCD.
5. **Enclosure**: Install components inside the vintage camera, positioning the LCD screen behind the camera's lens.

### Final Notes:
This project combines retro design with modern technology, preserving the look of a vintage camera while adding a digital photo-viewing feature.

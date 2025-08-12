# 🏎️ Engine & Dashboard Simulation

A simulation-powered dashboard project that blends software and hardware for a realistic engine experience.

## 📜 Overview
This project features:
- **Arduino-based engine simulation** providing real-time speed, RPM, and gear data.
- **Physical dashboard** with:
  - Speed display
  - RPM gauge
  - Limiter LED
  - Gear indicator
- **Serial terminal control** for throttle input and engine on/off toggling.

The simulation runs in Arduino, calculates engine state, and sends data to the dashboard for live updates.

## 🛠️ How It Works
1. **Simulation**: Arduino processes throttle, gear, and engine state.
2. **Communication**: Data sent to dashboard hardware.
3. **Dashboard Reaction**: Displays correct values and triggers LEDs accordingly.

## 🔧 Requirements
- Arduino board
- Serial terminal software
- Dashboard components (LEDs, 7-segment displays, or LCD)
- Basic electronics setup

## 🚀 Usage
1. Upload simulation code to Arduino.
2. Connect dashboard hardware.
3. Open Serial Monitor/terminal.
4. Control throttle & engine using 0-255 to throttle and -1 to turn off engine. Any positive value will turn the engine on.



💡 **Future Ideas**
- Add clutch & brake simulation
- Create standalone PC GUI interface

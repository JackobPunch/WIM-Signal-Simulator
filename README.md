# Signal Simulator for WIM Systems

Hardware and firmware for simulating the sensor signals of a Weigh-in-Motion (WIM) road installation, built as part of an engineering thesis at AGH University of Science and Technology.

> **"Signal simulator from the dynamic vehicle weighing system"**
> Jakub Cios, Faculty of Electrical Engineering, Automatics, Computer Science and Biomedical Engineering, 2025
> Supervisor: dr hab. inż. Ryszard Sroka, prof. AGH

Full thesis (LaTeX source + PDF): [WIM-Signal-Simulator-Thesis](https://github.com/jackobpunch/WIM-Signal-Simulator-Thesis)
Desktop control panel app: [WIM-Signal-Simulator-App](https://github.com/jackobpunch/WIM-Signal-Simulator-App)

---

## What this is

WIM (Weigh-in-Motion) systems measure vehicle weight while the vehicle is moving. They use two types of sensors embedded in the road:

- **Pressure sensors** (strain gauge / tensometric) — measure axle loads; signal amplitude 0–5 mV
- **Inductive loop sensors** — detect vehicle presence, speed, direction, and class

This simulator reproduces both signal types electrically, so the weighing computer (CAT Traffic CLW21) can be tested in a lab without real vehicles passing over the installation.

---

## Hardware

| Component | Role |
|-----------|------|
| Arduino Uno R4 Renesas WiFi | Microcontroller |
| MCP4728 (4-channel 12-bit DAC) | Pressure sensor signal generation |
| RC low-pass filter — 150 Ω, 10 µF (fc = 106 Hz) | Smooths DAC staircase output |
| Voltage dividers — 100 kΩ / 100 Ω | Scales DAC output down to 0–5 mV range |
| Relay Shield v3.0 (HLS8L-DC5V-S-C relays) | Controls inductive loop coils |
| Custom wound inductors — 72 µH, 0.3 mm² copper wire | Simulate inductive loop sensors |

Build photos and videos are in the `gallery/` folder.

---

## Repository structure

```
codes/
  ArduinUno/                     — main platform (final design)
    semi_trailer_generator/      — main simulation firmware (real waveform data)
    semi_trailer_generator_v2/   — improved version with ring-buffer channel delay
    semi_trailer_square_wave_test/ — rectangular signal variant (thesis test phase 2)
    semi_trailer_relay_test/     — active relay control + rectangular signals
    all4signals/                 — early 4-channel CSV generation test
    4signals08_12/               — early timing experiments
    CharReceiving/               — UART receive test
    IntReceiving/                — UART receive test (integer)
    FlutterCommunication/        — serial communication test with desktop app
    GaussGeneration/             — Gaussian signal generation experiment
    CSVdac5signal/               — CSV-driven DAC test
  ArduinoPortenta/               — early prototype (abandoned — I2C bus issues)
  esp32/ Esp32/                  — ESP32 experiments (abandoned — SD card issues)
  Comparison/                    — speed benchmark: Arduino Portenta vs ESP32
  Matlab/                        — signal analysis and waveform generation scripts
  Python/                        — data processing, CSV manipulation, serial scripts

data/
  vehicle1–vehicle10/            — CSV files with real sensor recordings from the WIM test site
  Wyniki_testów/                 — test session results (multiple dates, 2023–2024)

gallery/                         — build photos and videos
```

---

## How it works

### Signal data origin

Real vehicles were driven over the AGH WIM test installation. The weighing computer recorded sensor signals at 5 kHz sampling rate, saved as CSV files (now in `data/`). FFT analysis showed relevant signal harmonics up to ~80 Hz, so a generation rate above 240 Hz was required.

### Firmware approach

Reading full CSV files from an SD card at runtime introduced unacceptable delays on both Arduino Portenta (I2C bus issue) and Arduino Uno (insufficient RAM). The solution: every 6th sample from the CSV was extracted and hardcoded as arrays directly in the firmware, giving a generation rate of ~833 Hz. Between axle events, a constant baseline value is output.

The main firmware (`semi_trailer_generator/`) simulates a 5-axle articulated truck (semi-trailer, 2+3 axle configuration), the vehicle used in all thesis tests. Each of the 5 axle arrays drives channels A and B of the MCP4728 with a configurable inter-channel delay to reproduce the small timing offset between the two pressure sensor lines.

### Speed scaling

The `adjustSpeed()` function in v2 onward scales inter-peak delays and relay timing proportionally to the chosen speed relative to a 50 km/h reference, allowing simulation at 50, 60, or 70 km/h.

### Inductive loop simulation

Two custom inductors (72 µH each) are connected in series. One is periodically short-circuited by a relay, reducing the total inductance — the same effect a passing vehicle has on a real road loop. The relay is controlled by the Arduino on the same timing schedule as the pressure sensor peaks.

### Control

The simulator is operated via the [WIM-Signal-Simulator-App](https://github.com/jackobpunch/WIM-Signal-Simulator-App) Flutter app, which detects the Arduino via `arduino-cli`, compiles the firmware, and uploads it over USB.

---

## Test results

100 repeated simulations of a 5-axle articulated truck were run against the CLW21 weighing computer:

| Configuration | Mean total weight | Std dev | Relative error |
|---------------|-------------------|---------|----------------|
| Real waveforms, PC power supply | 31 839 kg | 162.6 kg | 0.51% |
| Square wave, PC power supply | 34 250 kg | 158.3 kg | 0.46% |
| Square wave, powerbank supply | 34 250 kg | 87.9 kg | 0.26% |

Power supply noise was identified as the main source of spread. Recommended next steps: shielded signal cables and a dedicated external power supply for the microcontroller.

---

## Dependencies

Install via Arduino Library Manager:

- `Adafruit MCP4728`
- `Wire`

Board package: `arduino:renesas_uno`

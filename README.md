# FlightControllerInput Plugin

### Unreal Engine 5 – HID-Based Flight Controller Input Integration

---

## Overview

**FlightControllerInput** is a Windows-only Unreal Engine plugin that provides low-level HID (Human Interface Device) access for flight controllers, joysticks, and RC-style input devices.
It detects USB flight controllers automatically, reads both analog and button data, and integrates them into the Unreal Engine input system as standard `FKey` bindings.

---

## Features

* Automatic detection of HID-compatible flight controllers (Joystick/Gamepad types)
* Supports up to **16 buttons** and **8 analog axes**
* Real-time input updates (button press, analog movement)
* Configurable **VID/PID** via `DefaultEngine.ini`
* Direct integration with Unreal Engine’s `IInputDeviceModule`
* No external dependencies
* Windows HID API backend (`hid.lib`, `setupapi.lib`)

---

## File Structure

```
FlightControllerInput/
├── FlightControllerInput.uplugin
├── Source/FlightControllerInput/
│   ├── FlightControllerInputModule.h/.cpp
│   ├── FlightControllerDevice.h/.cpp
│   ├── FlightControllerKeys.h/.cpp
│   ├── FlightControllerInput.Build.cs
└── Config/
    └── DefaultEngine.ini
```

---

## Installation

1. Copy the `FlightControllerInput` folder into your project’s `Plugins` directory:

   ```
   <YourProject>/Plugins/FlightControllerInput/
   ```
2. Launch Unreal Editor and verify that the plugin is detected under:
   **Edit → Plugins → Input → Flight Controller Input**
3. Enable the plugin and rebuild your project in Visual Studio, Rider, or Unreal Build Tool.

---

## Configuration

You can specify the flight controller’s **Vendor ID (VID)** and **Product ID (PID)** in
`Config/DefaultEngine.ini`.
If both are set to `0`, the plugin will automatically detect the first available game controller device.

```ini
[/Script/FlightControllerInput.FlightControllerSettings]
VendorID=4660
ProductID=22136
```

**Tips:**

* You can find the VID/PID through the Windows Device Manager or a HID enumeration tool (e.g., [pywinusb](https://pypi.org/project/pywinusb/)).
* Make sure the controller driver is properly installed before testing.

---

## Usage

When the plugin is enabled, Unreal Engine will register a set of new input keys that can be used in Blueprints or C++ bindings.

**Axis Keys:**
`FlightController_Axis0` … `FlightController_Axis7`

**Button Keys:**
`FlightController_Button0` … `FlightController_Button15`

### Blueprint Example

You can bind the custom axes and buttons in the Project Settings → Input → Action/Axis Mappings.
For example, bind `FlightController_Axis1` to control the pitch of your aircraft pawn.

### C++ Example

```cpp
void AMyDronePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    PlayerInputComponent->BindAxis("FlightController_Axis1", this, &AMyDronePawn::PitchInput);
    PlayerInputComponent->BindAxis("FlightController_Axis2", this, &AMyDronePawn::RollInput);
    PlayerInputComponent->BindAction("FlightController_Button0", IE_Pressed, this, &AMyDronePawn::ToggleEngine);
}
```

When running the project, the Output Log will display detected devices and live input data:

```
[LogFlightController] Flight Controller found and opened: VID=0x1234, PID=0x5678
[LogFlightController] Button 0 Pressed
[LogFlightController] Axis 1: -0.732
```

---

## Developer Notes

* The plugin registers itself as an input device module through `IInputDeviceModule` (see `FlightControllerInputModule.cpp`).
* HID handling is implemented in `FlightControllerDevice.cpp`, which performs device enumeration, data reading, and normalization.
* Custom key registration is handled by `FlightControllerKeys::Initialize()`.
* The build dependencies and libraries are declared in `FlightControllerInput.Build.cs`.
* Configuration parameters (VID/PID) are stored in `DefaultEngine.ini`.

---

## Compatibility

| Component             | Status                             |
| --------------------- | ---------------------------------- |
| Platform              | Windows (Win64)                    |
| Unreal Engine Version | 5.5 – 5.7                          |
| HID Device Type       | Joystick / Gamepad / RC Controller |
| Force Feedback        | Not Supported                      |

---

## License & Author

**Author:** YUQING DING
**Copyright:** © 2025 YUQING DING. All Rights Reserved.
**License:** Proprietary – All Rights Reserved
**Contact:** Private Developer – Unreal Flight Input Framework

---

## Summary

**FlightControllerInput** provides a clean, low-level integration for HID flight controllers on Windows platforms.
By exposing physical joystick and button data through Unreal’s input system, it allows developers to build flight simulators, RC training environments, and drone control experiments with full real-time input precision.

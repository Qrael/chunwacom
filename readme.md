chunwacom
---

`chunwacom` adds Wacom multi-touch tablet support and Leap Motion for Chunithm. For touch, it works by wrapping the API provided in Wacom Feel Multi-Touch SDK, then handles the touch input (with `chuniio.dll`).

`chunwacom` is based on [`chunithm-touch`](https://github.com/Nat-Lab/chunithm-touch) by [Nat](https://github.com/magicnat).

Headers for WacomMT API are modified from [sample code](https://github.com/Wacom-Developer/wacom-device-kit-windows) provided by Wacom.

### Installation

`chunwacom` requires `segatools` and Wacom drivers to work. If you are using some other tools to launch Chunithm, you are on your own.

If you are using `segatools`:

1. If you do not have [Wacom drivers](https://www.wacom.com/support/product-support/drivers) installed, go install them now.
2. Download `chunwacom.zip` from the [release](https://github.com/Joch2520/chunwacom/releases) page.
3. Unzip `chunwacom.zip`, copy everything in it to the `bin` folder of your game. Override any file that already exists. You may want to make a backup of your `bin` folder.
4. (Optional) If you plan to use a Leap Motion for AIR and AIR-actions, run `leapconfig.exe` to configure your Leap Motion controller. You may configure the controller manually in `chunwacom.ini` too.

### Usage

The program is designed to work with one multi-touch capable tablet only. If more than one touch tablets are connected, there is no guarantee which tablet the program will read from.

For playing, just tap/slide on the tablet. Slide up to simulate the IR sensor if you are using touch-bashed IR simulation. Raise your hand as if you were playing on the real arcade to simulate the IR sensor if you are using a Leap controller. A video demo of how touch controls work is available [here](https://youtu.be/Uknwet_-wWw). Use F1, F2, and F3 for test, service, and to insert coin.

### Configuration

Settings will be read from `chunwacom.ini`. Here's a list of configurable options:

```
[options]

[ir]
; source of control. 'touch' for touchscreen and 'leap' for leap motion.
control_source = touch
; height of each touch IR sensor (1000 stands for full vertical sensing height of the tablet)
touch_height = 30
; touch IR trigger threshold (amount required to move up for a move to
; be registered as air)
touch_trigger = 50
; specifies the axis to track hands on. x, y, z, -x, -y, or -z.
leap_orientation = y
; the minimum height of your hand(s) need to be for it to be registered as air
; (unit: millimeters)
leap_trigger = 100
; the height of each virtual IR sensor (unit: millimeters)
leap_step = 30

[io]
; use raw input
raw_input = 0

[misc]
; keep slider(s) holded while on air-action
ir_keep_slider = 0
```

### Building

To build chunwacom, you will need to get the Leap Motion standard Orion SDK package from [Leap Motion developer site](http://developer.leapmotion.com). Unzip the SDK package and copy the `LeapSDK` folder to `3rdparty/`.

You may build `chunwacom` on with any operating system that can run MinGW-w64. On Windows:

```
> meson build
> ninja -C build
```

On Unix-like:

```
$ meson --cross cross-build-32.txt build32
$ ninja -C build32
$ meson --cross cross-build-64.txt build64
$ ninja -C build64
```

Or, if you are using Windows and have Visual Studio installed, you may build it with Visual Studio:

```
> meson --backend vs build
> msbuild build\chunithm-wacom-tablet.sln
```

As Chunithm hard disk dump reveals the game is in 32bit, `chunwacom` should be built targeting 32bit. For some reason building with MinGW with 32bit target does not work for me, so please separate your MinGW and VS build folders for your own safety.

### License
UNLICENSE

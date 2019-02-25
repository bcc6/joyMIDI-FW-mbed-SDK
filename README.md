# joyMIDI-SDK
joyMIDI-SDK base on mbed-os-5.10.0


# Run mode
Configure `run_mode` in `mbed_app.json`, this will generate a macro `MBED_CONF_APP_RUN_MODE`

Run_mode  | MIDI-IN_serial  |  Debug_serial  |  Test_functions |      Jumper
----------|-----------------|----------------|-----------------|----------------------
0: Normal |       V         |       X        |        X        |       Short
1: Debug  |       X         |       V        |        X        |       Open
2: Test   |       X         |       V        |        V        |       Open

Jumper: MIDI-IN_path


# Setting up
VSCode [download](https://code.visualstudio.com/) and install.  
Mbed CLI [download](https://github.com/ARMmbed/mbed-cli-windows-installer/releases) and install.  
$ git clone https://github.com/bcc6/joyMIDI-SDK.git  
$ cd joyMIDI-SDK  
$ mbed compile -m NUMAKER_PFM_M453 -t GCC_ARM --profile .\MyProfile\my_profile_nano.json --stats-depth=2  


# newlib-nano (nanolibc)
Small code size but not thread safe.

API   | Available
------|-----------
RTOS  | X
Event | V


# Programming
 - ISP
 - ICP


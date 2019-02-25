# mbed-os-joyMIDI
joyMIDI SDK base on mbed-os


# run_mode in mbed_app.json (MBED_CONF_APP_RUN_MODE)

            | MIDI-IN_serial  |  Debug_serial  |  Test_functions |
            ------------------------------------------------------
  0: Normal |       V         |       X        |        X        |
  1: Debug  |       X         |       V        |        X        |
  2: Test   |       X         |       V        |        V        |


            | MIDI-IN_path    |
            -------------------
  0: Normal |       Short     |
  1: Debug  |       Open      |
  2: Test   |       Open      |
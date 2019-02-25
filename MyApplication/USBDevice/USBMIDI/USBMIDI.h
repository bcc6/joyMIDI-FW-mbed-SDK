#ifndef USBMIDI_H
#define USBMIDI_H

#include "mbed.h"
#include "USBEndpoints.h"
#include "USBDescriptor.h"
#include "USBDevice_Types.h"
#include "USBDevice.h"
#include "my_debug.h"


#define DEFAULT_CONFIGURATION (1)
#define CABLE_NUM (0<<4)
#define SYSEX_BUF_SIZE (64)


class USBMIDI : public USBDevice {    
public:
    USBMIDI(uint16_t vendor_id = 0x0700, uint16_t product_id = 0x0101, uint16_t product_release = 0x0001);

    /* Rx */
    inline void attachNoteOff(Callback<void(uint8_t, uint8_t, uint8_t)> fn)        { onNoteOff = fn; }
    inline void attachNoteOn (Callback<void(uint8_t, uint8_t, uint8_t)> fn)        { onNoteOn = fn; }
    inline void attachPolyAftertouch(Callback<void(uint8_t, uint8_t, uint8_t)> fn) { onPolyAftertouch = fn; }
    inline void attachControlChange(Callback<void(uint8_t, uint8_t, uint8_t)> fn)  { onControlChange = fn; }
    inline void attachProgramChange(Callback<void(uint8_t, uint8_t)> fn)           { onProgramChange = fn; }
    inline void attachChannelAftertouch(Callback<void(uint8_t, uint8_t)> fn)       { onChannelAftertouch = fn; }
    inline void attachPitchBend(Callback<void(uint8_t, uint16_t)> fn)              { onPitchBend = fn; }
    inline void attachSystemExclusive(Callback<void(uint8_t*, uint16_t, bool)> fn) { onSystemExclusive = fn; }
    inline void attachTimeCodeQuarterFrame(Callback<void(uint8_t)> fn)      { onTimeCodeQuarterFrame = fn; }
    inline void attachSongPositionPointer(Callback<void(uint16_t)> fn)      { onSongPositionPointer = fn;}
    inline void attachSongSelect(Callback<void(uint8_t)> fn)                { onSongSelect = fn; }
    inline void attachTuneRequest(Callback<void()> fn)                      { onTuneRequest = fn; }
    inline void attachTimingClock(Callback<void()> fn)   { onTimingClock = fn; }
    inline void attachStart(Callback<void()> fn)         { onStart = fn; }
    inline void attachContinue(Callback<void()> fn)      { onContinue = fn; }
    inline void attachStop(Callback<void()> fn)          { onStop = fn; }
    inline void attachActiveSensing(Callback<void()> fn) { onActiveSensing = fn; }
    inline void attachReset(Callback<void()> fn)         { onReset = fn; }

    /* Tx */
    void sendNoteOff(uint8_t channel, uint8_t note, uint8_t velocity);
    void sendNoteOn(uint8_t channel, uint8_t note, uint8_t velocity);
    void sendPolyAftertouch(uint8_t channel, uint8_t note, uint8_t pressure);
    void sendControlChange(uint8_t channel, uint8_t function, uint8_t value);
    void sendProgramChange(uint8_t channel, uint8_t program);
    void sendChannelAftertouch(uint8_t channel, uint8_t pressure);
    void sendPitchBend(uint8_t channel, uint16_t amount);
    void sendSystemExclusive(uint8_t *pkt, uint16_t length, bool hasNextPkt);
    void sendTimeCodeQuarterFrame(uint8_t timing);
    void sendSongPositionPointer(uint16_t position);
    void sendSongSelect(uint8_t song);
    void sendTuneRequest();
    void sendTimingClock();
    void sendStart();
    void sendContinue();
    void sendStop();
    void sendActiveSensing();
    void sendReset();
    void sendMMC(uint8_t mmc);
    void sendUserDefined(uint8_t *buf, uint8_t length);

protected:
    virtual bool EPBULK_OUT_callback();
    virtual bool USBCallback_setConfiguration(uint8_t configuration);
    virtual const uint8_t * stringIproductDesc();
    virtual const uint8_t * stringIinterfaceDesc();
    virtual const uint8_t * configurationDesc();

private:
    Callback<void(uint8_t, uint8_t, uint8_t)> onNoteOff;
    Callback<void(uint8_t, uint8_t, uint8_t)> onNoteOn;
    Callback<void(uint8_t, uint8_t, uint8_t)> onPolyAftertouch;
    Callback<void(uint8_t, uint8_t, uint8_t)> onControlChange;
    Callback<void(uint8_t, uint8_t)>          onProgramChange;
    Callback<void(uint8_t, uint8_t)>          onChannelAftertouch;
    Callback<void(uint8_t, uint16_t)>         onPitchBend;
    Callback<void(uint8_t *, uint16_t, bool)> onSystemExclusive;
    Callback<void(uint8_t)>                   onTimeCodeQuarterFrame;
    Callback<void(uint16_t)>                  onSongPositionPointer;
    Callback<void(uint8_t)>                   onSongSelect;
    Callback<void()>                          onTuneRequest;
    Callback<void()>                          onTimingClock;
    Callback<void()>                          onStart;
    Callback<void()>                          onContinue;
    Callback<void()>                          onStop;
    Callback<void()>                          onActiveSensing;
    Callback<void()>                          onReset;


    /**** MIDI Paser ****/

    enum MidiMessage {
        NoteOff               = 0x80,    // Channel Voice - Note Off : 3bytes
        NoteOn                = 0x90,    // Channel Voice - Note On : 3bytes
        PolyAfterTouch        = 0xA0,    // Channel Voice - Polyphonic AfterTouch : 3bytes
        ControlChange         = 0xB0,    // Channel Voice - Control Change / Channel Mode : 3bytes
        ProgramChange         = 0xC0,    // Channel Voice - Program Change : 2bytes
        ChannelAfterTouch     = 0xD0,    // Channel Voice - Channel (monophonic) AfterTouch : 2bytes
        PitchBend             = 0xE0,    // Channel Voice - Pitch Bend : 3bytes
        SystemExclusive       = 0xF0,    // System Common - System Exclusive : nbytes
        TimeCodeQuarterFrame  = 0xF1,    // System Common - MIDI Time Code Quarter Frame : 2bytes
        SongPosition          = 0xF2,    // System Common - Song Position Pointer : 3bytes
        SongSelect            = 0xF3,    // System Common - Song Select : 2bytes
        TuneRequest           = 0xF6,    // System Common - Tune Request : 1byte
        EOX                   = 0xF7,    // System Common - End of Exclusive : 1byte
        TimingClock           = 0xF8,    // System Real Time - Timing Clock : 1byte
        Start                 = 0xFA,    // System Real Time - Start : 1byte
        Continue              = 0xFB,    // System Real Time - Continue : 1byte
        Stop                  = 0xFC,    // System Real Time - Stop : 1byte
        ActiveSensing         = 0xFE,    // System Real Time - Active Sensing : 1byte
        Reset                 = 0xFF,    // System Real Time - System Reset : 1byte
    };

    uint8_t   sysex[SYSEX_BUF_SIZE];
    uint16_t  sysex_pos;
};

#endif

#include "USBMIDI.h"


USBMIDI::USBMIDI(uint16_t vendor_id, uint16_t product_id, uint16_t product_release)
    : USBDevice(vendor_id, product_id, product_release), sysex(), sysex_pos(0)
{
    USBDevice::connect();
}

void USBMIDI::sendNoteOff(uint8_t channel, uint8_t note, uint8_t velocity) {
    uint8_t buf[4];
    buf[0] = CABLE_NUM | 0x08;
    buf[1] = 0x80 | (channel & 0x0F);
    buf[2] = note & 0x7F;
    buf[3] = velocity & 0x7F;
    USBDevice::write(EPBULK_IN, buf, 4, MAX_PACKET_SIZE_EPBULK);
}

void USBMIDI::sendNoteOn(uint8_t channel, uint8_t note, uint8_t velocity) {
    uint8_t buf[4];
    buf[0] = CABLE_NUM | 0x09;
    buf[1] = 0x90 | (channel & 0x0F);
    buf[2] = note & 0x7F;
    buf[3] = velocity & 0x7F;
    USBDevice::write(EPBULK_IN, buf, 4, MAX_PACKET_SIZE_EPBULK);
}

void USBMIDI::sendPolyAftertouch(uint8_t channel, uint8_t note, uint8_t pressure) {
    uint8_t buf[4];
    buf[0] = CABLE_NUM | 0x0A;
    buf[1] = 0xA0 | (channel & 0x0F);
    buf[2] = note & 0x7F;
    buf[3] = pressure & 0x7F;
    USBDevice::write(EPBULK_IN, buf, 4, MAX_PACKET_SIZE_EPBULK);
}

void USBMIDI::sendControlChange(uint8_t channel, uint8_t function, uint8_t value) {
    uint8_t buf[4];
    buf[0] = CABLE_NUM | 0x0B;
    buf[1] = 0xB0 | (channel & 0x0F);
    buf[2] = function & 0x7F;
    buf[3] = value & 0x7F;
    USBDevice::write(EPBULK_IN, buf, 4, MAX_PACKET_SIZE_EPBULK);
}

void USBMIDI::sendProgramChange(uint8_t channel, uint8_t program) {
    uint8_t buf[4];
    buf[0] = CABLE_NUM | 0x0C;
    buf[1] = 0xC0 | (channel & 0x0F);
    buf[2] = program & 0x7F;
    buf[3] = 0x00;
    USBDevice::write(EPBULK_IN, buf, 4, MAX_PACKET_SIZE_EPBULK);
}

void USBMIDI::sendChannelAftertouch(uint8_t channel, uint8_t pressure) {
    uint8_t buf[4];
    buf[0] = CABLE_NUM | 0x0D;
    buf[1] = 0xD0 | (channel & 0x0F);
    buf[2] = pressure & 0x7F;
    buf[3] = 0x00;
    USBDevice::write(EPBULK_IN, buf, 4, MAX_PACKET_SIZE_EPBULK);
}

void USBMIDI::sendPitchBend(uint8_t channel, uint16_t amount) {
    uint8_t buf[4];
    buf[0] = CABLE_NUM | 0x0E;
    buf[1] = 0xE0 | (channel & 0x0F);
    buf[2] = amount & 0x7F;
    buf[3] = (amount >> 7) & 0x7F;
    USBDevice::write(EPBULK_IN, buf, 4, MAX_PACKET_SIZE_EPBULK);
}

void USBMIDI::sendSystemExclusive(uint8_t *pkt, uint16_t length, bool hasNextPkt) {
    uint8_t buf[64];
    uint8_t len = 0;

    while(length) {

        if (hasNextPkt) {
            // Not Last packet
            // SysEx starts or continues
            buf[len+0] = CABLE_NUM | 0x04;
            buf[len+1] = pkt[0];
            buf[len+2] = pkt[1];
            buf[len+3] = pkt[2];
            pkt += 3;
            length -= 3;
            len += 4;

            if (len == 64) {
                USBDevice::write(EPBULK_IN, buf, len, MAX_PACKET_SIZE_EPBULK);
                len = 0;
            }
        } else {
            // Last packet
            if (length > 3) {
                // SysEx starts or continues
                buf[len+0] = CABLE_NUM | 0x04;
                buf[len+1] = pkt[0];
                buf[len+2] = pkt[1];
                buf[len+3] = pkt[2];
                pkt += 3;
                length -= 3;
                len += 4;
            } else if (length == 1) {
                // SysEx ends with following single byte
                buf[len+0] = CABLE_NUM | 0x05;
                buf[len+1] = pkt[0];
                buf[len+2] = 0;
                buf[len+3] = 0;
                pkt += 1;
                length -= 1;
                len += 4;
                USBDevice::write(EPBULK_IN, buf, len, MAX_PACKET_SIZE_EPBULK);
            } else if (length == 2) {
                // SysEx ends with following two bytes
                buf[len+0] = CABLE_NUM | 0x06;
                buf[len+1] = pkt[0];
                buf[len+2] = pkt[1];
                buf[len+3] = 0;
                pkt += 2;
                length -= 2;
                len += 4;
                USBDevice::write(EPBULK_IN, buf, len, MAX_PACKET_SIZE_EPBULK);
            } else if (length == 3) {
                // SysEx ends with following three bytes
                buf[len+0] = CABLE_NUM | 0x07;
                buf[len+1] = pkt[0];
                buf[len+2] = pkt[1];
                buf[len+3] = pkt[2];
                pkt += 3;
                length -= 3;
                len += 4;
                USBDevice::write(EPBULK_IN, buf, len, MAX_PACKET_SIZE_EPBULK);
            }
        }
    } // End of while loop
}

void USBMIDI::sendTimeCodeQuarterFrame(uint8_t timing) {
    uint8_t buf[4];
    buf[0] = CABLE_NUM | 0x02;
    buf[1] = 0xF1;
    buf[2] = timing & 0x7F;
    buf[3] = 0x00;
    USBDevice::write(EPBULK_IN, buf, 4, MAX_PACKET_SIZE_EPBULK);
}

void USBMIDI::sendSongPositionPointer(uint16_t position) {
    uint8_t buf[4];
    buf[0] = CABLE_NUM | 0x03;
    buf[1] = 0xF2;
    buf[2] = position & 0x7F;
    buf[3] = (position >> 7) & 0x7f;
    USBDevice::write(EPBULK_IN, buf, 4, MAX_PACKET_SIZE_EPBULK);
}

void USBMIDI::sendSongSelect(uint8_t song) {
    uint8_t buf[4];
    buf[0] = CABLE_NUM | 0x02;
    buf[1] = 0xF3;
    buf[2] = song & 0x7F;
    buf[3] = 0x00;
    USBDevice::write(EPBULK_IN, buf, 4, MAX_PACKET_SIZE_EPBULK);
}

void USBMIDI::sendTuneRequest() {
    uint8_t buf[4];
    buf[0] = CABLE_NUM | 0x05;
    buf[1] = 0xF6;
    buf[2] = 0x00;
    buf[3] = 0x00;
    USBDevice::write(EPBULK_IN, buf, 4, MAX_PACKET_SIZE_EPBULK);
}

void USBMIDI::sendTimingClock() {
    uint8_t buf[4];
    buf[0] = CABLE_NUM | 0x0F;
    buf[1] = 0xF8;
    buf[2] = 0x00;
    buf[3] = 0x00;
    USBDevice::write(EPBULK_IN, buf, 4, MAX_PACKET_SIZE_EPBULK);
}

void USBMIDI::sendStart() {
    uint8_t buf[4];
    buf[0] = CABLE_NUM | 0x0F;
    buf[1] = 0xFA;
    buf[2] = 0x00;
    buf[3] = 0x00;
    USBDevice::write(EPBULK_IN, buf, 4, MAX_PACKET_SIZE_EPBULK);
}

void USBMIDI::sendContinue() {
    uint8_t buf[4];
    buf[0] = CABLE_NUM | 0x0F;
    buf[1] = 0xFB;
    buf[2] = 0x00;
    buf[3] = 0x00;
    USBDevice::write(EPBULK_IN, buf, 4, MAX_PACKET_SIZE_EPBULK);
}

void USBMIDI::sendStop() {
    uint8_t buf[4];
    buf[0] = CABLE_NUM | 0x0F;
    buf[1] = 0xFC;
    buf[2] = 0x00;
    buf[3] = 0x00;
    USBDevice::write(EPBULK_IN, buf, 4, MAX_PACKET_SIZE_EPBULK);
}

void USBMIDI::sendActiveSensing() {
    uint8_t buf[4];
    buf[0] = CABLE_NUM | 0x0F;
    buf[1] = 0xFE;
    buf[2] = 0x00;
    buf[3] = 0x00;
    USBDevice::write(EPBULK_IN, buf, 4, MAX_PACKET_SIZE_EPBULK);
}

void USBMIDI::sendReset() {
    uint8_t buf[4];
    buf[0] = CABLE_NUM | 0x0F;
    buf[1] = 0xFF;
    buf[2] = 0x00;
    buf[3] = 0x00;
    USBDevice::write(EPBULK_IN, buf, 4, MAX_PACKET_SIZE_EPBULK);
}

void USBMIDI::sendMMC(uint8_t mmc) {
    uint8_t buf[8];
    buf[0] = CABLE_NUM | 0x04;  // length > 3, SysEx starts or continues
    buf[1] = 0xF0;
    buf[2] = 0x7F;
    buf[3] = 0x7F;
    buf[4] = CABLE_NUM | 0x07;  // length == 3, SysEx ends with following three bytes
    buf[5] = 0x06;
    buf[6] = mmc;
    buf[7] = 0xF7;
    USBDevice::write(EPBULK_IN, buf, 8, MAX_PACKET_SIZE_EPBULK);
}

void USBMIDI::sendUserDefined(uint8_t *buf, uint8_t length){
    USBDevice::write(EPBULK_IN, buf, length, MAX_PACKET_SIZE_EPBULK);
}

bool USBMIDI::EPBULK_OUT_callback() {
    uint8_t buf[64];
    uint32_t len;
    readEP(EPBULK_OUT, buf, &len, sizeof(buf));

    for (uint32_t i=0; i<len; i+=4) {   

        switch(buf[i]) {
        case 0x2:   // Two-bytes System Common Message
            switch(buf[i+1]) {
            case TimeCodeQuarterFrame:        
                onTimeCodeQuarterFrame(buf[i+2]);
                break;
            case SongSelect:
                onSongSelect(buf[i+2]);
                break;
            }
            break;
        case 0x3:   // Three-byte System Common message
            onSongPositionPointer((buf[i+2] & 0x7f) | ((buf[i+3] & 0x7f) << 7));
            break;
        case 0x4:   // SysEx start or continue
            for (uint8_t j=1; j<=3; j++) {
                sysex[sysex_pos++] = buf[i+j];
                if (sysex_pos == sizeof(sysex)) {
                    onSystemExclusive(sysex, sysex_pos, 1);  // Split to many packets
                    sysex_pos = 0;
                }
            }
            break;
        case 0x5:   // Single-byte System Common Message or SysEx end with one byte
            switch(buf[i+1]) {
            case TuneRequest:
                onTuneRequest();
                break;
            case EOX:
                sysex[sysex_pos++] = buf[i+1];
                onSystemExclusive(sysex, sysex_pos, 0);
                sysex_pos = 0;
                break;
            }
            break;
        case 0x6:   // SysEx end with two bytes
            for (uint8_t j=1; j<=2; j++) {
                sysex[sysex_pos++] = buf[i+j];
                if (j == 2) {
                    onSystemExclusive(sysex, sysex_pos, 0);
                    sysex_pos = 0;
                } else if (sysex_pos == sizeof(sysex)) {
                    onSystemExclusive(sysex, sysex_pos, 1);  // Split to many packets
                    sysex_pos = 0;
                }
            }
            break;
        case 0x7:   // SysEx end with three bytes
            for (uint8_t j=1; j<=3; j++) {
                sysex[sysex_pos++] = buf[i+j];
                if (j == 3) {
                    onSystemExclusive(sysex, sysex_pos, 0);
                    sysex_pos = 0;
                } else if (sysex_pos == sizeof(sysex)) {
                    onSystemExclusive(sysex, sysex_pos, 1);  // Split to many packets
                    sysex_pos = 0;
                }
            }
            break;
        case 0x8:   // Note-off
            onNoteOff(buf[i+1] & 0x0F, buf[i+2], buf[i+3]);
            break;
        case 0x9:   // Note-on
            if (buf[i+3] == 0) {
                onNoteOff(buf[i+1] & 0x0F, buf[i+2], buf[i+3]);
            } else {
                onNoteOn(buf[i+1] & 0x0F, buf[i+2], buf[i+3]);
            }
            break;
        case 0xA:   // Poly-KeyPress
            onPolyAftertouch(buf[i+1] & 0x0F, buf[i+2], buf[i+3]);
            break;
        case 0xB:   // Control Change
            onControlChange(buf[i+1] & 0x0F, buf[i+2], buf[i+3]);
            break;
        case 0xC:   // Program change
            onProgramChange(buf[i+1] & 0x0F, buf[i+2]);
            break;
        case 0xD:   // Channel pressure
            onChannelAftertouch(buf[i+1] & 0x0F, buf[i+2]);
            break;
        case 0xE:   // PitchBend Change
            onPitchBend(buf[i+1] & 0x0F, (buf[i+2] & 0x7f) | ((buf[i+3] & 0x7f) << 7));
            break;
        case 0xF:   // Single byte
            switch(buf[i+1]) {
            case TimingClock:
                onTimingClock();
                break;
            case Start:
                onStart();
                break;
            case Continue:
                onContinue();
                break;
            case Stop:
                onStop();
                break;
            case ActiveSensing:
                onActiveSensing();
                break;
            case Reset:
                onReset();
                break;
            }
            break;    
        } 
    }   // End of for loop


    // We reactivate the endpoint to receive next characters
    readStart(EPBULK_OUT, MAX_PACKET_SIZE_EPBULK);
    return true;
}

// Called in ISR context
// Set configuration. Return false if the
// configuration is not supported.
bool USBMIDI::USBCallback_setConfiguration(uint8_t configuration) {
    if (configuration != DEFAULT_CONFIGURATION) {
        return false;
    }

    // Configure endpoints > 0
    addEndpoint(EPBULK_IN, MAX_PACKET_SIZE_EPBULK);
    addEndpoint(EPBULK_OUT, MAX_PACKET_SIZE_EPBULK);

    // We activate the endpoint to be able to receive data
    readStart(EPBULK_OUT, MAX_PACKET_SIZE_EPBULK);
    return true;
}

const uint8_t * USBMIDI::stringIinterfaceDesc() {
    static uint8_t stringIinterfaceDescriptor[] = {
        0x0c,                           //bLength
        STRING_DESCRIPTOR,              //bDescriptorType 0x03
        'A',0,'u',0,'d',0,'i',0,'o',0   //bString iInterface - Audio
    };
    return stringIinterfaceDescriptor;
}

const uint8_t * USBMIDI::stringIproductDesc() {
    static uint8_t stringIproductDescriptor[] = {
        0x22,                                                   //bLength
        STRING_DESCRIPTOR,                                      //bDescriptorType 0x03
        'G',0,'i',0,'t',0,'M',0,'u',0,'s',0,'i',0,'c',0,' ',0,
        'j',0,'o',0,'y',0,'M',0,'I',0,'D',0,'I',0,              //bString iProduct - GitMusic joyMIDI
    };
    return stringIproductDescriptor;
}

const uint8_t * USBMIDI::configurationDesc() {
    static uint8_t configDescriptor[] = {
        // configuration descriptor
        0x09, 0x02, 0x65, 0x00, 0x02, 0x01, 0x00, 0xc0, 0x32,   // MaxPower 100mA due to Apple limit

        // The Audio Interface Collection
        0x09, 0x04, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, // Standard AC Interface Descriptor
        0x09, 0x24, 0x01, 0x00, 0x01, 0x09, 0x00, 0x01, 0x01, // Class-specific AC Interface Descriptor
        0x09, 0x04, 0x01, 0x00, 0x02, 0x01, 0x03, 0x00, 0x00, // MIDIStreaming Interface Descriptors
        0x07, 0x24, 0x01, 0x00, 0x01, 0x41, 0x00,             // Class-Specific MS Interface Header Descriptor

        // MIDI IN JACKS
        0x06, 0x24, 0x02, 0x01, 0x01, 0x00,
        0x06, 0x24, 0x02, 0x02, 0x02, 0x00,

        // MIDI OUT JACKS
        0x09, 0x24, 0x03, 0x01, 0x03, 0x01, 0x02, 0x01, 0x00,
        0x09, 0x24, 0x03, 0x02, 0x06, 0x01, 0x01, 0x01, 0x00,

        // OUT endpoint descriptor
        0x09, 0x05, 0x05, 0x02, 0x40, 0x00, 0x00, 0x00, 0x00,   // Nuvoton's Bulk-out use EP5
        0x05, 0x25, 0x01, 0x01, 0x01,

        // IN endpoint descriptor
        0x09, 0x05, 0x86, 0x02, 0x40, 0x00, 0x00, 0x00, 0x00,   // Nuvoton's Bulk-in use EP6
        0x05, 0x25, 0x01, 0x01, 0x03,
    };
    return configDescriptor;
}


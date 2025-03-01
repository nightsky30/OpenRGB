/*-----------------------------------------*\
|  CorsairPeripheralController.h            |
|                                           |
|  Definitions and types for Corsair RGB    |
|  keyboard, mouse, and mousemat lighting   |
|  controller                               |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/9/2020    |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

enum
{
    CORSAIR_COMMAND_WRITE       = 0x07,
    CORSAIR_COMMAND_READ        = 0x0E,
    CORSAIR_COMMAND_STREAM      = 0x7F
};

enum
{
    CORSAIR_PROPERTY_FIRMWARE_INFO              = 0x01,
    CORSAIR_PROPERTY_RESET                      = 0x02,
    CORSAIR_PROPERTY_SPECIAL_FUNCTION           = 0x04,
    CORSAIR_PROPERTY_LIGHTING_CONTROL           = 0x05,
    CORSAIR_PROPERTY_HARDWARE_PROFILE           = 0x13,
    CORSAIR_PROPERTY_SUBMIT_MOUSE_COLOR         = 0x22,
    CORSAIR_PROPERTY_SUBMIT_KBZONES_COLOR_24    = 0x25,
    CORSAIR_PROPERTY_SUBMIT_KEYBOARD_COLOR_9    = 0x27,
    CORSAIR_PROPERTY_SUBMIT_KEYBOARD_COLOR_24   = 0x28,
};

enum
{
    CORSAIR_LIGHTING_CONTROL_HARDWARE           = 0x01,
    CORSAIR_LIGHTING_CONTROL_SOFTWARE           = 0x02
};

enum
{
    CORSAIR_COLOR_CHANNEL_RED       = 0x01,
    CORSAIR_COLOR_CHANNEL_GREEN     = 0x02,
    CORSAIR_COLOR_CHANNEL_BLUE      = 0x03
};

enum
{
    CORSAIR_LAYOUT_ANSI             = 0x00,
    CORSAIR_LAYOUT_ISO              = 0x01,
    CORSAIR_LAYOUT_ABNT             = 0x02,
    CORSAIR_LAYOUT_JIS              = 0x03,
    CORSAIR_LAYOUT_DUBEOLSIK        = 0x04
};

enum
{
    CORSAIR_TYPE_NORMAL             = 0,
    CORSAIR_TYPE_K95_PLAT           = 1,
    CORSAIR_TYPE_K95                = 2,
    CORSAIR_TYPE_K55                = 3
};

class CorsairPeripheralController
{
public:
    CorsairPeripheralController(hid_device* dev_handle, const char* path);
    ~CorsairPeripheralController();

    int             GetLogicalLayout();
    int             GetPhysicalLayout();
    device_type     GetDeviceType();
    std::string     GetDeviceLocation();
    std::string     GetFirmwareString();
    std::string     GetName();
    std::string     GetSerialString();

    void            SetLEDs(std::vector<RGBColor> colors);
    void            SetLEDsKeyboardFull(std::vector<RGBColor> colors);
    void            SetLEDsKeyboardLimited(std::vector<RGBColor> colors);
    void            SetLEDsMouse(std::vector<RGBColor> colors);
    void            SetLEDsMousemat(std::vector<RGBColor> colors);
    void            SetName(std::string device_name);

private:
    hid_device*             dev;

    std::string             firmware_version;
    std::string             location;
    std::string             name;
    device_type             type;
    int                     physical_layout;   //ANSI, ISO, etc.
    int                     logical_layout;    //Normal, K95 or K95 Platinum

    void    LightingControl();
    void    SetupK95LightingControl();
    void    SpecialFunctionControl();

    void    ReadFirmwareInfo();
    
    void    StreamPacket
                (
                unsigned char   packet_id,
                unsigned char   data_sz,
                unsigned char*  data_ptr
                );

    void    SubmitKeyboardFullColors
                (
                unsigned char   color_channel,
                unsigned char   packet_count,
                unsigned char   finish_val
                );

    void    SubmitKeyboardZonesColors
                (
                RGBColor left,
                RGBColor mid,
                RGBColor right
                );


    void    SubmitKeyboardLimitedColors
                (
                unsigned char   byte_count
                );

    void    SubmitMouseColors
                (
                unsigned char   num_zones,
                RGBColor *      color_data
                );

    void    SubmitMousematColors
            (
            unsigned char   num_zones,
            RGBColor *      color_data
            );
};

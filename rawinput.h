#ifndef RAWINPUT_H_INCLUDED
#define RAWINPUT_H_INCLUDED

#include <windows.h>

#define RIM_INPUT 0
#define RIM_INPUTSINK 1

typedef struct tagRAWINPUTHEADER {
     DWORD dwType;
     DWORD dwSize;
     HANDLE hDevice;
     WPARAM wParam;
} RAWINPUTHEADER, *PRAWINPUTHEADER, *LPRAWINPUTHEADER;

#define RIM_TYPEMOUSE 0
#define RIM_TYPEKEYBOARD 1
#define RIM_TYPEHID 2

typedef struct tagRAWMOUSE {
     USHORT usFlags;
     union {
          ULONG ulButtons;
          struct {
               USHORT usButtonFlags;
               USHORT usButtonData;
          };
     };
     ULONG ulRawButtons;
     LONG lLastX;
     LONG lLastY;
     ULONG ulExtraInformation;
} RAWMOUSE, *PRAWMOUSE, *LPRAWMOUSE;

#define RI_MOUSE_LEFT_BUTTON_DOWN 0x0001 // Left Button changed to down.
#define RI_MOUSE_LEFT_BUTTON_UP 0x0002 // Left Button changed to up.
#define RI_MOUSE_RIGHT_BUTTON_DOWN 0x0004 // Right Button changed to down.
#define RI_MOUSE_RIGHT_BUTTON_UP 0x0008 // Right Button changed to up.
#define RI_MOUSE_MIDDLE_BUTTON_DOWN 0x0010 // Middle Button changed to down.
#define RI_MOUSE_MIDDLE_BUTTON_UP 0x0020 // Middle Button changed to up.

#define RI_MOUSE_BUTTON_1_DOWN RI_MOUSE_LEFT_BUTTON_DOWN
#define RI_MOUSE_BUTTON_1_UP RI_MOUSE_LEFT_BUTTON_UP
#define RI_MOUSE_BUTTON_2_DOWN RI_MOUSE_RIGHT_BUTTON_DOWN
#define RI_MOUSE_BUTTON_2_UP RI_MOUSE_RIGHT_BUTTON_UP
#define RI_MOUSE_BUTTON_3_DOWN RI_MOUSE_MIDDLE_BUTTON_DOWN
#define RI_MOUSE_BUTTON_3_UP RI_MOUSE_MIDDLE_BUTTON_UP

#define RI_MOUSE_BUTTON_4_DOWN 0x0040
#define RI_MOUSE_BUTTON_4_UP 0x0080
#define RI_MOUSE_BUTTON_5_DOWN 0x0100
#define RI_MOUSE_BUTTON_5_UP 0x0200

#define RI_MOUSE_WHEEL 0x0400

#define MOUSE_MOVE_RELATIVE 0
#define MOUSE_MOVE_ABSOLUTE 1
#define MOUSE_VIRTUAL_DESKTOP 0x02 // the coordinates are mapped to the virtual desktop
#define MOUSE_ATTRIBUTES_CHANGED 0x04 // requery for mouse attributes

typedef struct tagRAWKEYBOARD {

     USHORT MakeCode;
     USHORT Flags;
     USHORT Reserved;
     USHORT VKey;
     UINT Message;
     ULONG ExtraInformation;


} RAWKEYBOARD, *PRAWKEYBOARD, *LPRAWKEYBOARD;

#define KEYBOARD_OVERRUN_MAKE_CODE 0xFF

#define RI_KEY_MAKE 0
#define RI_KEY_BREAK 1
#define RI_KEY_E0 2
#define RI_KEY_E1 4
#define RI_KEY_TERMSRV_SET_LED 8
#define RI_KEY_TERMSRV_SHADOW 0x10

typedef struct tagRAWHID {
     DWORD dwSizeHid; // byte size of each report
     DWORD dwCount; // number of input packed
     BYTE bRawData[1];
} RAWHID, *PRAWHID, *LPRAWHID;

typedef struct tagRAWINPUT {
     RAWINPUTHEADER header;
     union {
          RAWMOUSE mouse;
          RAWKEYBOARD keyboard;
          RAWHID hid;
     } data;
} RAWINPUT, *PRAWINPUT, *LPRAWINPUT;

#define RID_INPUT 0x10000003
#define RID_HEADER 0x10000005

typedef struct HRAWINPUT__ * HRAWINPUT;

typedef
UINT
(_stdcall * PGetRawInputData)(
     HRAWINPUT hRawInput,
     UINT uiCommand,
     LPVOID pData,
     PUINT pcbSize,
     UINT cbSizeHeader
);


#define RIDI_PREPARSEDDATA 0x20000005
#define RIDI_DEVICENAME 0x20000007 // the return valus is the character length, not the byte size
#define RIDI_DEVICEINFO 0x2000000b

typedef struct tagRID_DEVICE_INFO_MOUSE {
     DWORD dwId;
     DWORD dwNumberOfButtons;
     DWORD dwSampleRate;
} RID_DEVICE_INFO_MOUSE, *PRID_DEVICE_INFO_MOUSE;

typedef struct tagRID_DEVICE_INFO_KEYBOARD {
     DWORD dwType;
     DWORD dwSubType;
     DWORD dwKeyboardMode;
     DWORD dwNumberOfFunctionKeys;
     DWORD dwNumberOfIndicators;
     DWORD dwNumberOfKeysTotal;
} RID_DEVICE_INFO_KEYBOARD, *PRID_DEVICE_INFO_KEYBOARD;

typedef struct tagRID_DEVICE_INFO_HID {
     DWORD dwVendorId;
     DWORD dwProductId;
     DWORD dwVersionNumber;

     USHORT usUsagePage;
     USHORT usUsage;
} RID_DEVICE_INFO_HID, *PRID_DEVICE_INFO_HID;

typedef struct tagRID_DEVICE_INFO {
     DWORD cbSize;
     DWORD dwType;
     union {
          RID_DEVICE_INFO_MOUSE mouse;
          RID_DEVICE_INFO_KEYBOARD keyboard;
          RID_DEVICE_INFO_HID hid;
     };
} RID_DEVICE_INFO, *PRID_DEVICE_INFO, *LPRID_DEVICE_INFO;

typedef
UINT
(_stdcall * PGetRawInputDeviceInfoA)(
     HANDLE hDevice,
     UINT uiCommand,
     LPVOID pData,
     PUINT pcbSize
);

typedef
UINT
(_stdcall * PGetRawInputDeviceInfoW)(
     HANDLE hDevice,
     UINT uiCommand,
     LPVOID pData,
     PUINT pcbSize
);

typedef
UINT
(_stdcall * PGetRawInputBuffer)(
     PRAWINPUT pData,
     PUINT pcbSize,
     UINT cbSizeHeader
);

/*
* Raw Input request APIs
*/
typedef struct tagRAWINPUTDEVICE {
     USHORT usUsagePage; // Toplevel collection UsagePage
     USHORT usUsage; // Toplevel collection Usage
     DWORD dwFlags;
     HWND hwndTarget; // Target hwnd. NULL = follows keyboard focus
} RAWINPUTDEVICE, *PRAWINPUTDEVICE, *LPRAWINPUTDEVICE;

typedef CONST RAWINPUTDEVICE* PCRAWINPUTDEVICE;

#define RIDEV_REMOVE 0x00000001
#define RIDEV_EXCLUDE 0x00000010
#define RIDEV_PAGEONLY 0x00000020
#define RIDEV_NOLEGACY 0x00000030
#define RIDEV_INPUTSINK 0x00000100
#define RIDEV_CAPTUREMOUSE 0x00000200 // effective when mouse nolegacy is specified, otherwise it would be an error
#define RIDEV_NOHOTKEYS 0x00000200 // effective for keyboard.
#define RIDEV_APPKEYS 0x00000400 // effective for keyboard.
#define RIDEV_EXMODEMASK 0x000000F0

#define RIDEV_EXMODE(mode) ((mode) & RIDEV_EXMODEMASK)

typedef
BOOL
(_stdcall * PRegisterRawInputDevices)(
     PCRAWINPUTDEVICE pRawInputDevices,
     UINT uiNumDevices,
     UINT cbSize
);

typedef
UINT
(_stdcall * PGetRegisteredRawInputDevices)(
     PRAWINPUTDEVICE pRawInputDevices,
     PUINT puiNumDevices,
     UINT cbSize
);


typedef struct tagRAWINPUTDEVICELIST {
     HANDLE hDevice;
     DWORD dwType;
} RAWINPUTDEVICELIST, *PRAWINPUTDEVICELIST;

typedef
UINT
(_stdcall * PGetRawInputDeviceList)(
     PRAWINPUTDEVICELIST pRawInputDeviceList,
     PUINT puiNumDevices,
     UINT cbSize);

typedef
LRESULT
(_stdcall * PDefRawInputProc)(
     PRAWINPUT *paRawInput,
     INT nInput,
     UINT cbSizeHeader
);

//static PRegisterRawInputDevices RegisterRawInputDevices;
//static PGetRawInputData GetRawInputData;

#define WM_INPUT 0x00ff

#endif // RAWINPUT_H_INCLUDED

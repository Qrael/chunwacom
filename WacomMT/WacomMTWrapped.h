#ifndef WMTDLLHANDLER_C_
#define WMTDLLHANDLER_C_

#include <windows.h>
#include "WacomMTTypes.h"

typedef void (* WMT_ATTACH_CALLBACK)(WacomMTCapability deviceInfo, void *userData);
typedef void (* WMT_DETACH_CALLBACK)(int deviceID, void *userData);

typedef int (* WMT_FINGER_CALLBACK)(WacomMTFingerCollection *fingerPacket, void *userData);
typedef int (* WMT_BLOB_CALLBACK)(WacomMTBlobAggregate *blobPacket, void *userData);
typedef int (* WMT_RAW_CALLBACK)(WacomMTRawData *blobPacket, void *userData);

typedef WacomMTError   (* WACOMMTINITIALIZE)(int libraryAPIVersion);
typedef void           (* WACOMMTQUIT)(void);
typedef int            (* WACOMMTGETATTACHEDDEVICEIDS)(int *deviceArray, size_t bufferSize);
typedef WacomMTError   (* WACOMMTGETDEVICECAPABILITIES)(int deviceID, WacomMTCapability *capabilityBuffer);
typedef WacomMTError   (* WACOMMTREGISTERATTACHCALLBACK)(WMT_ATTACH_CALLBACK attachCallback, void *userData);
typedef WacomMTError   (* WACOMMTREGISTERDETACHCALLBACK)(WMT_DETACH_CALLBACK detachCallback, void *userData);
typedef WacomMTError   (* WACOMMTREGISTERFINGERREADCALLBACK)(int deviceID, WacomMTHitRect *hitRect, WacomMTProcessingMode mode, WMT_FINGER_CALLBACK fingerCallback, void *userData);
typedef WacomMTError   (* WACOMMTREGISTERBLOBREADCALLBACK)(int deviceID, WacomMTHitRect *hitRect, WacomMTProcessingMode mode, WMT_BLOB_CALLBACK blobCallback, void *userData);
typedef WacomMTError   (* WACOMMTREGISTERRAWREADCALLBACK)(int deviceID, WacomMTProcessingMode mode, WMT_RAW_CALLBACK rawCallback, void *userData);
typedef WacomMTError   (* WACOMMTUNREGISTERFINGERREADCALLBACK)(int deviceID, WacomMTHitRect *hitRect, WacomMTProcessingMode mode, void *userData);
typedef WacomMTError   (* WACOMMTUNREGISTERBLOBREADCALLBACK)(int deviceID, WacomMTHitRect *hitRect, WacomMTProcessingMode mode, void *userData);
typedef WacomMTError   (* WACOMMTUNREGISTERRAWREADCALLBACK)(int deviceID, WacomMTProcessingMode mode, void *userData);
typedef WacomMTError   (* WACOMMTMOVEREGISTEREDFINGERREADCALLBACK)(int deviceID, WacomMTHitRect *oldHitRect, WacomMTProcessingMode mode, WacomMTHitRect *newHitRect, void *userData);
typedef WacomMTError   (* WACOMMTMOVEREGISTEREDBLOBREADCALLBACK)(int deviceID, WacomMTHitRect *oldHhitRect, WacomMTProcessingMode mode, WacomMTHitRect *newHitRect, void *userData);
typedef WacomMTError   (* WACOMMTREGISTERFINGERREADHWND)(int deviceID, WacomMTProcessingMode mode, HWND hWnd, int bufferDepth);
typedef WacomMTError   (* WACOMMTREGISTERBLOBREADHWND)(int deviceID, WacomMTProcessingMode mode, HWND hWnd, int bufferDepth);
typedef WacomMTError   (* WACOMMTREGISTERRAWREADHWND)(int deviceID, WacomMTProcessingMode mode, HWND hWnd, int bufferDepth);
typedef WacomMTError   (* WACOMMTUNREGISTERFINGERREADHWND)(HWND hWnd);
typedef WacomMTError   (* WACOMMTUNREGISTERBLOBREADHWND)(HWND hWnd);

int LoadWacomMTLib(void);
void UnloadWacomMTLib(void);

//These are stubs to prevent a crash if the driver isn't loaded
int WacomMTGetAttachedDeviceIDsInternal(int *deviceArray, size_t bufferSize) {
	return 0;
}
WacomMTError WacomMTGetDeviceCapabilitiesInternal(int deviceID, WacomMTCapability *capabilityBuffer) {
	return WMTErrorQuit;
}
WacomMTError WacomMTRegisterAttachCallbackInternal(WMT_ATTACH_CALLBACK attachCallback, void *userData) {
	return WMTErrorQuit;
}
WacomMTError WacomMTRegisterDetachCallbackInternal(WMT_DETACH_CALLBACK detachCallback, void *userData) {
	return WMTErrorQuit;
}
WacomMTError WacomMTRegisterFingerReadCallbackInternal(int deviceID, WacomMTHitRect *hitRect, WacomMTProcessingMode mode, WMT_FINGER_CALLBACK fingerCallback, void *userData) {
	return WMTErrorQuit;
}
WacomMTError WacomMTRegisterBlobReadCallbackInternal(int deviceID, WacomMTHitRect *hitRect, WacomMTProcessingMode mode, WMT_BLOB_CALLBACK blobCallback, void *userData) {
	return WMTErrorQuit;
}
WacomMTError WacomMTRegisterRawReadCallbackInternal(int deviceID, WacomMTProcessingMode mode, WMT_RAW_CALLBACK rawCallback, void *userData) {
	return WMTErrorQuit;
}
WacomMTError WacomMTUnRegisterFingerReadCallbackInternal(int deviceID, WacomMTHitRect *hitRect, WacomMTProcessingMode mode, void *userData) {
	return WMTErrorQuit;
}
WacomMTError WacomMTUnRegisterBlobReadCallbackInternal(int deviceID, WacomMTHitRect *hitRect, WacomMTProcessingMode mode, void *userData) {
	return WMTErrorQuit;
}
WacomMTError WacomMTUnRegisterRawReadCallbackInternal(int deviceID, WacomMTProcessingMode mode, void *userData) {
	return WMTErrorQuit;
}
WacomMTError WacomMTMoveRegisteredFingerReadCallbackInternal(int deviceID, WacomMTHitRect *oldHitRect, WacomMTProcessingMode mode, WacomMTHitRect *newHitRect, void *userData) {
	return WMTErrorQuit;
}
WacomMTError WacomMTMoveRegisteredBlobReadCallbackInternal(int deviceID, WacomMTHitRect *oldHhitRect, WacomMTProcessingMode mode, WacomMTHitRect *newHitRect, void *userData) {
	return WMTErrorQuit;
}
WacomMTError WacomMTRegisterFingerReadHWNDInternal(int deviceID, WacomMTProcessingMode mode, HWND hWnd, int bufferDepth) {
	return WMTErrorQuit;
}
WacomMTError WacomMTRegisterBlobReadHWNDInternal(int deviceID, WacomMTProcessingMode mode, HWND hWnd, int bufferDepth) {
	return WMTErrorQuit;
}
WacomMTError WacomMTRegisterRawReadHWNDInternal(int deviceID, WacomMTProcessingMode mode, HWND hWnd, int bufferDepth) {
	return WMTErrorQuit;
}
WacomMTError WacomMTUnRegisterFingerReadHWNDInternal(HWND hWnd) {
	return WMTErrorQuit;
}
WacomMTError WacomMTUnRegisterBlobReadHWNDInternal(HWND hWnd) {
	return WMTErrorQuit;
}

HINSTANCE gpWacomMTLib = NULL;
WACOMMTINITIALIZE WacomMTInitializeInternal = NULL;
WACOMMTQUIT WacomMTQuitInternal = NULL;
WACOMMTGETATTACHEDDEVICEIDS WacomMTGetAttachedDeviceIDs = (WACOMMTGETATTACHEDDEVICEIDS)WacomMTGetAttachedDeviceIDsInternal;
WACOMMTGETDEVICECAPABILITIES WacomMTGetDeviceCapabilities = (WACOMMTGETDEVICECAPABILITIES)WacomMTGetDeviceCapabilitiesInternal;
WACOMMTREGISTERATTACHCALLBACK WacomMTRegisterAttachCallback = (WACOMMTREGISTERATTACHCALLBACK)WacomMTRegisterAttachCallbackInternal;
WACOMMTREGISTERDETACHCALLBACK WacomMTRegisterDetachCallback = (WACOMMTREGISTERDETACHCALLBACK)WacomMTRegisterDetachCallbackInternal;
WACOMMTREGISTERFINGERREADCALLBACK WacomMTRegisterFingerReadCallback = (WACOMMTREGISTERFINGERREADCALLBACK)WacomMTRegisterFingerReadCallbackInternal;
WACOMMTREGISTERBLOBREADCALLBACK WacomMTRegisterBlobReadCallback = (WACOMMTREGISTERBLOBREADCALLBACK)WacomMTRegisterBlobReadCallbackInternal;
WACOMMTREGISTERRAWREADCALLBACK WacomMTRegisterRawReadCallback = (WACOMMTREGISTERRAWREADCALLBACK)WacomMTRegisterRawReadCallbackInternal;
WACOMMTUNREGISTERFINGERREADCALLBACK WacomMTUnRegisterFingerReadCallback = (WACOMMTUNREGISTERFINGERREADCALLBACK)WacomMTUnRegisterFingerReadCallbackInternal;
WACOMMTUNREGISTERBLOBREADCALLBACK WacomMTUnRegisterBlobReadCallback = (WACOMMTUNREGISTERBLOBREADCALLBACK)WacomMTUnRegisterBlobReadCallbackInternal;
WACOMMTUNREGISTERRAWREADCALLBACK WacomMTUnRegisterRawReadCallback = (WACOMMTUNREGISTERRAWREADCALLBACK)WacomMTUnRegisterRawReadCallbackInternal;
WACOMMTMOVEREGISTEREDFINGERREADCALLBACK WacomMTMoveRegisteredFingerReadCallback = (WACOMMTMOVEREGISTEREDFINGERREADCALLBACK)WacomMTMoveRegisteredFingerReadCallbackInternal;
WACOMMTMOVEREGISTEREDBLOBREADCALLBACK WacomMTMoveRegisteredBlobReadCallback = (WACOMMTMOVEREGISTEREDBLOBREADCALLBACK)WacomMTMoveRegisteredBlobReadCallbackInternal;
WACOMMTREGISTERFINGERREADHWND WacomMTRegisterFingerReadHWND = (WACOMMTREGISTERFINGERREADHWND)WacomMTRegisterFingerReadHWNDInternal;
WACOMMTREGISTERBLOBREADHWND WacomMTRegisterBlobReadHWND = (WACOMMTREGISTERBLOBREADHWND)WacomMTRegisterBlobReadHWNDInternal;
WACOMMTREGISTERRAWREADHWND WacomMTRegisterRawReadHWND = (WACOMMTREGISTERRAWREADHWND)WacomMTRegisterRawReadHWNDInternal;
WACOMMTUNREGISTERFINGERREADHWND WacomMTUnRegisterFingerReadHWND = (WACOMMTUNREGISTERFINGERREADHWND)WacomMTUnRegisterFingerReadHWNDInternal;
WACOMMTUNREGISTERBLOBREADHWND WacomMTUnRegisterBlobReadHWND = (WACOMMTUNREGISTERBLOBREADHWND)WacomMTUnRegisterBlobReadHWNDInternal;

#define GETPROCADDRESS(type, func) \
	func = (type)GetProcAddress(gpWacomMTLib, #func); \
	if (!func){ err = GetLastError(); UnloadWacomMTLib(); return 0; }



///////////////////////////////////////////////////////////////////////////////
int LoadWacomMTLib( void )
{
	int err = 0;

	gpWacomMTLib = LoadLibrary("wacommt.dll");
	if (!gpWacomMTLib)
	{
		err = GetLastError();
		return err;
	}

	WacomMTInitializeInternal = (WACOMMTINITIALIZE)GetProcAddress( gpWacomMTLib, "WacomMTInitialize" );
	WacomMTQuitInternal = (WACOMMTQUIT)GetProcAddress( gpWacomMTLib, "WacomMTQuit" );
	if (!WacomMTInitializeInternal || !WacomMTQuitInternal)
	{
		err = GetLastError();
		UnloadWacomMTLib();
		return err;
	}

	// WacomMTGetAttachedDeviceIDs = (WACOMMTGETATTACHEDDEVICEIDS)GetProcAddress(gpWacomMTLib, "WacomMTGetAttachedDeviceIDs");

	GETPROCADDRESS( WACOMMTGETATTACHEDDEVICEIDS,             WacomMTGetAttachedDeviceIDs );
	GETPROCADDRESS( WACOMMTGETDEVICECAPABILITIES,            WacomMTGetDeviceCapabilities );
	GETPROCADDRESS( WACOMMTREGISTERATTACHCALLBACK,           WacomMTRegisterAttachCallback );
	GETPROCADDRESS( WACOMMTREGISTERDETACHCALLBACK,           WacomMTRegisterDetachCallback );
	GETPROCADDRESS( WACOMMTREGISTERFINGERREADCALLBACK,       WacomMTRegisterFingerReadCallback );
	GETPROCADDRESS( WACOMMTREGISTERBLOBREADCALLBACK,         WacomMTRegisterBlobReadCallback );
	GETPROCADDRESS( WACOMMTREGISTERRAWREADCALLBACK,          WacomMTRegisterRawReadCallback );
	GETPROCADDRESS( WACOMMTUNREGISTERFINGERREADCALLBACK,     WacomMTUnRegisterFingerReadCallback );
	GETPROCADDRESS( WACOMMTUNREGISTERBLOBREADCALLBACK,       WacomMTUnRegisterBlobReadCallback );
	GETPROCADDRESS( WACOMMTUNREGISTERRAWREADCALLBACK,        WacomMTUnRegisterRawReadCallback );
	GETPROCADDRESS( WACOMMTMOVEREGISTEREDFINGERREADCALLBACK, WacomMTMoveRegisteredFingerReadCallback );
	GETPROCADDRESS( WACOMMTMOVEREGISTEREDBLOBREADCALLBACK,   WacomMTMoveRegisteredBlobReadCallback );
	GETPROCADDRESS( WACOMMTREGISTERFINGERREADHWND,           WacomMTRegisterFingerReadHWND );
	GETPROCADDRESS( WACOMMTREGISTERBLOBREADHWND,             WacomMTRegisterBlobReadHWND );
	GETPROCADDRESS( WACOMMTREGISTERRAWREADHWND,              WacomMTRegisterRawReadHWND );
	GETPROCADDRESS( WACOMMTUNREGISTERFINGERREADHWND,         WacomMTUnRegisterFingerReadHWND );
	GETPROCADDRESS( WACOMMTUNREGISTERBLOBREADHWND,           WacomMTUnRegisterBlobReadHWND );

	return 1;
}



///////////////////////////////////////////////////////////////////////////////
void UnloadWacomMTLib( void )
{
	if (gpWacomMTLib)
	{
		FreeLibrary(gpWacomMTLib);
		gpWacomMTLib = NULL;
	}

	WacomMTInitializeInternal = NULL;
	WacomMTQuitInternal = NULL;
	WacomMTGetAttachedDeviceIDs = WacomMTGetAttachedDeviceIDsInternal;
	WacomMTGetDeviceCapabilities = WacomMTGetDeviceCapabilitiesInternal;
	WacomMTRegisterAttachCallback = WacomMTRegisterAttachCallbackInternal;
	WacomMTRegisterDetachCallback = WacomMTRegisterDetachCallbackInternal;
	WacomMTRegisterFingerReadCallback = WacomMTRegisterFingerReadCallbackInternal;
	WacomMTRegisterBlobReadCallback = WacomMTRegisterBlobReadCallbackInternal;
	WacomMTRegisterRawReadCallback = WacomMTRegisterRawReadCallbackInternal;
	WacomMTUnRegisterFingerReadCallback = WacomMTUnRegisterFingerReadCallbackInternal;
	WacomMTUnRegisterBlobReadCallback = WacomMTUnRegisterBlobReadCallbackInternal;
	WacomMTUnRegisterRawReadCallback = WacomMTUnRegisterRawReadCallbackInternal;
	WacomMTMoveRegisteredFingerReadCallback = WacomMTMoveRegisteredFingerReadCallbackInternal;
	WacomMTMoveRegisteredBlobReadCallback = WacomMTMoveRegisteredBlobReadCallbackInternal;
	WacomMTRegisterFingerReadHWND = WacomMTRegisterFingerReadHWNDInternal;
	WacomMTRegisterBlobReadHWND = WacomMTRegisterBlobReadHWNDInternal;
	WacomMTRegisterRawReadHWND = WacomMTRegisterRawReadHWNDInternal;
	WacomMTUnRegisterFingerReadHWND = WacomMTUnRegisterFingerReadHWNDInternal;
	WacomMTUnRegisterBlobReadHWND = WacomMTUnRegisterBlobReadHWNDInternal;
}



///////////////////////////////////////////////////////////////////////////////
WacomMTError WacomMTInitialize(int libraryAPIVersion)
{
	if (LoadWacomMTLib())
	{
		return WacomMTInitializeInternal(libraryAPIVersion);
	}
	return WMTErrorDriverNotFound;
}



///////////////////////////////////////////////////////////////////////////////
void WacomMTQuit(void)
{
	if (WacomMTQuitInternal)
	{
		WacomMTQuitInternal();
	}
	UnloadWacomMTLib();
}


#endif

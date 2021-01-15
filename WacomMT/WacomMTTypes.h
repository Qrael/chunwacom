///////////////////////////////////////////////////////////////////////////////
//
//	PURPOSE
//		Wacom MultiTouch API data types
//
//	COPYRIGHT
//		Copyright (c) 2012-2020 Wacom Co., Ltd.
//
//		The text and information contained in this file may be freely used,
//		copied, or distributed without compensation or licensing restrictions.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WacomMultitouchTypes_h
#define WacomMultitouchTypes_h

#ifdef __cplusplus
extern "C"
{
#endif

#define WACOM_MULTI_TOUCH_API_VERSION				4


//////////////////////////////////////////////////////////////////////////////
typedef enum WacomMTError
{
	WMTErrorSuccess				= 0,
	WMTErrorDriverNotFound		= 1,
	WMTErrorBadVersion			= 2,
	WMTErrorAPIOutdated			= 3,
	WMTErrorInvalidParam			= 4,
	WMTErrorQuit					= 5,
	WMTErrorBufferTooSmall		= 6
} WacomMTError;


typedef enum WacomMTDeviceType
{
	WMTDeviceTypeOpaque			= 0,
	WMTDeviceTypeIntegrated		= 1
} WacomMTDeviceType;


enum WacomMTCapabilityFlags
{
	WMTCapabilityFlagsRawAvailable			= (1 <<  0),
	WMTCapabilityFlagsBlobAvailable			= (1 <<  1),
	WMTCapabilityFlagsSensitivityAvailable	= (1 <<  2),
	WMTCapabilityFlagsReserved					= (1 << 31)
};
typedef int WacomMTCapabilityFlags;


typedef enum WacomMTFingerState
{
	WMTFingerStateNone			= 0,
	WMTFingerStateDown			= 1,
	WMTFingerStateHold			= 2,
	WMTFingerStateUp				= 3
} WacomMTFingerState;


typedef enum WacomMTBlobType
{
	WMTBlobTypePrimary			= 0,
	WMTBlobTypeVoid				= 1
} WacomMTBlobType;


typedef enum WacomMTProcessingMode
{
	WMTProcessingModeNone				= 0,
	WMTProcessingModeObserver			= (1 << 0),
	WMTProcessingModePassThrough		= (1 << 1),
	// bits 3-30 are reserved for future use
	WMTProcessingModeReserved			= (1 << 31)
} WacomMTProcessingMode;



//////////////////////////////////////////////////////////////////////////////
typedef struct WacomMTCapability
{
	int							Version;
	int							DeviceID;
	WacomMTDeviceType			Type;
	float							LogicalOriginX;
	float							LogicalOriginY;
	float							LogicalWidth;
	float							LogicalHeight;
	float							PhysicalSizeX;
	float							PhysicalSizeY;
	int							ReportedSizeX;
	int							ReportedSizeY;
	int							ScanSizeX;
	int							ScanSizeY;
	int							FingerMax;
	int							BlobMax;
	int							BlobPointsMax;
	WacomMTCapabilityFlags	CapabilityFlags;
} WacomMTCapability;


typedef struct WacomMTFinger
{
	int							FingerID;
	float							X;
	float							Y;
	float							Width;
	float							Height;
	unsigned short				Sensitivity;
	float							Orientation;
	int							Confidence;
	WacomMTFingerState		TouchState;
} WacomMTFinger;


typedef struct WacomMTFingerCollection
{
	int							Version;
	int							DeviceID;
	int							FrameNumber;
	int							FingerCount;
	WacomMTFinger				*Fingers;
} WacomMTFingerCollection;


typedef struct WacomMTBlobPoint
{
	float							X;
	float							Y;
	unsigned short				Sensitivity;
} WacomMTBlobPoint;


typedef struct WacomMTBlob
{
	int							BlobID;
	float							X;
	float							Y;
	int							Confidence;
	WacomMTBlobType			BlobType;
	int							ParentID;
	int							PointCount;
	WacomMTBlobPoint			*BlobPoints;
} WacomMTBlob;


typedef struct WacomMTBlobAggregate
{
	int							Version;
	int							DeviceID;
	int							FrameNumber;
	int							BlobCount;
	WacomMTBlob					*BlobArray;
} WacomMTBlobAggregate;


typedef struct WacomMTRawData
{
	int							Version;
	int							DeviceID;
	int							FrameNumber;
	int							ElementCount;
	unsigned short				*Sensitivity;
} WacomMTRawData;


typedef struct WacomMTHitRect
{
	float							originX;
	float							originY;
	float							width;
	float							height;
} WacomMTHitRect;



#ifdef __cplusplus
}
#endif

#endif /* WacomMultitouchTypes_h */

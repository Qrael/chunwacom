#include <process.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <fcntl.h>
#include <io.h>
#include "chuniio.h"
#include "leapio/leapio.h"
#include "log.h"
#include "WacomMTWrapped.h"

#define MAXFINGERS 10
#define CONFIG L".\\chunwacom.ini"

#define CSRC_TOUCH 0
#define CSRC_LEAP 1

#define LEAP_X 0
#define LEAP_Y 1
#define LEAP_Z 2

static double chuni_ir_trigger_threshold = 0.05;
static double chuni_ir_height = 0.03;
static UINT chuni_ir_leap_trigger = 500;
static UINT chuni_ir_leap_step = 300;
static uint8_t leap_orientation = LEAP_Y;
static BOOL leap_inverted = FALSE;

static int *deviceIDs;
static uint8_t ir_control_source = CSRC_TOUCH;
static bool ir_keep_slider = false;

static unsigned int __stdcall chuni_io_slider_thread_proc(void *ctx);

static bool chuni_coin_pending = true;
static uint16_t chuni_coins = 0;
static uint8_t chuni_ir_sensor_map = 0;
static HANDLE chuni_io_slider_thread;
static bool chuni_io_slider_stop_flag;
static uint8_t chuni_sliders[32];

static double start_locations[MAXFINGERS];
static LONG finger_ids[MAXFINGERS];

static int get_min(int a, int b) {
    return (a < b) ? a : b;
}

static int get_slider_from_pos(float x, float y) {
    int pos = 32 - (x*32);
    return pos==32 ? 31 : pos;
}

WacomMTError RegisterForTouch(int deviceID_I);

int FingerCallback(WacomMTFingerCollection *fingerData, void *userData);

void AttachCallback(WacomMTCapability deviceInfo, void *userRef) {
	if (deviceInfo.CapabilityFlags&(1<<2)) {
    int deviceID = deviceInfo.DeviceID;
    log_info("Device %d attached.", deviceID);
		RegisterForTouch(deviceID);
	}
}

static WacomMTError InitWacomMTAPI() {
    int loadstatus = LoadWacomMTLib();
    if (loadstatus) {
      log_info("WacomMT.dll successfully loaded.\n");
    } else {
      log_error("WacomMT.dll failed to load.");
      return WMTErrorDriverNotFound;
    }

    WacomMTError res = WacomMTInitialize(WACOM_MULTI_TOUCH_API_VERSION);
    if (res != WMTErrorSuccess) {
      log_error("Wacom Multi Touch API loading failed with code %d\n", res);
  		return res;
  	}

  	res = WacomMTRegisterAttachCallback(AttachCallback, NULL);
  	if (res != WMTErrorSuccess) {
      log_info("Attach Callback failed to register with code %d. New Wacom devices connected will not be recognized.\n", res);
  		return res;
  	}

    deviceIDs = (int *)malloc(sizeof(int));
    int deviceCount = WacomMTGetAttachedDeviceIDs(NULL, 0);
  	if (deviceCount) {
  		int newCount = 0;
  		while (newCount != deviceCount) {
  			realloc(deviceIDs, deviceCount * sizeof(int));
  			newCount = WacomMTGetAttachedDeviceIDs(deviceIDs, deviceCount * sizeof(int));
  		}

      for (size_t i = 0; i < deviceCount; i++) {
        log_info("Found Device ID %d.\n", i, *(deviceIDs+i));
        RegisterForTouch(*(deviceIDs+i));
      }
  	}

    log_info("%d Wacom device attached.\n", deviceCount);

  	return WMTErrorSuccess;
}

WacomMTError RegisterForTouch(int deviceID_I) {
	WacomMTError res = WMTErrorInvalidParam;

	res = WacomMTRegisterFingerReadCallback(deviceID_I, NULL, 0, FingerCallback, NULL);

  if (res==WMTErrorSuccess) {
    log_info("Device ID %d has been registered for touch input.\n", deviceID_I);
  }

	return res;
}

static void chuni_io_ir(uint8_t *bitmap, int8_t sensor_id, bool set) {
    if (sensor_id > 5) sensor_id = 5;
    if (sensor_id < 0) sensor_id = 0;
    if (sensor_id % 2 == 0) sensor_id++;
    else sensor_id--;
    if (set) *bitmap |= 1 << sensor_id;
    else *bitmap &= ~(1 << sensor_id);
}

int FingerCallback(WacomMTFingerCollection *fingerData, void *userData) {
  static uint8_t clicked_sliders[32];
  memset(clicked_sliders, 0, 32);
  uint8_t chuni_ir_map_local = 0;

  if (fingerData) {
    for (int i = 0; i < get_min(fingerData->FingerCount, MAXFINGERS); i++) {
      WacomMTFinger finger = fingerData->Fingers[i];
      if (finger.TouchState != WMTFingerStateNone) {
        //log_debug("Finger %d at X: %f, Y: %f\n", finger.FingerID, finger.X, finger.Y);
        if (finger.TouchState == WMTFingerStateDown) { start_locations[finger.FingerID] = finger.Y; }
        double y_diff = start_locations[finger.FingerID] - finger.Y;
        if (ir_control_source == CSRC_TOUCH && y_diff > chuni_ir_trigger_threshold) {
            int8_t ir_id = (y_diff / chuni_ir_height) - 1;
            chuni_io_ir(&chuni_ir_map_local, ir_id, true);
        }
        if (ir_control_source == CSRC_LEAP || y_diff <= chuni_ir_trigger_threshold || ir_keep_slider) {
            int slider_id = get_slider_from_pos(finger.X, finger.Y);
            //log_debug("Finger %d at X: %f, Y: %f; slider %d triggered\n", finger.FingerID, finger.X, finger.Y, slider_id);
            if (slider_id >= 0 && slider_id < 32) {
              if (finger.TouchState == WMTFingerStateUp) { clicked_sliders[slider_id] = 0; }
              else { clicked_sliders[slider_id] = 128; }
            }
        }
      }
    }
	}
  memcpy(chuni_sliders, clicked_sliders, 32);
  // for (int loop = 0; loop < 32; loop++) { printf("%d ", chuni_sliders[loop]); }
  // printf("\n");
  chuni_ir_sensor_map = chuni_ir_map_local;

	return 0;
}

static int get_finger_index(DWORD id) {
    int avail_indx = -1;
    for (int i = 0; i < MAXFINGERS; i++) {
        if (finger_ids[i] > 0 && (DWORD) finger_ids[i] == id) return i;
        if (avail_indx == -1 && finger_ids[i] == -1) avail_indx = i;
    }

    if (avail_indx == -1) return -1;
    finger_ids[avail_indx] = id;
    return avail_indx;
}

void leap_handler(const LEAP_TRACKING_EVENT *ev) {
    uint8_t chuni_ir_map_local = 0;

    for(uint32_t h = 0; h < ev->nHands; h++) {
        const LEAP_HAND* hand = &(ev->pHands[h]);
        float pos = 0;
        if (leap_orientation == LEAP_X) pos = hand->palm.position.x;
        if (leap_orientation == LEAP_Y) pos = hand->palm.position.y;
        if (leap_orientation == LEAP_Z) pos = hand->palm.position.z;

        if ((!leap_inverted && pos > chuni_ir_leap_trigger) || (leap_inverted && chuni_ir_leap_trigger > pos)) {
            int8_t ir_id = (leap_inverted ? -1 : 1) * (pos - chuni_ir_leap_trigger) / chuni_ir_leap_step + 1;
            if (ir_id > 5) ir_id = 5;
            if (ir_id < 0) ir_id = 0;
            chuni_io_ir(&chuni_ir_map_local, ir_id, true);
        }
    }

    chuni_ir_sensor_map = chuni_ir_map_local;
}

HRESULT chuni_io_jvs_init(void) {
    // alloc console for debug output
    AllocConsole();
    SetConsoleTitle("chunwacom");
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    log_info("allocated debug console.\n");

    WCHAR str_control_src[16];
    WCHAR str_leap_orientation[16];

    int8_t temp = GetPrivateProfileIntW(L"ir", L"touch_height", "30", CONFIG);
    chuni_ir_height = ((double)temp)/1000;
    temp = GetPrivateProfileIntW(L"ir", L"touch_trigger", "50", CONFIG);
    chuni_ir_trigger_threshold = ((double)temp)/1000;
    chuni_ir_leap_trigger = GetPrivateProfileIntW(L"ir", L"leap_trigger", 50, CONFIG);
    chuni_ir_leap_step = GetPrivateProfileIntW(L"ir", L"leap_step", 30, CONFIG);
    ir_keep_slider = GetPrivateProfileIntW(L"misc", L"ir_keep_slider", 0, CONFIG);

    GetPrivateProfileStringW(L"ir", L"control_source", L"touch", str_control_src, 16, CONFIG);
    GetPrivateProfileStringW(L"ir", L"leap_orientation", L"y", str_leap_orientation, 16, CONFIG);

    ir_control_source = (wcscmp(str_control_src, L"leap") == 0) ? CSRC_LEAP : CSRC_TOUCH;
    /**/ if (wcscmp(str_leap_orientation, L"x") == 0) leap_orientation = LEAP_X;
    else if (wcscmp(str_leap_orientation, L"y") == 0) leap_orientation = LEAP_Y;
    else if (wcscmp(str_leap_orientation, L"z") == 0) leap_orientation = LEAP_Z;
    else if (wcscmp(str_leap_orientation, L"-x") == 0) { leap_orientation = LEAP_X; leap_inverted = TRUE; }
    else if (wcscmp(str_leap_orientation, L"-y") == 0) { leap_orientation = LEAP_Y; leap_inverted = TRUE; }
    else if (wcscmp(str_leap_orientation, L"-z") == 0) { leap_orientation = LEAP_Z; leap_inverted = TRUE; }

    if (InitWacomMTAPI() != WMTErrorSuccess) {
      log_error("Could not initialize Wacom Multi-Touch API.\n");
    }

    for(int i = 0; i < MAXFINGERS; i++) finger_ids[i] = -1;

    if (ir_control_source == CSRC_LEAP) {
        log_info("connecting to leap service...\n");
        leap_connect(NULL);
        leap_set_tracking_handler(leap_handler);
        while (!leap_is_connected()) {
            Sleep(10);
        }
        log_info("connected to leap service.\n");
    }


    log_info("ir_keep_slider: %s\n", ir_keep_slider ? "enabled" : "disabled");

    if (ir_control_source == CSRC_TOUCH) {
        log_info("ir: touch mode, trigger_threshold: %lf, height: %lf\n", chuni_ir_trigger_threshold, chuni_ir_height);
    } else {
        log_info("ir: leap mode, axis: %u, trigger_threshold: %u, step: %u\n", leap_orientation, chuni_ir_leap_trigger, chuni_ir_leap_step);
    }

    return S_OK;
}

void chuni_io_jvs_read_coin_counter(uint16_t *out) {
    if (out == NULL) {
        return;
    }

    if (GetAsyncKeyState(VK_F3)) {
        if (chuni_coin_pending) {
            chuni_coins++;
            chuni_coin_pending = false;
        }
    } else chuni_coin_pending = true;

    *out = chuni_coins;
}

void chuni_io_jvs_poll(uint8_t *opbtn, uint8_t *beams) {
    *beams = chuni_ir_sensor_map;
}

void chuni_io_jvs_set_coin_blocker(bool open) {
    if (open) log_info("coin blocker disabled");
    else log_info("coin blocker enabled.");

}

HRESULT chuni_io_slider_init(void) {
    log_info("init slider...\n");
    return S_OK;
}

void chuni_io_slider_start(chuni_io_slider_callback_t callback) {
    log_info("starting slider...\n");
    if (chuni_io_slider_thread != NULL) {
        return;
    }

    chuni_io_slider_thread = (HANDLE)_beginthreadex(
        NULL,
        0,
        chuni_io_slider_thread_proc,
        callback,
        0,
        NULL);
}

void chuni_io_slider_stop(void) {
    log_info("stopping slider...\n");
    if (chuni_io_slider_thread == NULL) {
        return;
    }

    chuni_io_slider_stop_flag = true;

    WaitForSingleObject(chuni_io_slider_thread, INFINITE);
    CloseHandle(chuni_io_slider_thread);
    chuni_io_slider_thread = NULL;
    chuni_io_slider_stop_flag = false;
}

void chuni_io_slider_set_leds() {
}

static unsigned int __stdcall chuni_io_slider_thread_proc(void *ctx) {
    chuni_io_slider_callback_t callback;
    uint8_t pressure[32];
    size_t i;

    for (size_t i = 0; i < _countof(chuni_sliders); i++) chuni_sliders[i] = 0;

    callback = ctx;

    while (!chuni_io_slider_stop_flag) {
        callback(chuni_sliders);
        Sleep(1);
    }

    return 0;
}

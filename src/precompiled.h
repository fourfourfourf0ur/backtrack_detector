#pragma once

#ifdef _WIN32  // WINDOWS
#pragma warning(disable : 4005)
#else
#ifdef __FUNCTION__
#undef __FUNCTION__
#endif
#define __FUNCTION__ __func__
#endif  // _WIN32

#define OBS_NONE 0
#define MAX_PATH 260

#include <vector>
#include <deque>
#include <thread>
#include <memory>
#include <cstring>
#include <chrono>
#include <extdll.h>
#include <cbase.h>
#include <stdio.h>

#include <eiface.h>
#include <meta_api.h>

#include "sdk_util.h"

#include <regamedll_api.h>
#include <mapinfo.h>
#include <studio.h>
#include <r_studioint.h>

#include <rehlds_api.h>
#include "ex_rehlds_api.h"

#include "backtrack_detector.h"

#include "struct.h"
#undef DLLEXPORT
#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#define NOINLINE __declspec(noinline)
#else
#define DLLEXPORT __attribute__((visibility("default")))
#define NOINLINE __attribute__((noinline))
#define WINAPI /* */
#endif

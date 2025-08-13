#include "precompiled.h"

IRehldsApi* g_RehldsApi;
const RehldsFuncs_t* g_RehldsFuncs;
IRehldsHookchains* g_RehldsHookchains;
IRehldsServerStatic* g_RehldsSvs;

bool rehlds_api_init(CSysModule* engineModule) {
  if (!engineModule) {
    gpMetaUtilFuncs->pfnLogConsole(PLID, "Failed to locate engine module\n");
    return false;
  }

  CreateInterfaceFn ifaceFactory = Sys_GetFactory(engineModule);
  if (!ifaceFactory) {
    gpMetaUtilFuncs->pfnLogConsole(
        PLID, "Failed to locate interface factory in engine module\n");
    return false;
  }

  int retCode = 0;
  g_RehldsApi = (IRehldsApi*)ifaceFactory(VREHLDS_HLDS_API_VERSION, &retCode);
  if (!g_RehldsApi) {
    return false;
  }

  int majorVersion = g_RehldsApi->GetMajorVersion();
  int minorVersion = g_RehldsApi->GetMinorVersion();

  if (majorVersion != REHLDS_API_VERSION_MAJOR) {
    gpMetaUtilFuncs->pfnLogConsole(
        PLID, "REHLDS Api major version mismatch; expected %d, real %d\n",
        REHLDS_API_VERSION_MAJOR, majorVersion);
    return false;
  }

  if (minorVersion < REHLDS_API_VERSION_MINOR) {
    gpMetaUtilFuncs->pfnLogConsole(
        PLID,
        "REHLDS Api minor version mismatch; expected at least %d, real %d\n",
        REHLDS_API_VERSION_MINOR, minorVersion);
    return false;
  }

  g_RehldsFuncs = g_RehldsApi->GetFuncs();
  g_RehldsHookchains = g_RehldsApi->GetHookchains();
  g_RehldsSvs = g_RehldsApi->GetServerStatic();

  return true;
}

bool meta_init_rehlds_api() {
#ifdef WIN32
  CSysModule* engineModule = Sys_LoadModule("swds.dll");
  if (!rehlds_api_init(engineModule)) {
    engineModule = Sys_LoadModule("filesystem_stdio.dll");
    if (!rehlds_api_init(engineModule)) return false;
  }
#else
  CSysModule* engineModule = Sys_LoadModule("engine_i486.so");
  if (!rehlds_api_init(engineModule)) return false;
#endif

  return true;
}

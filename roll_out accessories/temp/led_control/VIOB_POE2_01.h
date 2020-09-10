// VIOB-POE2-01 API V1.1 2020/02/06
#define VIOB_POE2_01_API extern "C" __declspec(dllimport)


VIOB_POE2_01_API void PoE_Get_DLL_Version(int *pnMajorVersion, int *pnMinorVersion);

VIOB_POE2_01_API int  PoE_Connect();
VIOB_POE2_01_API void PoE_Disconnect();

VIOB_POE2_01_API int  PoE_GetMCUVersion(int *pnMCUVersion);
VIOB_POE2_01_API int  PoE_GetHWVersion(int *pnHWVersion);
VIOB_POE2_01_API int  PoE_GetDTPVersion(int *pnDTPVersion);
VIOB_POE2_01_API int  PoE_GetBLVersion(int *pnBLVersion);
VIOB_POE2_01_API int  PoE_GetSystemTemperature(int *pnSystemTemperature);

VIOB_POE2_01_API int  PoE_GetPSEFWVersion(int *pnPSEFWVersion);
VIOB_POE2_01_API int  PoE_GetPSETemperature(int *pnPSETemperature);
VIOB_POE2_01_API int  PoE_GetTotalConsumption(int *pnTotalConsumption);
VIOB_POE2_01_API int  PoE_GetPowerBudget(int *pnPowerBudget);
VIOB_POE2_01_API int  PoE_GetPort1Status(int *pnPort1Status);
VIOB_POE2_01_API int  PoE_GetPort2Status(int *pnPort2Status);
VIOB_POE2_01_API int  PoE_GetPort1PowerConsumption(int *pnPort1PowerConsumption);
VIOB_POE2_01_API int  PoE_GetPort2PowerConsumption(int *pnPort2PowerConsumption);

VIOB_POE2_01_API int  PoE_SetPortPowerControl(int nIndex, int nPower);

// Event
typedef void (*PoE_EventSystemAlarm_CallBack)(unsigned char byAlarm);
VIOB_POE2_01_API void PoE_RegisterSystemAlarmEvent(PoE_EventSystemAlarm_CallBack fnESA);

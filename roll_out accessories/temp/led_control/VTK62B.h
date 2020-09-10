// VTK62B API V1.7 2018/08/10
#define VTK62B_API extern "C" __declspec(dllimport)


VTK62B_API void VTK_Get_DLL_Version(int *pnMajorVersion, int *pnMinorVersion);

VTK62B_API int  VTK_Connect(int nSerialPortNum);
VTK62B_API void VTK_Disconnect();

VTK62B_API int  VTK_GetMCUVersion(int *pnMCUVersion);
VTK62B_API int  VTK_GetHWVersion(int *pnPCB);
VTK62B_API int  VTK_GetDTPVersion(int *pnDTPVersion);
VTK62B_API int  VTK_GetBLVersion(int *pnBLVersion);
VTK62B_API int  VTK_GetIgnitionStatus(int *pnIgnitionStatus);
VTK62B_API int  VTK_GetSupplyVoltage(int *pnSupplyVoltage);
VTK62B_API int  VTK_GetMDOStatus(int *pnMDO1, int *pnMDO2);
VTK62B_API int  VTK_GetPowerType(int *pnPowerType);
VTK62B_API int  VTK_GetPowerRange(int *pnPowerRange);

VTK62B_API int  VTK_SetFlashUpdate();
VTK62B_API int  VTK_SetMDOControl(int nMDO1, int nMDO2);

VTK62B_API int  VTK_GetBackupBatteryStatus(int *pnBatteryStatus);
VTK62B_API int  VTK_GetSmartBatteryVoltage(int *pnBatteryVoltage);
VTK62B_API int  VTK_GetSmartBatteryTemperature(int *pnBatteryTemperature);
VTK62B_API int  VTK_GetSmartBatteryStatus(int *pnBatteryStatus);
VTK62B_API int  VTK_GetSmartBatteryCapacity(int *pnBatteryCapacity);
VTK62B_API int  VTK_GetSmartBatteryCurrent(int *pnBatteryCurrent);
VTK62B_API int  VTK_GetThresholdSetting(int *pnThreshold_Low, int *pnThreshold_High);
VTK62B_API int  VTK_GetDelayTimeStatus(int *pnStatus);
VTK62B_API int  VTK_GetDelayTimeNow(int *pnDelayTime);

VTK62B_API int  VTK_SetDelayTimeControl(int nTime);

typedef void (__stdcall *VTK_BackupBatteryAlarm_CallBack)(unsigned char byAlarm);

VTK62B_API void VTK_RegisterBatteryCallBackFunction(
	VTK_BackupBatteryAlarm_CallBack  fnBBA);

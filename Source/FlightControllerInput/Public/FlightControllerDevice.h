// Copyright © 2025 丁钰清 (YUQING DING), All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "IInputDevice.h"
#include "GenericPlatform/GenericApplicationMessageHandler.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "GenericPlatform/IInputInterface.h"
#include "InputCoreTypes.h"

#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include <windows.h>
#include <hidsdi.h>
#include <hidpi.h>
#include <setupapi.h>
#include "Windows/HideWindowsPlatformTypes.h"
#endif

/**
 * 飞行遥控器输入设备
 */
class FFlightControllerDevice : public IInputDevice
{
public:
	FFlightControllerDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler);
	virtual ~FFlightControllerDevice();

	/** IInputDevice interface */
	virtual void Tick(float DeltaTime) override;
	virtual void SendControllerEvents() override;
	virtual void SetMessageHandler(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler) override;
	virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override;
	virtual void SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value) override;
	virtual void SetChannelValues(int32 ControllerId, const FForceFeedbackValues& values) override;

private:
	/** 初始化 HID 设备 */
	bool InitializeDevice();
	
	/** 关闭设备 */
	void CloseDevice();
	
	/** 读取 HID 数据 */
	bool ReadInputData();
	
	/** 查找飞行遥控器设备 */
	bool FindFlightController();

	/** 消息处理器 */
	TSharedRef<FGenericApplicationMessageHandler> MessageHandler;

#if PLATFORM_WINDOWS
	/** HID 设备句柄 */
	HANDLE DeviceHandle;
	
	/** HID 输入缓冲区 */
	TArray<uint8> InputBuffer;
	
	/** Preparsed Data */
	PHIDP_PREPARSED_DATA PreparsedData;
	
	/** 设备能力 */
	HIDP_CAPS Capabilities;
	
	/** 按钮能力数组 */
	TArray<HIDP_BUTTON_CAPS> ButtonCaps;
	
	/** 数值能力数组 */
	TArray<HIDP_VALUE_CAPS> ValueCaps;
#endif

	/** 上一帧的输入状态 */
	struct FControllerState
	{
		TMap<int32, bool> ButtonStates;      // 拨杆等按钮状态
		TMap<int32, float> AxisValues;        // 摇杆等模拟轴值
	} CurrentState, PreviousState;

	/** 设备是否已初始化 */
	bool bIsInitialized;
	
	/** VID 和 PID (可配置) */
	uint16 VendorID;
	uint16 ProductID;
	
	/** Platform User ID 和 Device ID */
	FPlatformUserId PlatformUserId;
	FInputDeviceId InputDeviceId;
};

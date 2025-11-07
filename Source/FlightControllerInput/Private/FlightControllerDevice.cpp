// Copyright © 2025 丁钰清 (YUQING DING), All rights reserved.
#include "FlightControllerDevice.h"
#include "FlightControllerKeys.h"
#include "Engine/Engine.h"
#include "Misc/ConfigCacheIni.h"

DEFINE_LOG_CATEGORY_STATIC(LogFlightController, Log, All);

FFlightControllerDevice::FFlightControllerDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
	: MessageHandler(InMessageHandler)
	, DeviceHandle(INVALID_HANDLE_VALUE)
	, PreparsedData(nullptr)
	, bIsInitialized(false)
	, VendorID(0x0000)  // 设置你的遥控器 VID
	, ProductID(0x0000) // 设置你的遥控器 PID
	, PlatformUserId(FPlatformUserId::CreateFromInternalId(0))
	, InputDeviceId(FInputDeviceId::CreateFromInternalId(0))
{
	// 从配置文件读取 VID/PID（可选）
	FString ConfigSection = TEXT("/Script/FlightControllerInput.FlightControllerSettings");
	int32 ConfigVID = 0, ConfigPID = 0;
	
	if (GConfig->GetInt(*ConfigSection, TEXT("VendorID"), ConfigVID, GEngineIni))
	{
		VendorID = static_cast<uint16>(ConfigVID);
	}
	
	if (GConfig->GetInt(*ConfigSection, TEXT("ProductID"), ConfigPID, GEngineIni))
	{
		ProductID = static_cast<uint16>(ConfigPID);
	}

	InitializeDevice();
}

FFlightControllerDevice::~FFlightControllerDevice()
{
	CloseDevice();
}

bool FFlightControllerDevice::InitializeDevice()
{
#if PLATFORM_WINDOWS
	if (bIsInitialized)
	{
		return true;
	}

	UE_LOG(LogFlightController, Log, TEXT("Initializing Flight Controller Device..."));

	// 查找设备
	if (!FindFlightController())
	{
		UE_LOG(LogFlightController, Warning, TEXT("Flight Controller not found"));
		return false;
	}

	// 获取 Preparsed Data
	if (!HidD_GetPreparsedData(DeviceHandle, &PreparsedData))
	{
		UE_LOG(LogFlightController, Error, TEXT("Failed to get preparsed data"));
		CloseDevice();
		return false;
	}

	// 获取设备能力
	NTSTATUS status = HidP_GetCaps(PreparsedData, &Capabilities);
	if (status != HIDP_STATUS_SUCCESS)
	{
		UE_LOG(LogFlightController, Error, TEXT("Failed to get device capabilities"));
		CloseDevice();
		return false;
	}

	UE_LOG(LogFlightController, Log, TEXT("Device Capabilities:"));
	UE_LOG(LogFlightController, Log, TEXT("  Usage Page: 0x%04X"), Capabilities.UsagePage);
	UE_LOG(LogFlightController, Log, TEXT("  Usage: 0x%04X"), Capabilities.Usage);
	UE_LOG(LogFlightController, Log, TEXT("  Input Report Length: %d"), Capabilities.InputReportByteLength);
	UE_LOG(LogFlightController, Log, TEXT("  Number of Buttons: %d"), Capabilities.NumberInputButtonCaps);
	UE_LOG(LogFlightController, Log, TEXT("  Number of Values: %d"), Capabilities.NumberInputValueCaps);

	// 分配输入缓冲区
	InputBuffer.SetNum(Capabilities.InputReportByteLength);

	// 获取按钮能力
	if (Capabilities.NumberInputButtonCaps > 0)
	{
		ButtonCaps.SetNum(Capabilities.NumberInputButtonCaps);
		USHORT capsLength = Capabilities.NumberInputButtonCaps;
		HidP_GetButtonCaps(HidP_Input, ButtonCaps.GetData(), &capsLength, PreparsedData);
		
		for (int32 i = 0; i < ButtonCaps.Num(); i++)
		{
			UE_LOG(LogFlightController, Log, TEXT("  Button Cap %d: UsagePage=0x%04X, UsageMin=%d, UsageMax=%d, DataIndexMin=%d"),
				i, ButtonCaps[i].UsagePage, ButtonCaps[i].Range.UsageMin, 
				ButtonCaps[i].Range.UsageMax, ButtonCaps[i].Range.DataIndexMin);
		}
	}

	// 获取数值能力（摇杆等）
	if (Capabilities.NumberInputValueCaps > 0)
	{
		ValueCaps.SetNum(Capabilities.NumberInputValueCaps);
		USHORT capsLength = Capabilities.NumberInputValueCaps;
		HidP_GetValueCaps(HidP_Input, ValueCaps.GetData(), &capsLength, PreparsedData);
		
		for (int32 i = 0; i < ValueCaps.Num(); i++)
		{
			UE_LOG(LogFlightController, Log, TEXT("  Value Cap %d: UsagePage=0x%04X, Usage=%d, LogicalMin=%d, LogicalMax=%d"),
				i, ValueCaps[i].UsagePage, ValueCaps[i].Range.UsageMin,
				ValueCaps[i].LogicalMin, ValueCaps[i].LogicalMax);
		}
	}

	bIsInitialized = true;
	UE_LOG(LogFlightController, Log, TEXT("Flight Controller initialized successfully"));
	return true;

#else
	UE_LOG(LogFlightController, Error, TEXT("Flight Controller only supported on Windows"));
	return false;
#endif
}

void FFlightControllerDevice::CloseDevice()
{
#if PLATFORM_WINDOWS
	if (PreparsedData)
	{
		HidD_FreePreparsedData(PreparsedData);
		PreparsedData = nullptr;
	}

	if (DeviceHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(DeviceHandle);
		DeviceHandle = INVALID_HANDLE_VALUE;
	}

	bIsInitialized = false;
	UE_LOG(LogFlightController, Log, TEXT("Flight Controller device closed"));
#endif
}

bool FFlightControllerDevice::FindFlightController()
{
#if PLATFORM_WINDOWS
	GUID hidGuid;
	HidD_GetHidGuid(&hidGuid);

	// 获取所有 HID 设备
	HDEVINFO deviceInfoSet = SetupDiGetClassDevs(&hidGuid, nullptr, nullptr, 
		DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

	if (deviceInfoSet == INVALID_HANDLE_VALUE)
	{
		UE_LOG(LogFlightController, Error, TEXT("Failed to get device info set"));
		return false;
	}

	SP_DEVICE_INTERFACE_DATA deviceInterfaceData;
	deviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

	// 遍历所有 HID 设备
	for (DWORD i = 0; SetupDiEnumDeviceInterfaces(deviceInfoSet, nullptr, &hidGuid, i, &deviceInterfaceData); i++)
	{
		// 获取设备路径大小
		DWORD requiredSize = 0;
		SetupDiGetDeviceInterfaceDetail(deviceInfoSet, &deviceInterfaceData, nullptr, 0, &requiredSize, nullptr);

		// 分配内存
		PSP_DEVICE_INTERFACE_DETAIL_DATA deviceInterfaceDetailData = 
			(PSP_DEVICE_INTERFACE_DETAIL_DATA)FMemory::Malloc(requiredSize);
		deviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

		// 获取设备路径
		if (SetupDiGetDeviceInterfaceDetail(deviceInfoSet, &deviceInterfaceData, 
			deviceInterfaceDetailData, requiredSize, nullptr, nullptr))
		{
			// 打开设备
			HANDLE testHandle = CreateFile(deviceInterfaceDetailData->DevicePath,
				GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_READ | FILE_SHARE_WRITE,
				nullptr,
				OPEN_EXISTING,
				0,
				nullptr);

			if (testHandle != INVALID_HANDLE_VALUE)
			{
				HIDD_ATTRIBUTES attributes;
				attributes.Size = sizeof(HIDD_ATTRIBUTES);

				// 获取设备属性
				if (HidD_GetAttributes(testHandle, &attributes))
				{
					UE_LOG(LogFlightController, Verbose, TEXT("Found HID Device: VID=0x%04X, PID=0x%04X"), 
						attributes.VendorID, attributes.ProductID);

					// 如果 VID/PID 为 0，接受第一个游戏控制器类型设备
					// 否则匹配指定的 VID/PID
					bool isMatch = false;
					
					if (VendorID == 0 && ProductID == 0)
					{
						// 检查是否是游戏控制器类型设备
						PHIDP_PREPARSED_DATA tempPreparsedData;
						if (HidD_GetPreparsedData(testHandle, &tempPreparsedData))
						{
							HIDP_CAPS tempCaps;
							if (HidP_GetCaps(tempPreparsedData, &tempCaps) == HIDP_STATUS_SUCCESS)
							{
								// 通用游戏控制器: UsagePage=0x01 (Generic Desktop), Usage=0x04 (Joystick) 或 0x05 (Game Pad)
								if (tempCaps.UsagePage == 0x01 && (tempCaps.Usage == 0x04 || tempCaps.Usage == 0x05))
								{
									isMatch = true;
									VendorID = attributes.VendorID;
									ProductID = attributes.ProductID;
									UE_LOG(LogFlightController, Log, TEXT("Auto-detected controller: VID=0x%04X, PID=0x%04X"), 
										VendorID, ProductID);
								}
							}
							HidD_FreePreparsedData(tempPreparsedData);
						}
					}
					else
					{
						isMatch = (attributes.VendorID == VendorID && attributes.ProductID == ProductID);
					}

					if (isMatch)
					{
						DeviceHandle = testHandle;
						FMemory::Free(deviceInterfaceDetailData);
						SetupDiDestroyDeviceInfoList(deviceInfoSet);
						
						UE_LOG(LogFlightController, Log, TEXT("Flight Controller found and opened: VID=0x%04X, PID=0x%04X"), 
							VendorID, ProductID);
						return true;
					}
				}

				CloseHandle(testHandle);
			}
		}

		FMemory::Free(deviceInterfaceDetailData);
	}

	SetupDiDestroyDeviceInfoList(deviceInfoSet);
	UE_LOG(LogFlightController, Warning, TEXT("Flight Controller not found (VID=0x%04X, PID=0x%04X)"), 
		VendorID, ProductID);
	return false;

#else
	return false;
#endif
}

bool FFlightControllerDevice::ReadInputData()
{
#if PLATFORM_WINDOWS
	if (!bIsInitialized || DeviceHandle == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	// 读取输入报告
	DWORD bytesRead = 0;
	if (!ReadFile(DeviceHandle, InputBuffer.GetData(), InputBuffer.Num(), &bytesRead, nullptr))
	{
		DWORD error = GetLastError();
		if (error != ERROR_IO_PENDING)
		{
			UE_LOG(LogFlightController, Warning, TEXT("Failed to read from device, error: %d"), error);
			return false;
		}
	}

	// 保存上一帧状态
	PreviousState = CurrentState;

	// 解析按钮状态（拨杆等）
	for (int32 i = 0; i < ButtonCaps.Num(); i++)
	{
		ULONG usageLength = ButtonCaps[i].Range.UsageMax - ButtonCaps[i].Range.UsageMin + 1;
		TArray<USAGE> usages;
		usages.SetNum(usageLength);

		// 先将所有按钮状态设为 false
		for (USAGE usage = ButtonCaps[i].Range.UsageMin; usage <= ButtonCaps[i].Range.UsageMax; usage++)
		{
			int32 buttonIndex = usage - ButtonCaps[i].Range.UsageMin;
			CurrentState.ButtonStates.FindOrAdd(buttonIndex) = false;
		}

		// 读取实际按下的按钮
		NTSTATUS status = HidP_GetUsages(HidP_Input, ButtonCaps[i].UsagePage, 0,
			usages.GetData(), &usageLength, PreparsedData, 
			(PCHAR)InputBuffer.GetData(), InputBuffer.Num());

		if (status == HIDP_STATUS_SUCCESS)
		{
			// usageLength 现在是实际按下的按钮数量
			UE_LOG(LogFlightController, Verbose, TEXT("📊 Button Cap %d: Found %d pressed buttons"), i, usageLength);
			
			// 设置按下的按钮为 true
			for (ULONG j = 0; j < usageLength; j++)
			{
				int32 buttonIndex = usages[j] - ButtonCaps[i].Range.UsageMin;
				CurrentState.ButtonStates.FindOrAdd(buttonIndex) = true;
				UE_LOG(LogFlightController, Verbose, TEXT("  Button %d (Usage %d) is pressed"), buttonIndex, usages[j]);
			}
		}
		else
		{
			UE_LOG(LogFlightController, Warning, TEXT("Failed to get button usages, status: 0x%08X"), status);
		}
	}

	// 解析数值（摇杆等）
	for (int32 i = 0; i < ValueCaps.Num(); i++)
	{
		ULONG value = 0;
		NTSTATUS status = HidP_GetUsageValue(HidP_Input, ValueCaps[i].UsagePage,
			0, ValueCaps[i].Range.UsageMin, &value, PreparsedData,
			(PCHAR)InputBuffer.GetData(), InputBuffer.Num());

		if (status == HIDP_STATUS_SUCCESS)
		{
			// 归一化到 -1.0 到 1.0
			float normalizedValue = 0.0f;
			LONG logicalMin = ValueCaps[i].LogicalMin;
			LONG logicalMax = ValueCaps[i].LogicalMax;
			
			if (logicalMax != logicalMin)
			{
				normalizedValue = ((float)(LONG)value - logicalMin) / (logicalMax - logicalMin) * 2.0f - 1.0f;
			}

			CurrentState.AxisValues.FindOrAdd(i) = normalizedValue;
			
			UE_LOG(LogFlightController, Verbose, TEXT("📊 Axis %d: Raw=%d, Normalized=%.3f (Min=%d, Max=%d)"), 
				i, value, normalizedValue, logicalMin, logicalMax);
		}
		else
		{
			UE_LOG(LogFlightController, Warning, TEXT("Failed to get axis %d value, status: 0x%08X"), i, status);
		}
	}

	return true;

#else
	return false;
#endif
}

void FFlightControllerDevice::Tick(float DeltaTime)
{
	if (!bIsInitialized)
	{
		return;
	}

	// 读取新数据
	if (ReadInputData())
	{
		// 发送控制器事件
		SendControllerEvents();
	}
}

void FFlightControllerDevice::SendControllerEvents()
{
	if (!bIsInitialized)
	{
		return;
	}

	// 使用存储的有效 PlatformUserId 和 InputDeviceId
	// 而不是每次都尝试获取可能无效的 ID

	// 按钮键映射表
	static const FKey* ButtonKeys[] = {
		&FFlightControllerKeys::FlightController_Button0,
		&FFlightControllerKeys::FlightController_Button1,
		&FFlightControllerKeys::FlightController_Button2,
		&FFlightControllerKeys::FlightController_Button3,
		&FFlightControllerKeys::FlightController_Button4,
		&FFlightControllerKeys::FlightController_Button5,
		&FFlightControllerKeys::FlightController_Button6,
		&FFlightControllerKeys::FlightController_Button7,
		&FFlightControllerKeys::FlightController_Button8,
		&FFlightControllerKeys::FlightController_Button9,
		&FFlightControllerKeys::FlightController_Button10,
		&FFlightControllerKeys::FlightController_Button11,
		&FFlightControllerKeys::FlightController_Button12,
		&FFlightControllerKeys::FlightController_Button13,
		&FFlightControllerKeys::FlightController_Button14,
		&FFlightControllerKeys::FlightController_Button15
	};

	// 轴键映射表
	static const FKey* AxisKeys[] = {
		&FFlightControllerKeys::FlightController_Axis0,
		&FFlightControllerKeys::FlightController_Axis1,
		&FFlightControllerKeys::FlightController_Axis2,
		&FFlightControllerKeys::FlightController_Axis3,
		&FFlightControllerKeys::FlightController_Axis4,
		&FFlightControllerKeys::FlightController_Axis5,
		&FFlightControllerKeys::FlightController_Axis6,
		&FFlightControllerKeys::FlightController_Axis7
	};

	// 发送按钮事件（拨杆等）
	for (const auto& ButtonPair : CurrentState.ButtonStates)
	{
		int32 ButtonIndex = ButtonPair.Key;
		bool bIsPressed = ButtonPair.Value;
		bool bWasPressed = PreviousState.ButtonStates.FindRef(ButtonIndex);

		if (bIsPressed != bWasPressed)
		{
			// 使用预定义的 FKey
			if (ButtonIndex >= 0 && ButtonIndex < 16)
			{
				const FKey& ButtonKey = *ButtonKeys[ButtonIndex];
				
				if (bIsPressed)
				{
					MessageHandler->OnControllerButtonPressed(ButtonKey.GetFName(), PlatformUserId, InputDeviceId, false);
					UE_LOG(LogFlightController, Log, TEXT("🎮 Button %d Pressed"), ButtonIndex);
				}
				else
				{
					MessageHandler->OnControllerButtonReleased(ButtonKey.GetFName(), PlatformUserId, InputDeviceId, false);
					UE_LOG(LogFlightController, Log, TEXT("🎮 Button %d Released"), ButtonIndex);
				}
			}
		}
	}

	// 发送轴事件（摇杆等）
	for (const auto& AxisPair : CurrentState.AxisValues)
	{
		int32 AxisIndex = AxisPair.Key;
		float AxisValue = AxisPair.Value;
		float PreviousValue = PreviousState.AxisValues.FindRef(AxisIndex);

		// 死区处理
		const float DeadZone = 0.1f;
		if (FMath::Abs(AxisValue) < DeadZone)
		{
			AxisValue = 0.0f;
		}

		if (AxisValue != PreviousValue)
		{
			// 使用预定义的 FKey
			if (AxisIndex >= 0 && AxisIndex < 8)
			{
				const FKey& AxisKey = *AxisKeys[AxisIndex];
				MessageHandler->OnControllerAnalog(AxisKey.GetFName(), PlatformUserId, InputDeviceId, AxisValue);
				
				// 只在有意义的变化时输出日志
				// 1. 变化幅度要足够大（>0.15）
				// 2. 或者值从0变为非0，或从非0变为0（表示开始/停止移动）
				float DeltaValue = FMath::Abs(AxisValue - PreviousValue);
				bool bSignificantChange = DeltaValue > 0.15f;
				bool bStartedMoving = (FMath::Abs(PreviousValue) < 0.01f && FMath::Abs(AxisValue) > 0.01f);
				bool bStoppedMoving = (FMath::Abs(PreviousValue) > 0.01f && FMath::Abs(AxisValue) < 0.01f);
				
				if (bSignificantChange || bStartedMoving || bStoppedMoving)
				{
					UE_LOG(LogFlightController, Log, TEXT("🕹️ Axis %d: %.3f"), AxisIndex, AxisValue);
				}
			}
		}
	}
}

void FFlightControllerDevice::SetMessageHandler(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
{
	MessageHandler = InMessageHandler;
}

bool FFlightControllerDevice::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	// 可以添加调试命令
	return false;
}

void FFlightControllerDevice::SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value)
{
	// 力反馈不支持
}

void FFlightControllerDevice::SetChannelValues(int32 ControllerId, const FForceFeedbackValues& values)
{
	// 力反馈不支持
}
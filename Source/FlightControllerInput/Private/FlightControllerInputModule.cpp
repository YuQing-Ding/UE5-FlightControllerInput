// Copyright © 2025 丁钰清 (YUQING DING), All rights reserved.
#include "FlightControllerInputModule.h"
#include "FlightControllerDevice.h"
#include "FlightControllerKeys.h"
#include "IInputDeviceModule.h"

#define LOCTEXT_NAMESPACE "FFlightControllerInputModule"

void FFlightControllerInputModule::StartupModule()
{
	// 注册自定义输入键
	FFlightControllerKeys::Initialize();
	
	// 注册为输入设备模块
	IModularFeatures::Get().RegisterModularFeature(IInputDeviceModule::GetModularFeatureName(), this);
	
	UE_LOG(LogTemp, Log, TEXT("Flight Controller Input Plugin Started"));
}

void FFlightControllerInputModule::ShutdownModule()
{
	// 注销模块特性
	IModularFeatures::Get().UnregisterModularFeature(IInputDeviceModule::GetModularFeatureName(), this);
	
	UE_LOG(LogTemp, Log, TEXT("Flight Controller Input Plugin Shutdown"));
}

TSharedPtr<class IInputDevice> FFlightControllerInputModule::CreateInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
{
	if (!InputDevice.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Creating Flight Controller Input Device"));
		InputDevice = MakeShareable(new FFlightControllerDevice(InMessageHandler));
	}
	
	return InputDevice;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFlightControllerInputModule, FlightControllerInput)

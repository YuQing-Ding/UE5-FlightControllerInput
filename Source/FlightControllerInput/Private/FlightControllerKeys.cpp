// Copyright © 2025 丁钰清 (YUQING DING), All rights reserved.
#include "FlightControllerKeys.h"

// 定义轴键
const FKey FFlightControllerKeys::FlightController_Axis0(TEXT("FlightController_Axis0"));
const FKey FFlightControllerKeys::FlightController_Axis1(TEXT("FlightController_Axis1"));
const FKey FFlightControllerKeys::FlightController_Axis2(TEXT("FlightController_Axis2"));
const FKey FFlightControllerKeys::FlightController_Axis3(TEXT("FlightController_Axis3"));
const FKey FFlightControllerKeys::FlightController_Axis4(TEXT("FlightController_Axis4"));
const FKey FFlightControllerKeys::FlightController_Axis5(TEXT("FlightController_Axis5"));
const FKey FFlightControllerKeys::FlightController_Axis6(TEXT("FlightController_Axis6"));
const FKey FFlightControllerKeys::FlightController_Axis7(TEXT("FlightController_Axis7"));

// 定义按钮键
const FKey FFlightControllerKeys::FlightController_Button0(TEXT("FlightController_Button0"));
const FKey FFlightControllerKeys::FlightController_Button1(TEXT("FlightController_Button1"));
const FKey FFlightControllerKeys::FlightController_Button2(TEXT("FlightController_Button2"));
const FKey FFlightControllerKeys::FlightController_Button3(TEXT("FlightController_Button3"));
const FKey FFlightControllerKeys::FlightController_Button4(TEXT("FlightController_Button4"));
const FKey FFlightControllerKeys::FlightController_Button5(TEXT("FlightController_Button5"));
const FKey FFlightControllerKeys::FlightController_Button6(TEXT("FlightController_Button6"));
const FKey FFlightControllerKeys::FlightController_Button7(TEXT("FlightController_Button7"));
const FKey FFlightControllerKeys::FlightController_Button8(TEXT("FlightController_Button8"));
const FKey FFlightControllerKeys::FlightController_Button9(TEXT("FlightController_Button9"));
const FKey FFlightControllerKeys::FlightController_Button10(TEXT("FlightController_Button10"));
const FKey FFlightControllerKeys::FlightController_Button11(TEXT("FlightController_Button11"));
const FKey FFlightControllerKeys::FlightController_Button12(TEXT("FlightController_Button12"));
const FKey FFlightControllerKeys::FlightController_Button13(TEXT("FlightController_Button13"));
const FKey FFlightControllerKeys::FlightController_Button14(TEXT("FlightController_Button14"));
const FKey FFlightControllerKeys::FlightController_Button15(TEXT("FlightController_Button15"));

void FFlightControllerKeys::Initialize()
{
	// 注册轴键 - UE 5.6 兼容方式
	// 使用 Axis1D 标志告诉 UE 这是一个轴输入
	EKeys::AddKey(FKeyDetails(FlightController_Axis0, NSLOCTEXT("FlightControllerKeys", "FlightController_Axis0", "Flight Controller Axis 0"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FlightController_Axis1, NSLOCTEXT("FlightControllerKeys", "FlightController_Axis1", "Flight Controller Axis 1"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FlightController_Axis2, NSLOCTEXT("FlightControllerKeys", "FlightController_Axis2", "Flight Controller Axis 2"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FlightController_Axis3, NSLOCTEXT("FlightControllerKeys", "FlightController_Axis3", "Flight Controller Axis 3"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FlightController_Axis4, NSLOCTEXT("FlightControllerKeys", "FlightController_Axis4", "Flight Controller Axis 4"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FlightController_Axis5, NSLOCTEXT("FlightControllerKeys", "FlightController_Axis5", "Flight Controller Axis 5"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FlightController_Axis6, NSLOCTEXT("FlightControllerKeys", "FlightController_Axis6", "Flight Controller Axis 6"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FlightController_Axis7, NSLOCTEXT("FlightControllerKeys", "FlightController_Axis7", "Flight Controller Axis 7"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));

	// 注册按钮键
	EKeys::AddKey(FKeyDetails(FlightController_Button0, NSLOCTEXT("FlightControllerKeys", "FlightController_Button0", "Flight Controller Button 0"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(FlightController_Button1, NSLOCTEXT("FlightControllerKeys", "FlightController_Button1", "Flight Controller Button 1"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(FlightController_Button2, NSLOCTEXT("FlightControllerKeys", "FlightController_Button2", "Flight Controller Button 2"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(FlightController_Button3, NSLOCTEXT("FlightControllerKeys", "FlightController_Button3", "Flight Controller Button 3"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(FlightController_Button4, NSLOCTEXT("FlightControllerKeys", "FlightController_Button4", "Flight Controller Button 4"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(FlightController_Button5, NSLOCTEXT("FlightControllerKeys", "FlightController_Button5", "Flight Controller Button 5"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(FlightController_Button6, NSLOCTEXT("FlightControllerKeys", "FlightController_Button6", "Flight Controller Button 6"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(FlightController_Button7, NSLOCTEXT("FlightControllerKeys", "FlightController_Button7", "Flight Controller Button 7"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(FlightController_Button8, NSLOCTEXT("FlightControllerKeys", "FlightController_Button8", "Flight Controller Button 8"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(FlightController_Button9, NSLOCTEXT("FlightControllerKeys", "FlightController_Button9", "Flight Controller Button 9"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(FlightController_Button10, NSLOCTEXT("FlightControllerKeys", "FlightController_Button10", "Flight Controller Button 10"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(FlightController_Button11, NSLOCTEXT("FlightControllerKeys", "FlightController_Button11", "Flight Controller Button 11"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(FlightController_Button12, NSLOCTEXT("FlightControllerKeys", "FlightController_Button12", "Flight Controller Button 12"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(FlightController_Button13, NSLOCTEXT("FlightControllerKeys", "FlightController_Button13", "Flight Controller Button 13"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(FlightController_Button14, NSLOCTEXT("FlightControllerKeys", "FlightController_Button14", "Flight Controller Button 14"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(FlightController_Button15, NSLOCTEXT("FlightControllerKeys", "FlightController_Button15", "Flight Controller Button 15"), FKeyDetails::GamepadKey));

	UE_LOG(LogTemp, Log, TEXT("FlightControllerKeys: Registered all input keys (Axes with Axis1D flag)"));
}
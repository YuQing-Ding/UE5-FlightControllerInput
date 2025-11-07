// Copyright © 2025 丁钰清 (YUQING DING), All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "InputCoreTypes.h"

/**
 * 飞行遥控器输入键定义
 * 在模块启动时注册自定义键到 UE 输入系统
 */
struct FFlightControllerKeys
{
	static const FKey FlightController_Axis0;
	static const FKey FlightController_Axis1;
	static const FKey FlightController_Axis2;
	static const FKey FlightController_Axis3;
	static const FKey FlightController_Axis4;
	static const FKey FlightController_Axis5;
	static const FKey FlightController_Axis6;
	static const FKey FlightController_Axis7;

	static const FKey FlightController_Button0;
	static const FKey FlightController_Button1;
	static const FKey FlightController_Button2;
	static const FKey FlightController_Button3;
	static const FKey FlightController_Button4;
	static const FKey FlightController_Button5;
	static const FKey FlightController_Button6;
	static const FKey FlightController_Button7;
	static const FKey FlightController_Button8;
	static const FKey FlightController_Button9;
	static const FKey FlightController_Button10;
	static const FKey FlightController_Button11;
	static const FKey FlightController_Button12;
	static const FKey FlightController_Button13;
	static const FKey FlightController_Button14;
	static const FKey FlightController_Button15;

	/** 初始化并注册所有键 */
	static void Initialize();
};

"""
Copyright © 2025 丁钰清 (YUQING DING), All rights reserved.
HID 设备查找工具 (修正版)
用于查找连接的飞行遥控器的 VID 和 PID
"""

try:
    import pywinusb.hid as hid
    import sys

    print("=" * 60)
    print("HID 设备扫描工具 (修正版)")
    print("=" * 60)
    print()

    all_devices = hid.HidDeviceFilter().get_devices()

    if not all_devices:
        print("未找到任何 HID 设备！")
        sys.exit(1)

    print(f"找到 {len(all_devices)} 个 HID 设备：\n")

    controller_devices = []

    for i, device in enumerate(all_devices):
        print(f"设备 #{i+1}:")
        print(f"  名称: {device.product_name}")
        print(f"  厂商: {device.vendor_name}")
        print(f"  VID: 0x{device.vendor_id:04X} (十进制: {device.vendor_id})")
        print(f"  PID: 0x{device.product_id:04X} (十进制: {device.product_id})")

        usage_page = None
        usage = None

        # 尝试打开设备以读取 usage 信息
        try:
            device.open()
            caps = device.capabilities
            usage_page = caps.usage_page
            usage = caps.usage
        except Exception:
            pass
        finally:
            device.close()

        if usage_page is not None and usage is not None:
            print(f"  使用页: 0x{usage_page:04X}")
            print(f"  使用: 0x{usage:04X}")
        else:
            print(f"  使用页/使用: (无法读取)")

        # 判断是否为飞行遥控器或游戏控制器
        if usage_page == 0x01 and usage in [0x04, 0x05, 0x08]:
            print("  *** 这可能是你的飞行遥控器！***")
            controller_devices.append(device)

        print()

    if controller_devices:
        print("=" * 60)
        print("检测到的游戏控制器设备：")
        print("=" * 60)
        for device in controller_devices:
            print(f"\n设备: {device.product_name}")
            print(f"配置信息（复制到 DefaultEngine.ini）：")
            print(f"VendorID={device.vendor_id}")
            print(f"ProductID={device.product_id}")
    else:
        print("未检测到游戏控制器类型的设备。")

    print("=" * 60)
    print("提示：")
    print("- 将上面的 VendorID 和 ProductID 添加到 DefaultEngine.ini")
    print("- 或者保持 VendorID=0 和 ProductID=0 使用自动检测")
    print("=" * 60)

except ImportError:
    print("错误：未安装 pywinusb 库")
    print("请运行: pip install pywinusb")
except Exception as e:
    print(f"发生错误: {e}")
    import traceback
    traceback.print_exc()
    sys.exit(1)

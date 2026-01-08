# Settings

## Settings Local 

其实就是继承了 GameUserSettings 的类，实现一些设置属性。

处理一些重写的函数。范围这几类

### Frontend state

前端状态 会对帧数的限制值有影响，这里只是一个bool，似乎是用引擎的帧数限制还是UI上玩家改的帧数限制。

### Performance stats

stat统计是否开启

### Brightness/Gamma

DisplayGamma 的 set,get,apply

### Display

FrameRateLimit_OnBattery
FrameRateLimit_InMenu
FrameRateLimit_WhenBackgrounded
应该是不同状态下的帧数限制

### Display - Mobile quality settings

MobileFrameRateLimit 移动帧率限制
DeviceDefaultScalabilitySettings 移动设备默认的可伸缩性设置
bSettingOverallQualityGuard 是否启用整体质量保护
DesiredMobileFrameRateLimit 期望的移动帧率限制

### Display - Console quality presets

DesiredUserChosenDeviceProfileSuffix
CurrentAppliedDeviceProfileOverrideSuffix
UserChosenDeviceProfileSuffix

### Audio - Volume

OnAudioOutputDeviceChanged 音频输出设备更改事件

headphoneMode 设置耳机模式相关的两个变量
bDesiredHeadphoneMode
bUseHeadphoneMode

bUseHDRAudioMode 是否使用HDR音频模式

AudioOutputDeviceId 音频输出设备ID

### Safezone

SafeZoneScale 设置安全区比例

SetVolumeForControlBus 函数，设置控制总线的音量

### Keybindings

SetControllerPlatform 输入设置的平台设置。

### Replays

bShouldAutoRecordReplays
NumberOfReplaysToKeep
是否自动录制回放，保留的回放数量

## 多了什么文件

Audio/
    LochAudioSettings
    LochAudioMixEffectsSubsystem
Performance/
    LochPerformanceSettings
    LochPerformanceStatTypes


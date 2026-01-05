# GameInstance

主要是这里有个加密的流程，去断点看看

- HandlerUserInitialized 点击搜索玩家的时候会到这里

UCommonUserSubsystem::HandleUserInitializeSucceeded
ULyraGameInstance::HandlerUserInitialized

- 第二个玩家加入房间的时候

UCommonSessionSubsystem::ConnectToHostReservationBeacon
UCommonSessionSubsystem::InternalTravelToSession
ULyraGameInstance::OnPreClientTravelToSession
这里的URL是 IP:7777

其他的比较简单，直接能跑。这个加密没跑进来。


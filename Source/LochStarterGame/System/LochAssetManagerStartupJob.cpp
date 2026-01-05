#include "LochAssetManagerStartupJob.h"

#include "LochLogChannels.h"

TSharedPtr<FStreamableHandle> FLochAssetManagerStartupJob::DoJob() const
{
	const double JobStartTime = FPlatformTime::Seconds();

	TSharedPtr<FStreamableHandle> Handle;
	UE_LOG(LogLoch, Display, TEXT("Startup job \"%s\" starting"), *JobName);
	JobFunc(*this, Handle); // 执行具体的任务，感觉只是包了一层能有个加载进度，实际没什么用？

	if (Handle.IsValid())
	{
		Handle->BindUpdateDelegate(FStreamableUpdateDelegate::CreateRaw(this, &FLochAssetManagerStartupJob::UpdateSubstepProgressFromStreamable));
		Handle->WaitUntilComplete(0.0f, false);
		Handle->BindUpdateDelegate(FStreamableUpdateDelegate());
	}

	UE_LOG(LogLoch, Display, TEXT("Startup job \"%s\" took %.2f seconds to complete"), *JobName, FPlatformTime::Seconds() - JobStartTime);

	return Handle;
}

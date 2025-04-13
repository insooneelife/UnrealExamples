#include "ConvTask.h"


FConvTask::FConvTask()
    : TaskState(ETaskState::None)
{
}

void FConvTask::StartTask()
{
    if (TaskState == ETaskState::None)
    {
        SetTaskState(ETaskState::Progressing);
        OnTaskStarted();
    }
}

void FConvTask::StopTask()
{
    if (TaskState == ETaskState::Progressing)
    {
        SetTaskState(ETaskState::None);
        OnTaskStopped();
    }
}

bool FConvTask::Tick(float DeltaTime)
{
    // Return true only if the task is still in progress
    return TaskState == ETaskState::Progressing;
}



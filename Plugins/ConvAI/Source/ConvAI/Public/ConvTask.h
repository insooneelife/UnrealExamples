#pragma once

#include "CoreMinimal.h"



// FDAITask is a base class representing tasks that an AI agent can perform.
// Concrete tasks can be implemented by inheriting from this class.
class CONVAI_API FConvTask
{
public:
	// Enumeration representing the state of a task.
	enum ETaskState
	{
		None,       // Initial state
		Progressing,// Task in progress
		Completed,  // Task completed
		Failed      // Task failed
	};

public:
    FConvTask();



    // Starts the task
    virtual void StartTask();

    // Stops the task
    virtual void StopTask();

    // Returns the current state of the task
    ETaskState GetTaskState() const { return TaskState; }

    // Returns whether the task is completed
    bool IsTaskCompleted() const { return TaskState == ETaskState::Completed; }

    // Returns whether the task has failed
    bool IsTaskFailed() const { return TaskState == ETaskState::Failed; }

    // Returns whether the task is in progress
    bool IsTaskProgressing() const { return TaskState == ETaskState::Progressing; }

    // Updates the task state
    // Returns true if the task is still in progress, false otherwise
    virtual bool Tick(float DeltaTime);

protected:
    // Called when the task starts
    virtual void OnTaskStarted(){}

    // Called when the task stops
    virtual void OnTaskStopped(){}

    // Called when the task completes
    virtual void OnTaskCompleted(){}

    // Called when the task fails
    virtual void OnTaskFailed(const FString& ErrorMessage){}

    // Sets the task state
    void SetTaskState(ETaskState NewState) {TaskState = NewState;}

private:
    // Current state of the task
    ETaskState TaskState;
}; 
// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.


#include "DialogueRunner.h"

#include "DialogueAsset.h"
#include "DialogueNode.h"
#include "DialogueExtensions.h"

#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

#define LOCTEXT_NAMESPACE "DialogueRunner"

void UDialogueRunner::StartDialogue(UDialogueAsset* Asset, APlayerController* Player, AActor* Speaker)
{
	// Reset any state from a previous run so restarting is always clean.
	PendingTargets.Reset();
	CurrentNode = nullptr;

	if (!Asset || !Asset->EntryNode)
	{
		Finish();
		return;
	}

	CurrentAsset = Asset;

	Context = NewObject<UDialogueExecutionContext>(this);
	Context->Runner = this;
	Context->Player = Player;
	Context->Speaker = Speaker;

	EnterNode(Asset->EntryNode);
}

void UDialogueRunner::EnterNode(UDialogueNode* Node)
{
	CurrentNode = Node;
	if (!Node)
	{
		Finish();
		return;
	}

	// OnEnterEffects is an instanced array and may contain null entries (e.g. a slot left empty).
	for (UDialogueEffect* Effect : Node->OnEnterEffects)
	{
		if (Effect)
		{
			Effect->Execute(Context);
		}
	}

	PresentCurrent();
}

void UDialogueRunner::PresentCurrent()
{
	PendingTargets.Reset();
	TArray<FDialogueOption> Options;

	FText Speaker;
	FText Text;

	if (const UDialogueNode_Line* Line = Cast<UDialogueNode_Line>(CurrentNode))
	{
		// A line always offers a single "Continue" that walks its one output (null == end).
		Speaker = Line->SpeakerName;
		Text = Line->Text;

		UDialogueNode* Next = Line->Outputs.Num() > 0 ? Line->Outputs[0].TargetNode : nullptr;

		FDialogueOption Option;
		Option.Text = LOCTEXT("Continue", "Continue");
		Option.Index = 0;
		Options.Add(Option);

		PendingTargets.Add(Next);
	}
	else if (const UDialogueNode_Choice* Choice = Cast<UDialogueNode_Choice>(CurrentNode))
	{
		// A choice offers one option per transition whose condition passes.
		for (const FDialogueTransition& Transition : Choice->Outputs)
		{
			const bool bAvailable = !Transition.Condition || Transition.Condition->Evaluate(Context);
			if (!bAvailable)
			{
				continue;
			}

			FDialogueOption Option;
			Option.Text = Transition.Label;
			Option.Index = PendingTargets.Num();
			Options.Add(Option);
			PendingTargets.Add(Transition.TargetNode);
		}
	}

	// No presentable options (unknown node type, or every choice gated off) — nothing left to do.
	if (Options.Num() == 0)
	{
		Finish();
		return;
	}

	OnDialogueNode.Broadcast(Speaker, Text, Options);
}

void UDialogueRunner::SelectOption(int32 OptionIndex)
{
	if (PendingTargets.IsValidIndex(OptionIndex))
	{
		EnterNode(PendingTargets[OptionIndex]);
	}
}

void UDialogueRunner::Finish()
{
	CurrentNode = nullptr;
	CurrentAsset = nullptr;
	PendingTargets.Reset();
	OnDialogueEnded.Broadcast();
}

#undef LOCTEXT_NAMESPACE

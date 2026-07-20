// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.

#pragma once

#include "EdGraph/EdGraph.h"
#include "DialogueGraph.generated.h"

class UDialogueAsset;

UCLASS()
class SIMPLEDIALOGUEEDITOR_API UDialogueGraph : public UEdGraph
{
	GENERATED_BODY()
public:
	UDialogueAsset* GetDialogueAsset() const;  
};

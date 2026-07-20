// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.

#pragma once

#include "SGraphNode.h"

class UDialogueGraphNode;

class SDialogueGraphNode : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SDialogueGraphNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UDialogueGraphNode* InNode);
	
	virtual void UpdateGraphNode() override;

protected:
	TSharedRef<SWidget> CreateContentPreview() const;
	FText GetTitleText() const;
};

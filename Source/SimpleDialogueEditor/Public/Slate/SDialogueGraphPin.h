// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.

#pragma once

#include "KismetPins/SGraphPinExec.h"

class SDialogueGraphPin : public SGraphPinExec
{
public:
	SLATE_BEGIN_ARGS(SDialogueGraphPin) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InPin);
};
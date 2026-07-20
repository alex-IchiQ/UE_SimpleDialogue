// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.

#pragma once

#include "UObject/NameTypes.h"

namespace DialogueGraph
{
	/**
	 * Pin category shared by every dialogue graph node (Entry, Line, Choice, Jump).
	 * All transition pins use this single category — that is exactly what lets any output
	 * connect to any input. Keep it in one place so the categories can never drift apart.
	 */
	inline const FName TransitionPinCategory(TEXT("Transition"));
}

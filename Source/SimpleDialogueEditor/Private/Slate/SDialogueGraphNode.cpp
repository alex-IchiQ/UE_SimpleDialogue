// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.


#include "Slate/SDialogueGraphNode.h"

#include "Graph/DialogueGraphNode.h"
#include "DialogueNode.h"

#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Styling/AppStyle.h"
#include "Styling/CoreStyle.h"

#define LOCTEXT_NAMESPACE "SDialogueGraphNode"

void SDialogueGraphNode::Construct(const FArguments& InArgs, UDialogueGraphNode* InNode)
{
	GraphNode = InNode;
	SetCursor(EMouseCursor::CardinalCross);
	UpdateGraphNode();
}

void SDialogueGraphNode::UpdateGraphNode()
{
	InputPins.Empty();
	OutputPins.Empty();
	RightNodeBox.Reset();
	LeftNodeBox.Reset();

	const FLinearColor TitleColor = GraphNode->GetNodeTitleColor();
	ContentScale.Bind(this, &SGraphNode::GetContentScale);

	GetOrAddSlot(ENodeZone::Center)
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("Graph.Node.Body"))
		.Padding(0.0f)
		[
			SNew(SVerticalBox)

			// --- Title bar ---
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SBorder)
				.BorderImage(FAppStyle::GetBrush("Graph.Node.TitleGloss"))
				.BorderBackgroundColor(TitleColor)
				.Padding(FMargin(12.0f, 6.0f))
				[
					SNew(STextBlock)
					.Text(this, &SDialogueGraphNode::GetTitleText)
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 11))
					.ColorAndOpacity(FLinearColor::White)
				]
			]

			// --- Body: input pins | content | output pins ---
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				[
					SAssignNew(LeftNodeBox, SVerticalBox)
				]

				+ SHorizontalBox::Slot()
				.FillWidth(1.0f)
				.Padding(12.0f, 8.0f)
				.VAlign(VAlign_Center)
				[
					CreateContentPreview()
				]

				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				[
					SAssignNew(RightNodeBox, SVerticalBox)
				]
			]
		]
	];

	CreatePinWidgets();
}

TSharedRef<SWidget> SDialogueGraphNode::CreateContentPreview() const
{
	const UDialogueGraphNode* DialogueNode = Cast<UDialogueGraphNode>(GraphNode);
	UDialogueNode* Instance = DialogueNode ? DialogueNode->GetInstance() : nullptr;

	const TSharedRef<SVerticalBox> Box = SNew(SVerticalBox);

	if (const UDialogueNode_Line* Line = Cast<UDialogueNode_Line>(Instance))
	{
		if (!Line->SpeakerName.IsEmpty())
		{
			Box->AddSlot().AutoHeight().Padding(0, 0, 0, 2)
			[
				SNew(STextBlock)
				.Text(Line->SpeakerName)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 9))
				.ColorAndOpacity(FLinearColor(0.9f, 0.9f, 0.6f))
			];
		}

		Box->AddSlot().AutoHeight()
		[
			SNew(STextBlock)
			.Text(Line->Text.IsEmpty() ? LOCTEXT("EmptyLine", "<empty line>") : Line->Text)
			.Font(FCoreStyle::GetDefaultFontStyle("Italic", 9))
			.ColorAndOpacity(FLinearColor(0.85f, 0.85f, 0.85f))
			.AutoWrapText(true)
		];
	}
	else if (Cast<UDialogueNode_Choice>(Instance))
	{
		Box->AddSlot().AutoHeight()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("ChoicePreview", "Player Choice"))
			.Font(FCoreStyle::GetDefaultFontStyle("Italic", 9))
			.ColorAndOpacity(FLinearColor(0.8f, 0.85f, 1.0f))
		];
	}
	else
	{
		Box->AddSlot().AutoHeight()
		[
			SNew(STextBlock).Text(LOCTEXT("Unknown", "..."))
		];
	}

	return SNew(SBox).WidthOverride(240.0f)[ Box ];
}

FText SDialogueGraphNode::GetTitleText() const
{
	return GraphNode ? GraphNode->GetNodeTitle(ENodeTitleType::ListView) : FText::GetEmpty();
}


#undef LOCTEXT_NAMESPACE
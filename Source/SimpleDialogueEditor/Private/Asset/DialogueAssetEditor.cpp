// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.


#include "Asset/DialogueAssetEditor.h"

#include "DialogueAsset.h"
#include "DialogueNode.h"
#include "Graph/DialogueGraph.h"
#include "Graph/DialogueGraphNode.h"
#include "Graph/DialogueGraphSchema.h"
#include "Compile/DialogueCompiler.h"

#include "PropertyEditorModule.h"
#include "GraphEditor.h"
#include "IDetailsView.h"
#include "ScopedTransaction.h"

#include "Modules/ModuleManager.h"
#include "Widgets/Docking/SDockTab.h"
#include "Framework/Docking/TabManager.h"
#include "Framework/Docking/WorkspaceItem.h"
#include "Framework/Commands/GenericCommands.h"
#include "Framework/Commands/UICommandList.h"


#define LOCTEXT_NAMESPACE "DialogueAssetEditor"

const FName FDialogueAssetEditor::GraphTabId(TEXT("DialogueAssetEditor_Graph"));
const FName FDialogueAssetEditor::DetailsTabId(TEXT("DialogueAssetEditor_Details"));

void FDialogueAssetEditor::InitEditor(EToolkitMode::Type Mode, TSharedPtr<IToolkitHost> InitToolkitHost, UDialogueAsset* Asset)
{
	DialogueAsset = Asset;
	EnsureGraphExists();

	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("DialogueAssetEditor_Layout_v1")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Horizontal)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.7f)
				->AddTab(GraphTabId, ETabState::OpenedTab)
			)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.3f)
				->AddTab(DetailsTabId, ETabState::OpenedTab)
			)
		);

	constexpr bool bCreateDefaultStandaloneMenu = true;
	constexpr bool bCreateDefaultToolbar = true;
	
	InitAssetEditor(Mode, InitToolkitHost, TEXT("DialogueAssetEditorApp"), Layout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, DialogueAsset);
}

void FDialogueAssetEditor::EnsureGraphExists()
{
	if (!DialogueAsset->Graph)
	{
		UDialogueGraph* NewGraph = NewObject<UDialogueGraph>(DialogueAsset, UDialogueGraph::StaticClass(), NAME_None, RF_Transactional);

		NewGraph->Schema = UDialogueGraphSchema::StaticClass();
		NewGraph->GetSchema()->CreateDefaultNodesForGraph(*NewGraph); // no-op until step 4b (entry node)

		DialogueAsset->Graph = NewGraph;
	}
}

void FDialogueAssetEditor::DeleteSelectedNodes()
{
	if (!GraphEditor.IsValid())
	{
		return;
	}

	const FScopedTransaction Transaction(LOCTEXT("DeleteNodes", "Delete Nodes"));
	GraphEditor->GetCurrentGraph()->Modify();
	DialogueAsset->Modify();

	const FGraphPanelSelectionSet Selected = GraphEditor->GetSelectedNodes();
	GraphEditor->ClearSelectionSet();

	for (UObject* Object : Selected)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(Object);
		if (!Node || !Node->CanUserDeleteNode())
		{
			continue;
		}

		if (const UDialogueGraphNode* DialogueNode = Cast<UDialogueGraphNode>(Node))
		{
			DialogueAsset->Nodes.Remove(DialogueNode->GetInstance());
		}

		Node->Modify();
		Node->DestroyNode();
	}
}

bool FDialogueAssetEditor::CanDeleteNodes() const
{
	if (!GraphEditor.IsValid())
	{
		return false;
	}

	for (UObject* Object : GraphEditor->GetSelectedNodes())
	{
		if (const UEdGraphNode* Node = Cast<UEdGraphNode>(Object))
		{
			if (Node->CanUserDeleteNode())
			{
				return true;
			}
		}
	}
	
	return false;
}

void FDialogueAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	const TSharedRef<FWorkspaceItem> Category = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu", "Dialogue Editor"));

	InTabManager->RegisterTabSpawner(GraphTabId, FOnSpawnTab::CreateSP(this, &FDialogueAssetEditor::SpawnTab_Graph))
		.SetDisplayName(LOCTEXT("GraphTabTitle", "Graph"))
		.SetGroup(Category);

	InTabManager->RegisterTabSpawner(DetailsTabId, FOnSpawnTab::CreateSP(this, &FDialogueAssetEditor::SpawnTab_Details))
		.SetDisplayName(LOCTEXT("DetailsTabTitle", "Details"))
		.SetGroup(Category);
}

void FDialogueAssetEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
	
	InTabManager->UnregisterTabSpawner(GraphTabId);
	InTabManager->UnregisterTabSpawner(DetailsTabId);
}

void FDialogueAssetEditor::SaveAsset_Execute()
{
	FDialogueCompiler::Compile(DialogueAsset);
	FAssetEditorToolkit::SaveAsset_Execute();
}

TSharedRef<SDockTab> FDialogueAssetEditor::SpawnTab_Graph(const FSpawnTabArgs& SpawnTabArgs)
{
	if (!GraphEditor.IsValid())
	{
		GraphEditor = BuildGraphEditor();
	}

	return SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		[
			GraphEditor.ToSharedRef()
		];
}

TSharedRef<SDockTab> FDialogueAssetEditor::SpawnTab_Details(const FSpawnTabArgs& SpawnTabArgs)
{
	if (!DetailsView.IsValid())
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

		FDetailsViewArgs Args;
		Args.bHideSelectionTip = true;
		Args.NameAreaSettings = FDetailsViewArgs::HideNameArea;

		DetailsView = PropertyModule.CreateDetailView(Args);
		
		DetailsView->OnFinishedChangingProperties().AddSP(this, &FDialogueAssetEditor::OnDetailsPropertyChanged);
	}

	return SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		[
			DetailsView.ToSharedRef()
		];
}

TSharedRef<SGraphEditor> FDialogueAssetEditor::BuildGraphEditor()
{
	SGraphEditor::FGraphEditorEvents Events;
	Events.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FDialogueAssetEditor::OnGraphSelectionChanged);

	FGraphAppearanceInfo Appearance;
	Appearance.CornerText = LOCTEXT("GraphCornerText", "DIALOGUE");

	if (!GraphEditorCommands.IsValid())
	{
		GraphEditorCommands = MakeShared<FUICommandList>();
		GraphEditorCommands->MapAction(
			FGenericCommands::Get().Delete,
			FExecuteAction::CreateSP(this, &FDialogueAssetEditor::DeleteSelectedNodes),
			FCanExecuteAction::CreateSP(this, &FDialogueAssetEditor::CanDeleteNodes));
	}
	
	return SNew(SGraphEditor)
		.IsEditable(true)
		.Appearance(Appearance)
		.GraphToEdit(DialogueAsset->Graph)
		.GraphEvents(Events)
		.AdditionalCommands(GraphEditorCommands);
}

void FDialogueAssetEditor::OnGraphSelectionChanged(const FGraphPanelSelectionSet& NewSelection)
{
	UObject* ToShow = nullptr;
	
	if (NewSelection.Num() == 1)
	{
		UObject* Selected = *NewSelection.CreateConstIterator();

		if (const UDialogueGraphNode* Node = Cast<UDialogueGraphNode>(Selected))
		{
			ToShow = Node->GetInstance();
		}
		else if (UEdGraphNode* EdNode = Cast<UEdGraphNode>(Selected))
		{
			ToShow = EdNode;
		}
	}

	if (DetailsView.IsValid())
	{
		DetailsView->SetObject(ToShow);
	}
}

void FDialogueAssetEditor::OnDetailsPropertyChanged(const FPropertyChangedEvent& Event)
{
	if (!GraphEditor.IsValid())
	{
		return;
	}

	for (UObject* Object : GraphEditor->GetSelectedNodes())
	{
		if (UDialogueGraphNode* Node = Cast<UDialogueGraphNode>(Object))
		{
			Node->ReconstructNode();
		}
	}

	GraphEditor->NotifyGraphChanged();
}

FName FDialogueAssetEditor::GetToolkitFName() const
{
	return FName(TEXT("DialogueAssetEditor"));
}

FText FDialogueAssetEditor::GetBaseToolkitName() const
{
	return LOCTEXT("AppLabel", "Dialogue Editor");
}

FString FDialogueAssetEditor::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("TabPrefix", "Dialogue ").ToString();
}

FLinearColor FDialogueAssetEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.15f, 0.35f, 0.75f, 0.5f);
}

#undef LOCTEXT_NAMESPACE

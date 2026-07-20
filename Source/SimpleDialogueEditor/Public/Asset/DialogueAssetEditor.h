// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.

#pragma once

#include "Toolkits/AssetEditorToolkit.h"

class UDialogueAsset;
class SGraphEditor;
class SGraphPanel;
class SDockTab;
class IDetailsView;
class FSpawnTabArgs;
class FUICommandList;

struct FPropertyChangedEvent;

class SIMPLEDIALOGUEEDITOR_API FDialogueAssetEditor : public FAssetEditorToolkit
{
	static const FName GraphTabId;
	static const FName DetailsTabId;
	
public:
	void InitEditor(EToolkitMode::Type Mode, TSharedPtr<IToolkitHost> InitToolkitHost, UDialogueAsset* Asset);

	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;

	virtual void SaveAsset_Execute() override;

	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;

private:
	void EnsureGraphExists();
	void DeleteSelectedNodes();
	bool CanDeleteNodes() const;

	TSharedRef<SDockTab> SpawnTab_Graph(const FSpawnTabArgs& SpawnTabArgs);
	TSharedRef<SDockTab> SpawnTab_Details(const FSpawnTabArgs& SpawnTabArgs);

	TSharedRef<SGraphEditor> BuildGraphEditor();

	void OnGraphSelectionChanged(const FGraphPanelSelectionSet& NewSelection);
	void OnDetailsPropertyChanged(const FPropertyChangedEvent& Event);

private:
	TObjectPtr<UDialogueAsset> DialogueAsset = nullptr;
	
	TSharedPtr<SGraphEditor> GraphEditor;
	TSharedPtr<IDetailsView> DetailsView;
	
	TSharedPtr<FUICommandList> GraphEditorCommands;
};

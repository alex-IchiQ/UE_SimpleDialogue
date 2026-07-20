// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.


#include "Asset/DialogueAssetDefinition.h"

#include "DialogueAsset.h"
#include "Asset/DialogueAssetEditor.h"

EAssetCommandResult UDialogueAssetDefinition::OpenAssets(const FAssetOpenArgs& OpenArgs) const
{
	for (UDialogueAsset* Asset : OpenArgs.LoadObjects<UDialogueAsset>())
	{
		const TSharedRef<FDialogueAssetEditor> Editor = MakeShared<FDialogueAssetEditor>();
		Editor->InitEditor(OpenArgs.GetToolkitMode(), OpenArgs.ToolkitHost, Asset);
	}

	return EAssetCommandResult::Handled;
}

FText UDialogueAssetDefinition::GetAssetDisplayName() const
{
	return NSLOCTEXT("Dialogue", "DialogueAssetDisplayName", "Dialogue");
}

FLinearColor UDialogueAssetDefinition::GetAssetColor() const
{
	return FLinearColor(FColor(96, 158, 255));
}

TSoftClassPtr<UObject> UDialogueAssetDefinition::GetAssetClass() const
{
	return UDialogueAsset::StaticClass();
}

TConstArrayView<FAssetCategoryPath> UDialogueAssetDefinition::GetAssetCategories() const
{
	static const FAssetCategoryPath Categories[] = {
		FAssetCategoryPath(NSLOCTEXT("Dialogue", "DialogueAssetCategory", "Dialogue"))
	};
	
	return Categories;
}

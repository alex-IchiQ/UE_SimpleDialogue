# Simple Dialogue

A **beginner-friendly, node-based dialogue system** for Unreal Engine 5.7.

Build conversations by dragging boxes and connecting them — just like Blueprints, but for dialogue.
No third-party dependencies, no scripting language to learn, and everything works from Blueprint if
you don't write C++.

> **New to Unreal plugins?** You're in the right place. This guide assumes you've opened Unreal before
> but have never touched a dialogue system or C++. Follow it top to bottom.

---

## Table of contents

1. [What is this?](#what-is-this)
2. [What you can build](#what-you-can-build)
3. [Installation (step by step)](#installation-step-by-step)
4. [Your first dialogue in 5 minutes](#your-first-dialogue-in-5-minutes)
5. [The node types](#the-node-types)
6. [Playing the dialogue in your game](#playing-the-dialogue-in-your-game)
7. [Making choices smarter: Conditions & Effects](#making-choices-smarter-conditions--effects)
8. [Troubleshooting](#troubleshooting)
9. [FAQ](#faq)
10. [License](#license)

---

## What is this?

A **dialogue** is a conversation: an NPC says something, the player picks a reply, the NPC responds,
and so on. This plugin lets you author those conversations visually and play them back in your game.

There are two halves:

- **The editor** — a graph you draw in, with a palette of dialogue nodes. This is where you *write*.
- **The runtime** — a small object (`Dialogue Runner`) that *plays* what you wrote, one node at a time,
  and tells your UI what to show.

When you **Save** a dialogue, the plugin quietly "compiles" your graph into a compact form the game
reads at runtime. You don't have to think about this — just remember: **Save = apply your changes.**

---

## What you can build

- Linear cutscene-style conversations.
- Branching dialogue where the player picks between replies.
- Loops and hubs ("What else can I help you with?") using **Jump** nodes.
- Choices that only appear when a condition is met (e.g. *"[Give 10 gold]"* only if the player has 10 gold).
- Side effects when a line plays (grant an item, set a quest flag, play a sound).

---

## Installation (step by step)

1. Find your project folder (the one with the `.uproject` file).
2. If it doesn't already have a `Plugins` folder, create one next to the `.uproject`.
3. Copy the whole `SimpleDialogue` folder into `Plugins/`. You should end up with:
   `YourProject/Plugins/SimpleDialogue/SimpleDialogue.uplugin`
4. **Right-click your `.uproject` → Generate Visual Studio project files.**
5. Open the project. Unreal will say the plugin's modules are missing and offer to rebuild — click **Yes**.
   (This compiles the C++. First build takes a few minutes.)
6. Once the editor opens, go to **Edit → Plugins**, search for **Simple Dialogue**, and make sure it's enabled.

> **No C++ toolchain?** You need Visual Studio (Windows) or Xcode (Mac) installed with the C++/game
> workloads, because this plugin has C++ code. Epic's docs on "Setting Up Visual Studio for Unreal" cover this.

---

## Your first dialogue in 5 minutes

1. In the **Content Browser**, click **Add (+) → Dialogue → Dialogue**. Name it `DL_Test`.
2. Double-click it to open the **Dialogue Editor**. You'll see one node already there: **Entry**.
   That's where every conversation begins.
3. **Right-click** on empty graph space → **Dialogue → Add Line Node**. A green node appears.
4. Click the **Entry** node's output dot and drag a wire to the **Line** node's input dot.
5. Select the Line node. In the **Details** panel on the right, fill in:
   - **Speaker Name**: `Guard`
   - **Text**: `Halt! Who goes there?`
6. Right-click again → **Add Choice Node** (a blue node). Wire the Line node's output to the Choice's input.
7. Select the Choice node. In **Details**, expand **Outputs** — there are two by default. Set their **Label**s
   to `A friend.` and `None of your business.`. Each label becomes a separate output pin on the node.
8. Add two more Line nodes for the Guard's replies and wire each Choice output to one of them.
9. **Save** (Ctrl+S). Done — your dialogue is ready to play.

That's the whole loop: **add nodes → wire them → fill in text → Save.**

---

## The node types

| Node | Color | What it does |
| --- | --- | --- |
| **Entry** | Yellow | The start. Every dialogue has exactly one; you can't delete it. Wire its output to your first node. |
| **Line** | Green | One character says one thing. Has a Speaker, Text, and a single output ("Continue"). |
| **Choice** | Blue | The player picks a reply. One output pin per option; each option's text is its **Label**. |
| **Jump** | Purple | A shortcut. Instead of dragging a long wire, point a branch at a node by its **Tag**. Great for loops and hubs. |

**About Tags and Jumps:** give any node a **Tag** (in its Details), then a Jump node can target it by
name. This lets a branch loop back to an earlier node without a messy wire crossing the whole graph.

---

## Playing the dialogue in your game

At runtime you create a **Dialogue Runner**, tell it which dialogue to play, and listen for two events:

- **On Dialogue Node** — fires each time a node is shown. Gives you the *speaker*, the *text*, and the
  list of *options* to display as buttons.
- **On Dialogue Ended** — fires when the conversation is over (close your dialogue UI here).

When the player clicks an option button, call **Select Option** with that option's `Index`.

### In Blueprint

1. `Construct Object from Class` → **Dialogue Runner**. Store it in a variable.
2. Bind events **On Dialogue Node** and **On Dialogue Ended** to functions that update your widget.
3. Call **Start Dialogue** (pass your Dialogue asset, the Player Controller, and the speaking Actor).
4. In your option buttons' OnClicked, call **Select Option** with the index of the option.

### In C++

```cpp
UDialogueRunner* Runner = NewObject<UDialogueRunner>(this);

Runner->OnDialogueNode.AddDynamic(this, &AMyActor::HandleNode);
Runner->OnDialogueEnded.AddDynamic(this, &AMyActor::HandleEnded);

Runner->StartDialogue(MyDialogueAsset, PlayerController, SpeakerActor);

// ...later, when the player clicks an option:
Runner->SelectOption(ClickedOption.Index);
```

```cpp
void AMyActor::HandleNode(const FText& Speaker, const FText& Text, const TArray<FDialogueOption>& Options)
{
    // Show Speaker + Text, and make one button per Options[i], each calling SelectOption(Options[i].Index).
}
```

> A **Line** node always gives you exactly one option, labelled "Continue", so the same code handles
> both plain lines and branching choices — you just render whatever options you're handed.

---

## Making choices smarter: Conditions & Effects

Two optional building blocks let dialogue react to your game:

- **Condition** — decides whether a choice is *shown*. Example: only show *"[Bribe the guard]"* if the
  player has enough gold.
- **Effect** — runs some logic when a node is *entered*. Example: subtract the gold, or set a quest flag.

You can make both **without C++** — right-click in the Content Browser → **Blueprint Class**, pick
`DialogueCondition` or `DialogueEffect` as the parent, and override the `Evaluate` / `Execute` event.

Then assign them in the Details panel:
- A **Condition** goes on a Choice node's **Output → Condition**.
- An **Effect** goes on any node's **On Enter Effects** list.

Both receive an **Execution Context** with the Runner, the Player, the Speaker, and a **Blackboard**
(a free-form key/value store) you can use to pass data around during a conversation.

---

## Troubleshooting

**The plugin won't build / modules are missing.**
Make sure Visual Studio (or Xcode) is installed with C++ game-development support, then regenerate
project files and rebuild. The plugin targets **Unreal Engine 5.7**.

**My dialogue changes don't show up in game.**
Did you **Save** the asset? Saving is what compiles the graph. Unsaved edits aren't played.

**"Add → Dialogue" doesn't appear in the Content Browser.**
The plugin isn't enabled. Go to **Edit → Plugins**, find *Simple Dialogue*, tick it, and restart.

**A Choice option never appears at runtime.**
It probably has a **Condition** that returns false, or its output pin isn't connected. An unconnected
output is a valid "dead end" and simply ends that branch.

**A Jump node does nothing.**
Check its **Target Tag** matches the **Tag** of an existing node exactly (tags are picked from a dropdown).

---

## FAQ

**Do I need to know C++?**
No. Authoring is fully visual, and playback plus Conditions/Effects all work from Blueprint. C++ is only
there if you prefer it.

**Can one NPC have several dialogues?**
Yes — each Dialogue asset is independent. Pick which one to play when you call *Start Dialogue*.

**Does it handle localization?**
All player-facing text uses `FText`, so it works with Unreal's standard localization pipeline.

**Is it free?**
Yes — MIT licensed. Use it in commercial projects, modify it, ship it.

---

## License

Released under the [MIT License](LICENSE). © 2026 IchiQ.

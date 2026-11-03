# Menu flow

After initialization, the Qt main window opens the main menu. Each box below represents a screen or the exit action.

```mermaid
flowchart TD
    menu["Main menu"]
    menu --> cp["Create player"]
    menu --> ct["Create tournament"]
    menu --> sp["Search players"]
    menu --> st["Search tournaments"]
    menu --> lb["Leaderboard by sport"]
    menu --> quit(["Exit application"])
    cp -->|Created or Back| menu
    ct -->|Created or Back| menu
    sp -->|Select player| pd["Player details"]
    pd -->|Back| sp
    st -->|Select tournament| td["Tournament details"]
    td -->|Back| st
    sp -->|Back| menu
    st -->|Back| menu
    lb -->|Back| menu
```

- **Player details.** View player statistics, add the player to a tournament, or remove them from one.
- **Tournament details.** View tournament statistics, start the tournament, or simulate the current round.
- **Leaderboard.** Switch between football and basketball to refresh the rankings on the same screen.

[Browse the Qt navigation code](../../ui/MainWindow.cpp)

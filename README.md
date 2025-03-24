# PokeViewer

# PokeViewer ⚡

This is a Qt-based desktop app that interacts with the [PokeAPI](https://pokeapi.co) to fetch and display Pokémon data including:

- Name
- Type(s)
- Abilities
- Weight
- Sprite image

## 💻 Built With
- C++
- Qt 6.8.2
- Qt Widgets (UI)
- Qt Network (HTTP GET for API + image requests)
- JSON parsing via `QJsonDocument`

## 🖼️ App Features
- Enter a Pokémon name (e.g., `pikachu`, `bulbasaur`)
- Automatically fetches info and displays it in a structured UI per user request via a button press
- Displays sprite image, types, abilities, and weight

## 🛠️ How to Build
Built for:
- Qt Creator (or Qt installed manually)
- A C++17 compiler

To build:
1. Open `pokeviewer.pro` in Qt Creator
2. Configure your kit (e.g., MinGW 64-bit)
3. Build and run!

## 📂 Files Debrief
- `main.cpp`: Application entry point
- `widget.cpp/h`: Main UI logic and API handlers
- `widget.ui`: Qt Designer file
- `pokeviewer.pro`: Project file


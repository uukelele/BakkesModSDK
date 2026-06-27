# BakkesModSDK
BakkesMod is a mod for Rocket League initially built as an enhancement for freeplay. Over time it has grown and now it features enhancements such as a dollycam, variance options to the ingame custom trainer, multiplayer game hosting, POV goal replays and much more.

BakkesMod also provides a software development kit/API which interfaces with Rocket League. For now it is mostly used for creating freeplay trainings and replay enhancements (such as analysis tools). The plan is to also extend the SDK so (limited) information can be retrieved from online games, which can be used to for example keep track of advanced statistics during gameplay.

Examples of plugins that are embedded with the mod when it is installed can be found [here](https://github.com/Bakkes/BakkesMod2-Plugins). The dollycam plugin is also open source and can be found [here](https://github.com/Bakkes/DollyCamPlugin2)

To get started using BakkesMod, simply go to http://bakkesmod.com and download the injector. If you wish to create plugins for the mod follow the steps that are described further down this page.

Currently the SDK is being extended to make it easier for plugin developers to create their own multiplayer gamemodes/training scenarios. BakkesMod has the ability to host an online game server which other players can connect to. For now only freeplay is supported but support for Team soccar/rumble is being added. Simply type "host" in the BakkesMod console to host a game, other players need to type "connect 127.0.0.1" to connect to your server (where 127.0.0.1 is your IP of course). In order for this to work you need to have forwarded port 27016 or use an utility such as Hamachi to emulate a LAN environment.

## Getting started
In order to create plugins for Rocket League with BakkesMod you need the SDK. By default the SDK will be included with the BakkesMod installation and can be found in %appdata%\bakkesmod\bakkesmod\bakkesmodsdk\. You could also simply clone this repository as this also contains the full SDK and is often more up to date.

### IDE
To get started quickly, check out the [template](https://github.com/Martinii89/BakkesmodPluginTemplate) to automatically generate a project skeleton. In order to load your plugin ingame make sure the generated .dll is placed in the plugins folder of BakkesMod. Next, in Rocket League open the console and type ```plugin load filename``` (without the .dll extension!). To unload your plugin you can use ```plugin unload filename```. Any cvars/notifiers/drawables that are registered when a plugin is loaded are automatically removed when a plugin is unloaded, so you don't have to worry about that!

### Command Line

You'll still need to have Visual Studio installed for its included compilers.

1. Open `x64 Native Tools Command Prompt for VS 2019`.
2. Replace the paths to `bakkesmodsdk` below with the correct ones for your system
   and `plugin.cpp` with the actual name of the plugin.

```powershell
cl /LD -I bakkesmodsdk/include/ bakkesmodsdk/lib/pluginsdk.lib plugin.cpp
```

## Python Bindings

In addition to the C++ SDK, this repository includes **CPython bindings** (via pybind11) that expose the full BakkesMod SDK API to Python 3.8+ scripts running inside the mod.

### Structure

```
python/
├── bakkesmod/          # Python package (import as `from bakkesmod import ...`)
│   └── __init__.py     # Re-exports all ~215 SDK types with __all__
├── bindings/           # C++ pybind11 source files (10 files, ~3800 lines)
│   ├── main.cpp        # Module definition
│   ├── core_structs.cpp    # Math types, structs, HTTP (Vector, Rotator, CurlRequest...)
│   ├── enums.cpp           # 12 SDK enums (PLUGINTYPE, Equipslot, OnlinePlatform...)
│   ├── engine_wrappers.cpp # Object → Actor → RBActor → PRI chain
│   ├── game_object_wrappers.cpp  # Car, Ball, Camera, Rumble pickups...
│   ├── game_event_wrappers.cpp   # Server, Replay, TrainingEditor...
│   ├── game_wrapper.cpp    # GameWrapper — main API entry point
│   ├── wrappers.cpp        # Canvas, CVar, MMR, Matchmaking, Sequences...
│   ├── items_wrappers.cpp  # Items, Products, Attributes, Loadouts...
│   └── misc_wrappers.cpp   # HTTP, Image, GfxDataTraining, 19 Rumble subclasses
├── setup.py            # setuptools build config
├── pyproject.toml      # PEP 517 build config
├── CMakeLists.txt      # Alternative CMake build
└── examples/           # 9 example Python plugins
    ├── basic_plugin.py         # Full plugin skeleton (onLoad/onUnload, cvars, notifiers)
    ├── game_state.py           # Ball/car/server/team state, physics, ball prediction
    ├── event_hooks.py          # 7 game event hooks (goal scored, demos, supersonic...)
    ├── rendering.py            # Canvas drawing (text, boxes, lines, 3D projection)
    ├── cvars_and_notifiers.py  # CVars with bounds, key binds (F6-F8), aliases
    ├── http_requests.py        # GET/POST/JSON requests via HttpWrapper
    ├── mmr_and_rank.py         # MMR, skill rating, competitive rank lookup
    ├── items_loadout.py        # Product/inventory/loadout queries
    └── training_tools.py       # Freeplay ball spawn, training round control, ball prediction
```

### Building

The bindings must be compiled **on Windows with MSVC** (the SDK is Windows-only and links against `pluginsdk.lib`).

#### Using setuptools (recommended)

```powershell
# From the python/ directory:
cd python
pip install .
```

This will compile the 10 C++ binding files into `bakkesmod/_bindings.pyd` and install the `bakkesmod` package into your current Python environment.

#### Using CMake

```powershell
cd python
mkdir build && cd build
cmake .. -A x64
cmake --build . --config Release
```

Copy the generated `_bindings.pyd` alongside `bakkesmod/__init__.py`.

### Requirements

- **Windows** (the SDK lib is a `.lib` targeting Windows)
- **Python 3.8+**
- **Visual Studio 2019+** with the "Desktop development with C++" workload
- **pybind11** (`pip install pybind11`)

### How Python Plugins Work

A Python plugin consists of two pieces:

1. **`_bindings.pyd`** — The compiled CPython extension (the `.dll` you built above). Goes into the `bakkesmod/` package folder.
2. **Your `.py` script** — A Python file with a class implementing `on_load` / `on_unload`. Goes into the BakkesMod plugins folder.

BakkesMod's Python plugin system uses [pybind11](https://github.com/pybind/pybind11) under the hood. When it loads a Python script, it calls `on_load(game_wrapper, cvar_manager)` and your plugin registers cvars, notifiers, hooks, and drawables — exactly like a C++ plugin.

### Loading a Python Plugin Ingame

1. **Install the bindings** (one-time):
   ```powershell
   cd python
   pip install .
   ```
   This compiles `_bindings.pyd` and installs the `bakkesmod` package into your Python environment. BakkesMod needs to find this package at runtime — see requirements below.

2. **Write your plugin** as a `.py` file. The class must define `on_load(self, game_wrapper, cvar_manager)` and `on_unload(self)`:
   ```python
   from bakkesmod import GameWrapper, CVarManagerWrapper, NOTIFIER_PERMISSION

   class MyPlugin:
       def on_load(self, game_wrapper: GameWrapper, cvar_manager: CVarManagerWrapper) -> None:
           self.gw = game_wrapper
           cvar_manager.registerNotifier("hello", lambda args: self._hello(), "Say hello.", NOTIFIER_PERMISSION.ALL)

       def _hello(self) -> None:
           self.gw.LogToChatbox("Hello from Python!", "MyPlugin")

       def on_unload(self) -> None:
           pass
   ```

3. **Place the `.py` file** in the BakkesMod `plugins/` folder (typically `%appdata%\bakkesmod\bakkesmod\plugins\`).

4. **Start Rocket League** and open the BakkesMod console (default: **F6**). Type:
   ```
   plugin load my_plugin
   ```
   (Omit the `.py` extension, just like C++ plugins omit `.dll`.)

5. **Test it**: Type `hello` in the console. You should see `"Hello from Python!"` in the chatbox.

6. **Unload** at any time:
   ```
   plugin unload my_plugin
   ```

### Python Plugin Lifecycle

| Event | What happens |
|---|---|
| `plugin load my_plugin` | BakkesMod imports `my_plugin.py` and calls `MyPlugin.on_load(gw, cvar)` |
| Console command typed | Your notifier callback fires with `args[0]` = command name, `args[1:]` = arguments |
| Game event fires | Your hook callback fires with the event name string |
| `plugin unload my_plugin` | BakkesMod calls `MyPlugin.on_unload()`, then automatically cleans up all cvars, notifiers, hooks, and drawables |
| Plugin reloaded | BakkesMod calls `on_unload()`, re-imports the module, then calls `on_load()` again |

### Important Notes

- **Notifiers, cvars, hooks, and drawables are automatically cleaned up** on unload — you don't need to manually remove them in `on_unload()`.
- **HTTP callbacks run on background threads**. Always wrap SDK calls in `GameWrapper.Execute(lambda _: ...)` when inside an HTTP callback — see the [http_requests.py](python/examples/http_requests.py) example.
- **The `bakkesmod` package** (with `_bindings.pyd` inside) must be importable by BakkesMod's embedded Python. This usually means it needs to be installed in BakkesMod's bundled Python environment, or `PYTHONPATH` must point to it.
- **Naming**: Your plugin's `.py` filename becomes its plugin name. Avoid spaces and special characters.

See the [examples](python/examples/) directory for complete, documented plugins covering the full API surface — each one is a real plugin you can load with `plugin load <name>`.

### API Coverage

The bindings expose all major SDK wrapper types including:

| Category | Types |
|---|---|
| **Game state** | `GameWrapper`, `ServerWrapper`, `CarWrapper`, `BallWrapper`, `PriWrapper`, `TeamWrapper` |
| **Training** | `TrainingEditorWrapper`, `GfxDataTrainingWrapper`, `TutorialWrapper` |
| **Replays** | `ReplayWrapper`, `ReplayDirectorWrapper`, `ReplaySoccarWrapper`, `ReplayServerWrapper` |
| **Items** | `ItemsWrapper`, `ProductWrapper`, `OnlineProductWrapper`, `LoadoutWrapper`, 13 attribute types, 5 databases |
| **Rumble** | 20 rumble pickup types (`AttachmentPickup`, `TornadoPickup`, `GrapplingHookPickup`, ...) |
| **Camera** | `CameraWrapper`, `CameraXWrapper`, `BaseCameraWrapper`, 5 camera state types |
| **MMR/Rank** | `MMRWrapper`, `UniqueIDWrapper`, `SkillRating`, `SkillRank` |
| **Rendering** | `CanvasWrapper` (text/shapes/3D projection) |
| **HTTP** | `HttpWrapper` (GET/POST/JSON/file), `CurlRequest`, `FormField` |
| **CVars** | `CVarManagerWrapper`, `CVarWrapper` (typed cvars) |
| **Math** | `Vector`, `Rotator`, `Quat`, `LinearColor`, `RBState`, `PredictionInfo` |
| **Utilities** | `ModalWrapper`, `ImageWrapper`, `MatchmakingWrapper`, `MenuStackWrapper`, `EnumWrapper` |

## Help & more info

If you have any questions feel free to drop by the [BakkesMod programming discord](https://discord.gg/HMptXSzCvU). This discord is also full of many tutorials on how to get started.

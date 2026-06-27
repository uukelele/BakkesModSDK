#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <bakkesmod/wrappers/GameWrapper.h>

namespace py = pybind11;

void init_game_wrapper(py::module& m) {
    // ============================================================
    // GameWrapper (the main API entry point)
    // ============================================================
    py::class_<GameWrapper>(m, "GameWrapper",
        R"doc(The main entry point for the BakkesMod SDK API.

        Provides access to all game state, rendering, event hooks, timing,
        modals, and utility functions. Every plugin receives a GameWrapper
        instance via the BakkesModPlugin base class.
        )doc")
        .def(py::init<std::uintptr_t, long, std::type_index>(),
            py::arg("memory_address"), py::arg("plugin_type"), py::arg("type_index"))

        // Game state queries
        .def("IsInGame", &GameWrapper::IsInGame,
            "Check if the game is currently in any match.")
        .def("IsInOnlineGame", &GameWrapper::IsInOnlineGame,
            "Check if currently in an online match.")
        .def("IsInFreeplay", &GameWrapper::IsInFreeplay,
            "Check if currently in freeplay mode.")
        .def("IsInReplay", &GameWrapper::IsInReplay,
            "Check if viewing a replay.")
        .def("IsInCustomTraining", &GameWrapper::IsInCustomTraining,
            "Check if currently in custom training.")
        .def("IsPaused", &GameWrapper::IsPaused,
            "Check if the game is paused.")
        .def("IsUsingEpicVersion", &GameWrapper::IsUsingEpicVersion,
            "Check if running the Epic Games version.")
        .def("IsUsingSteamVersion", &GameWrapper::IsUsingSteamVersion,
            "Check if running the Steam version.")

        // Game state access
        .def("GetCurrentGameState", &GameWrapper::GetCurrentGameState,
            "Get the current game state as a ServerWrapper.")
        .def("GetOnlineGame", &GameWrapper::GetOnlineGame,
            "Get the online game state.")
        .def("GetGameEventAsServer", &GameWrapper::GetGameEventAsServer,
            "Get the current game event as a ServerWrapper.")
        .def("GetGameEventAsReplay", &GameWrapper::GetGameEventAsReplay,
            "Get the current game event as a ReplayServerWrapper.")

        // Player/Camera access
        .def("GetLocalCar", &GameWrapper::GetLocalCar,
            "Get the local player's car.")
        .def("GetCamera", &GameWrapper::GetCamera,
            "Get the game camera.")
        .def("GetEngine", &GameWrapper::GetEngine,
            "Get the physics engine state wrapper.")
        .def("GetPlayerController", &GameWrapper::GetPlayerController,
            "Get the local player's controller.")

        // Wrapper accessors
        .def("GetMMRWrapper", &GameWrapper::GetMMRWrapper,
            "Get the MMR/skill rating wrapper.")
        .def("GetItemsWrapper", &GameWrapper::GetItemsWrapper,
            "Get the items/inventory wrapper.")
        .def("GetMatchmakingWrapper", &GameWrapper::GetMatchmakingWrapper,
            "Get the matchmaking wrapper.")
        .def("GetSettings", &GameWrapper::GetSettings,
            "Get the game settings wrapper.")
        .def("GetPluginManager", &GameWrapper::GetPluginManager,
            "Get the plugin manager wrapper.")
        .def("GetGUIManager", &GameWrapper::GetGUIManager,
            "Get the GUI manager wrapper.")
        .def("GetUserLoadoutSave", &GameWrapper::GetUserLoadoutSave,
            "Get the user's loadout save wrapper.")
        .def("GetReplayManagerWrapper", &GameWrapper::GetReplayManagerWrapper,
            "Get the replay manager wrapper.")
        .def("GetMapListWrapper", &GameWrapper::GetMapListWrapper,
            "Get the map list wrapper.")

        // Modals
        .def("CreateModal", &GameWrapper::CreateModal, py::arg("title"),
            "Create a modal dialog with the given title.")
        .def("CreateTextInputModal", &GameWrapper::CreateTextInputModal, py::arg("title"),
            "Create a text input modal dialog with the given title.")

        // Timing/Threading
        .def("SetTimeout", &GameWrapper::SetTimeout, py::arg("callback"), py::arg("time"),
            R"doc(Set a timed callback to execute after a delay.

            Args:
                callback: Callable that takes a GameWrapper parameter.
                time: Delay in seconds.
            )doc")
        .def("Execute", &GameWrapper::Execute, py::arg("callback"),
            R"doc(Execute a callback on the game thread (use from background threads).

            Args:
                callback: Callable that takes a GameWrapper parameter.
            )doc")

        // Rendering
        .def("RegisterDrawable", &GameWrapper::RegisterDrawable, py::arg("callback"),
            R"doc(Register a drawable callback for 2D rendering.

            Args:
                callback: Callable that takes a CanvasWrapper parameter.
                    Called every frame for rendering.
            )doc")
        .def("UnregisterDrawables", &GameWrapper::UnregisterDrawables,
            "Unregister all drawable callbacks.")

        // FName utilities
        .def("GetFNameByIndex", &GameWrapper::GetFNameByIndex, py::arg("index"),
            "Get an FName string by its index.")
        .def("GetFNameIndexByString", &GameWrapper::GetFNameIndexByString, py::arg("name"),
            "Get an FName index by its string.")

        // Event hooks
        .def("HookEvent", &GameWrapper::HookEvent, py::arg("eventName"), py::arg("callback"),
            R"doc(Hook a game event by name.

            Args:
                eventName: The name of the event to hook.
                callback: Callable that takes the event name string.
            )doc")
        .def("UnhookEvent", &GameWrapper::UnhookEvent, py::arg("eventName"),
            "Remove a previously registered event hook.")
        .def("HookEventPost", &GameWrapper::HookEventPost, py::arg("eventName"), py::arg("callback"),
            "Hook a game event by name (post-event callback).")
        .def("UnhookEventPost", &GameWrapper::UnhookEventPost, py::arg("eventName"),
            "Remove a post-event hook.")

        // Communication
        .def("LogToChatbox", &GameWrapper::LogToChatbox, py::arg("text"), py::arg("sender") = "BAKKESMOD",
            "Send a message to the game chatbox.")

        // Toast notifications
        .def("LoadToastTexture", py::overload_cast<std::string, std::string>(&GameWrapper::LoadToastTexture),
            py::arg("name"), py::arg("path"),
            "Load a texture for use in toast notifications.")
        .def("Toast", &GameWrapper::Toast, py::arg("title"), py::arg("text"),
            py::arg("texture") = "default", py::arg("timeout") = 3.5f,
            py::arg("toastType") = (uint8_t)0, py::arg("width") = 290.f,
            py::arg("height") = 60.f,
            R"doc(Show a toast notification.

            Args:
                title: The title text.
                text: The body text.
                texture: Texture name (default shows BakkesMod logo).
                timeout: Display duration in seconds.
                toastType: Toast type (0=Info, 1=OK, 2=Warning, 3=Error).
                width: Toast width in pixels.
                height: Toast height in pixels.
            )doc")

        // Input
        .def("IsKeyPressed", &GameWrapper::IsKeyPressed, py::arg("keyName"),
            "Check if a key is currently pressed.")
        .def("IsCursorVisible", &GameWrapper::IsCursorVisible,
            "Check cursor visibility: 0=hidden, 1=RL cursor, 2=ImGui cursor.")

        // Commands
        .def("ExecuteUnrealCommand", &GameWrapper::ExecuteUnrealCommand, py::arg("command"),
            "Execute an Unreal Engine console command.")

        // Map utilities
        .def("GetRandomMap", &GameWrapper::GetRandomMap,
            "Get a random map/arena name.")
        .def("GetCurrentMap", &GameWrapper::GetCurrentMap,
            "Get the current map/arena name.")

        // Player identification
        .def("GetSteamID", &GameWrapper::GetSteamID,
            "Get the local player's Steam ID (deprecated, use GetUniqueID).")
        .def("GetEpicID", &GameWrapper::GetEpicID,
            "Get the local player's Epic account ID.")
        .def("GetUniqueID", &GameWrapper::GetUniqueID,
            "Get the local player's cross-platform unique ID.")
        .def("GetPlayerName", &GameWrapper::GetPlayerName,
            "Get the local player's name.")
        .def("GetLocalClub", &GameWrapper::GetLocalClub,
            "Get the local player's club details.")
        .def("GetMainSequence", &GameWrapper::GetMainSequence,
            "Get the main Kismet sequence.")
        .def("GetGfxTrainingData", &GameWrapper::GetGfxTrainingData,
            "Get training GFX data.")
        .def("GetMenuStack", &GameWrapper::GetMenuStack,
            "Get the menu stack wrapper.")

        // Display
        .def("GetScreenSize", &GameWrapper::GetScreenSize,
            "Get the screen size in pixels.")
        .def("GetDisplayScale", &GameWrapper::GetDisplayScale,
            "Get the display/UI scale factor.")
        .def("GetInterfaceScale", &GameWrapper::GetInterfaceScale,
            "Get the interface scale factor.")
        .def("GetbMetric", &GameWrapper::GetbMetric,
            "Check if metric units are enabled.")
        .def("GetUILanguage", &GameWrapper::GetUILanguage,
            "Get the current UI language.")
        .def("GetbColorBlind", &GameWrapper::GetbColorBlind,
            "Check if color blind mode is enabled.")

        // Path utilities
        .def("GetBakkesModPathW", &GameWrapper::GetBakkesModPathW,
            "Get the BakkesMod installation directory (wide string).")
        .def("GetDataFolderW", &GameWrapper::GetDataFolderW,
            "Get the BakkesMod data folder path (wide string).")
        .def("GetBakkesModVersion", &GameWrapper::GetBakkesModVersion,
            "Get the BakkesMod version number.")

        // Replay
        .def("PlayReplay", &GameWrapper::PlayReplay, py::arg("path"),
            "Play a replay file from the given path.")
        .def("PlayReplayFromTime", &GameWrapper::PlayReplayFromTime, py::arg("path"), py::arg("time"),
            "Play a replay file from a specific time offset.")

        // Bot loadout (experimental)
        .def("SetBotLoadout", &GameWrapper::SetBotLoadout, py::arg("bot_pri"), py::arg("loadout_data"),
            R"doc(Set the loadout for a bot player. (Experimental)

            Args:
                bot_pri: The bot's PriWrapper.
                loadout_data: The BotLoadoutData to apply.
            )doc")

        // Steam version
        .def("GetSteamVersion", &GameWrapper::GetSteamVersion,
            "Get the Steam version/build number.")
        .def("GetPsyBuildID", &GameWrapper::GetPsyBuildID,
            "Get the Psyonix build ID string.");
}

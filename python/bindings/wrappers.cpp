#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <bakkesmod/wrappers/canvaswrapper.h>
#include <bakkesmod/wrappers/cvarmanagerwrapper.h>
#include <bakkesmod/wrappers/cvarwrapper.h>
#include <bakkesmod/wrappers/MMRWrapper.h>
#include <bakkesmod/wrappers/SettingsWrapper.h>
#include <bakkesmod/wrappers/UniqueIDWrapper.h>
#include <bakkesmod/wrappers/MatchmakingWrapper.h>
#include <bakkesmod/wrappers/PluginManagerWrapper.h>
#include <bakkesmod/wrappers/GuiManagerWrapper.h>
#include <bakkesmod/wrappers/MenuStackWrapper.h>
#include <bakkesmod/wrappers/SpectatorHUDWrapper.h>
#include <bakkesmod/wrappers/ControllerWrapper.h>
#include <bakkesmod/wrappers/AIControllerWrapper.h>
#include <bakkesmod/wrappers/PlayerControllerWrapper.h>
#include <bakkesmod/wrappers/ReplayServerWrapper.h>
#include <bakkesmod/wrappers/ReplayViewerDataWrapper.h>
#include <bakkesmod/wrappers/CareerStatsWrapper.h>
#include <bakkesmod/wrappers/GameObject/ReplayManagerWrapper.h>
#include <bakkesmod/wrappers/GameObject/Stats/StatEventWrapper.h>
#include <bakkesmod/wrappers/Engine/EnumWrapper.h>
#include <bakkesmod/wrappers/kismet/SequenceWrapper.h>
#include <bakkesmod/wrappers/kismet/SequenceObjectWrapper.h>
#include <bakkesmod/wrappers/kismet/SequenceVariableWrapper.h>
#include <bakkesmod/wrappers/kismet/SequenceOpWrapper.h>
#include <bakkesmod/wrappers/GameObject/PerformanceStats/SampleHistoryWrapper.h>
#include <bakkesmod/wrappers/GameObject/PerformanceStats/StatGraphWrapper.h>
#include <bakkesmod/wrappers/GameObject/PerformanceStats/StatGraphSystemWrapper.h>
#include <bakkesmod/wrappers/GameObject/PerformanceStats/StartGraphSystemWrapper.h>
#include <bakkesmod/wrappers/GameObject/PerformanceStats/InputBufferGraphWrapper.h>
#include <bakkesmod/wrappers/GameObject/PerformanceStats/NetStatGraphWrapper.h>
#include <bakkesmod/wrappers/GameObject/PerformanceStats/PerfStatGraphWrapper.h>
#include <bakkesmod/wrappers/GameObject/PerformanceStats/SampleRecordSettingsWrapper.h>
#include <bakkesmod/wrappers/GameObject/MeshComponents/MeshComponentWrapper.h>
#include <bakkesmod/wrappers/GameObject/MeshComponents/CarMeshComponentBaseWrapper.h>
#include <bakkesmod/wrappers/GameObject/MeshComponents/SkeletalMeshComponentWrapper.h>

namespace py = pybind11;

void init_wrappers(py::module& m) {
    // ============================================================
    // CanvasWrapper
    // ============================================================
    py::class_<CanvasWrapper>(m, "CanvasWrapper",
        R"doc(2D rendering canvas for drawing on the game screen.

        Provides line, box, triangle, text, and texture drawing primitives.
        Use with GameWrapper.RegisterDrawable() to render each frame.
        )doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"))

        // Float-based API
        .def("SetPosition", py::overload_cast<Vector2F>(&CanvasWrapper::SetPosition), py::arg("pos"),
            "Set the drawing cursor position (float coordinates).")
        .def("GetPositionFloat", &CanvasWrapper::GetPositionFloat,
            "Get the current drawing cursor position.")
        .def("SetColor", py::overload_cast<char, char, char, char>(&CanvasWrapper::SetColor),
            py::arg("Red"), py::arg("Green"), py::arg("Blue"), py::arg("Alpha"),
            "Set the drawing color using 0-255 byte values.")
        .def("SetColor", py::overload_cast<LinearColor>(&CanvasWrapper::SetColor), py::arg("color"),
            "Set the drawing color using a LinearColor.")
        .def("GetColor", &CanvasWrapper::GetColor,
            "Get the current drawing color.")
        .def("DrawBox", py::overload_cast<Vector2F>(&CanvasWrapper::DrawBox), py::arg("size"),
            "Draw an unfilled box outline of the given size.")
        .def("FillBox", py::overload_cast<Vector2F>(&CanvasWrapper::FillBox), py::arg("size"),
            "Draw a filled box of the given size.")
        .def("FillTriangle", py::overload_cast<Vector2F, Vector2F, Vector2F>(&CanvasWrapper::FillTriangle),
            py::arg("p1"), py::arg("p2"), py::arg("p3"),
            "Draw a filled triangle with the given vertices.")
        .def("FillTriangle", py::overload_cast<Vector2F, Vector2F, Vector2F, LinearColor>(&CanvasWrapper::FillTriangle),
            py::arg("p1"), py::arg("p2"), py::arg("p3"), py::arg("color"),
            "Draw a filled triangle with the given vertices and color.")
        .def("DrawString", py::overload_cast<std::string>(&CanvasWrapper::DrawString),
            py::arg("text"), "Draw a string of text at the current position.")
        .def("DrawString", py::overload_cast<std::string, float, float>(&CanvasWrapper::DrawString),
            py::arg("text"), py::arg("xScale"), py::arg("yScale"),
            "Draw a string with custom scale.")
        .def("DrawString", py::overload_cast<std::string, float, float, bool, bool>(&CanvasWrapper::DrawString),
            py::arg("text"), py::arg("xScale"), py::arg("yScale"),
            py::arg("dropShadow"), py::arg("wrapText") = false,
            "Draw a string with scale, drop shadow, and word wrap options.")
        .def("GetStringSize", &CanvasWrapper::GetStringSize,
            py::arg("text"), py::arg("xScale") = 1.0f, py::arg("yScale") = 1.0f,
            "Get the rendered size of a string.")
        .def("DrawLine", py::overload_cast<Vector2F, Vector2F>(&CanvasWrapper::DrawLine),
            py::arg("start"), py::arg("end"),
            "Draw a line between two points.")
        .def("DrawLine", py::overload_cast<Vector2F, Vector2F, float>(&CanvasWrapper::DrawLine),
            py::arg("start"), py::arg("end"), py::arg("width"),
            "Draw a line with a specific width.")
        .def("DrawRect", py::overload_cast<Vector2F, Vector2F>(&CanvasWrapper::DrawRect),
            py::arg("start"), py::arg("end"),
            "Draw a rectangle defined by start and end corners.")
        .def("DrawTexture", &CanvasWrapper::DrawTexture, py::arg("img"), py::arg("scale"),
            "Draw a texture at the current position.")
        .def("DrawTile", &CanvasWrapper::DrawTile, py::arg("img"), py::arg("XL"), py::arg("YL"),
            py::arg("U"), py::arg("V"), py::arg("UL"), py::arg("VL"), py::arg("Color"),
            py::arg("ClipTile"), py::arg("Blend"))
        .def("DrawRotatedTile", &CanvasWrapper::DrawRotatedTile, py::arg("img"), py::arg("Rotation"),
            py::arg("XL"), py::arg("YL"), py::arg("U"), py::arg("V"), py::arg("UL"), py::arg("VL"),
            py::arg("AnchorX"), py::arg("AnchorY"))
        .def("SetPosition", py::overload_cast<Vector2>(&CanvasWrapper::SetPosition), py::arg("pos"))
        .def("GetPosition", &CanvasWrapper::GetPosition)
        .def("DrawBox", py::overload_cast<Vector2>(&CanvasWrapper::DrawBox), py::arg("size"))
        .def("FillBox", py::overload_cast<Vector2>(&CanvasWrapper::FillBox), py::arg("size"))
        .def("FillTriangle", py::overload_cast<Vector2, Vector2, Vector2>(&CanvasWrapper::FillTriangle),
            py::arg("p1"), py::arg("p2"), py::arg("p3"))
        .def("FillTriangle", py::overload_cast<Vector2, Vector2, Vector2, LinearColor>(&CanvasWrapper::FillTriangle),
            py::arg("p1"), py::arg("p2"), py::arg("p3"), py::arg("color"))
        .def("DrawLine", py::overload_cast<Vector2, Vector2>(&CanvasWrapper::DrawLine),
            py::arg("start"), py::arg("end"))
        .def("DrawLine", py::overload_cast<Vector2, Vector2, float>(&CanvasWrapper::DrawLine),
            py::arg("start"), py::arg("end"), py::arg("width"))
        .def("DrawRect", py::overload_cast<Vector2, Vector2>(&CanvasWrapper::DrawRect),
            py::arg("start"), py::arg("end"))
        .def("Project", &CanvasWrapper::Project, py::arg("location"),
            "Project a 3D world location to 2D screen coordinates.")
        .def("ProjectF", &CanvasWrapper::ProjectF, py::arg("location"),
            "Project a 3D world location to 2D float screen coordinates.")
        .def("DeProject", &CanvasWrapper::DeProject, py::arg("screenPos"),
            R"doc(Convert screen coordinates to a world-space ray.

            Returns:
                tuple: (worldOrigin, worldDirection) as Vector2F pair.
            )doc")
        .def("GetSize", &CanvasWrapper::GetSize,
            "Get the canvas/screen size.");

    // ============================================================
    // CVarManagerWrapper
    // ============================================================
    py::class_<CVarManagerWrapper>(m, "CVarManagerWrapper",
        R"doc(Manager for console variables, notifiers, and commands.

        Used to register cvars, notifiers, execute commands, manage binds,
        aliases, and config files.
        )doc")
        .def(py::init<std::uintptr_t, std::type_index>(), py::arg("memory_address"), py::arg("plugin_type_index"))
        .def("executeCommand", &CVarManagerWrapper::executeCommand,
            py::arg("command"), py::arg("log") = true,
            "Execute a console command.")
        .def("registerNotifier",
            py::overload_cast<std::string, std::function<void(std::vector<std::string>)>, std::string, unsigned char>(
                &CVarManagerWrapper::registerNotifier),
            py::arg("cvar"), py::arg("notifier"), py::arg("description"), py::arg("permissions"),
            R"doc(Register a notifier (console command callback).

            Args:
                cvar: The notifier name.
                notifier: Callback receiving the command arguments as a list of strings.
                description: Description for the notifier.
                permissions: Permission flags (NOTIFIER_PERMISSION values).
            )doc")
        .def("removeNotifier", &CVarManagerWrapper::removeNotifier, py::arg("cvar"),
            "Remove a previously registered notifier.")
        .def("registerCvar", &CVarManagerWrapper::registerCvar,
            py::arg("cvar"), py::arg("defaultValue"), py::arg("desc") = "",
            py::arg("searchable") = true, py::arg("hasMin") = false, py::arg("min") = 0.0f,
            py::arg("hasMax") = false, py::arg("max") = 0.0f, py::arg("saveToCfg") = true,
            R"doc(Register a console variable (cvar).

            Args:
                cvar: The cvar name.
                defaultValue: Default value string.
                desc: Description of the cvar.
                searchable: Whether the cvar appears in search.
                hasMin: Whether there's a minimum value constraint.
                min: Minimum value (if hasMin).
                hasMax: Whether there's a maximum value constraint.
                max: Maximum value (if hasMax).
                saveToCfg: Whether to save this cvar to the config file.

            Returns:
                CVarWrapper: The registered cvar.
            )doc")
        .def("removeCvar", &CVarManagerWrapper::removeCvar, py::arg("cvar"),
            "Remove a previously registered cvar.")
        .def("log", py::overload_cast<std::string>(&CVarManagerWrapper::log),
            py::arg("text"), "Log text to the BakkesMod console.")
        .def("log", py::overload_cast<std::wstring>(&CVarManagerWrapper::log),
            py::arg("text"), "Log wide text to the BakkesMod console.")
        .def("getCvar", &CVarManagerWrapper::getCvar, py::arg("cvar"),
            "Get a CVarWrapper by cvar name.")
        .def("getBindStringForKey", &CVarManagerWrapper::getBindStringForKey, py::arg("key"),
            "Get the bind string for a given key name.")
        .def("setBind", &CVarManagerWrapper::setBind, py::arg("key"), py::arg("command"),
            "Bind a command to a key.")
        .def("removeBind", &CVarManagerWrapper::removeBind, py::arg("key"),
            "Remove a key bind.")
        .def("getAlias", &CVarManagerWrapper::getAlias, py::arg("alias"),
            "Get an alias's script content.")
        .def("setAlias", &CVarManagerWrapper::setAlias, py::arg("key"), py::arg("script"),
            "Set an alias to run a script.")
        .def("backupCfg", &CVarManagerWrapper::backupCfg, py::arg("path"),
            "Backup the config file to the given path.")
        .def("backupBinds", &CVarManagerWrapper::backupBinds, py::arg("path"),
            "Backup binds to the given path.")
        .def("loadCfg", &CVarManagerWrapper::loadCfg, py::arg("path"),
            "Load a config file from the given path.");

    // ============================================================
    // CVarWrapper
    // ============================================================
    py::class_<CVarWrapper>(m, "CVarWrapper",
        R"doc(Console variable wrapper for reading and modifying registered cvars.

        Provides typed getters and setters, value change notifications,
        and variable binding.
        )doc")
        .def(py::init<std::uintptr_t, std::type_index>(), py::arg("memory_address"), py::arg("plugin_type_index"))
        .def("getCVarName", &CVarWrapper::getCVarName,
            "Get the cvar's name.")
        .def("getIntValue", &CVarWrapper::getIntValue,
            "Get the cvar's value as an integer.")
        .def("getFloatValue", &CVarWrapper::getFloatValue,
            "Get the cvar's value as a float.")
        .def("getBoolValue", &CVarWrapper::getBoolValue,
            "Get the cvar's value as a boolean.")
        .def("getColorValue", &CVarWrapper::getColorValue,
            R"doc(Get the cvar's value as a LinearColor.

            Supports formats: (R, G, B, A) with 0-255 range,
            or hex #123ABC / #1234ABCD.

            Returns:
                LinearColor: (0,0,0,0) if parsing fails.
            )doc")
        .def("getStringValue", &CVarWrapper::getStringValue,
            "Get the cvar's value as a string.")
        .def("getDescription", &CVarWrapper::getDescription,
            "Get the cvar's description.")
        .def("GetDefaultValue", &CVarWrapper::GetDefaultValue,
            "Get the cvar's default value.")
        .def("HasMinimum", &CVarWrapper::HasMinimum,
            "Check whether the cvar has a minimum value constraint.")
        .def("HasMaximum", &CVarWrapper::HasMaximum,
            "Check whether the cvar has a maximum value constraint.")
        .def("GetMinimum", &CVarWrapper::GetMinimum,
            "Get the cvar's minimum value (if HasMinimum).")
        .def("GetMaximum", &CVarWrapper::GetMaximum,
            "Get the cvar's maximum value (if HasMaximum).")
        .def("IsHidden", &CVarWrapper::IsHidden,
            "Check whether the cvar is hidden from UI.")
        .def("ShouldSaveToCfg", &CVarWrapper::ShouldSaveToCfg,
            "Check whether the cvar saves to config.")
        .def("ResetToDefault", &CVarWrapper::ResetToDefault,
            "Reset the cvar to its default value.")
        .def("notify", &CVarWrapper::notify,
            "Send a notification to all value-change listeners.")
        .def("setValue", py::overload_cast<std::string>(&CVarWrapper::setValue), py::arg("value"),
            "Set the cvar's value from a string.")
        .def("setValue", py::overload_cast<int>(&CVarWrapper::setValue), py::arg("value"),
            "Set the cvar's value from an integer.")
        .def("setValue", py::overload_cast<float>(&CVarWrapper::setValue), py::arg("value"),
            "Set the cvar's value from a float.")
        .def("setValue", py::overload_cast<LinearColor>(&CVarWrapper::setValue), py::arg("value"),
            "Set the cvar's value from a LinearColor (0-255 range).")
        .def("addOnValueChanged", &CVarWrapper::addOnValueChanged, py::arg("changeFunc"),
            R"doc(Register a callback for when the cvar value changes.

            Args:
                changeFunc: Callable taking (old_value_string, cvar_wrapper).
            )doc")
        .def("removeOnValueChanged", &CVarWrapper::removeOnValueChanged,
            "Remove the value-change callback.")
        .def("IsNull", &CVarWrapper::IsNull,
            "Check whether this cvar wrapper points to a valid cvar.")
        .def("__bool__", &CVarWrapper::operator bool);

    // ============================================================
    // MMRWrapper
    // ============================================================
    py::class_<MMRWrapper, ObjectWrapper>(m, "MMRWrapper",
        R"doc(Wrapper for matchmaking rating (MMR) and skill rating data.

        Provides access to player skill ratings, ranks, and MMR values
        across different playlists.
        )doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"))
        .def("IsSyncing", py::overload_cast<UniqueIDWrapper>(&MMRWrapper::IsSyncing),
            py::arg("playerID"), "Check if MMR data is syncing for a player.")
        .def("IsSynced", py::overload_cast<UniqueIDWrapper, int>(&MMRWrapper::IsSynced),
            py::arg("playerID"), py::arg("playlistID"),
            "Check if MMR data is synced for a player in a playlist.")
        .def("IsRanked", &MMRWrapper::IsRanked, py::arg("playlistID"),
            "Check if a playlist is ranked.")
        .def("GetPlayerSkillRating", py::overload_cast<UniqueIDWrapper, int>(&MMRWrapper::GetPlayerSkillRating),
            py::arg("playerID"), py::arg("playlistID"),
            "Get the skill rating (mu/sigma) for a player in a playlist.")
        .def("GetPlayerRank", py::overload_cast<UniqueIDWrapper, int>(&MMRWrapper::GetPlayerRank),
            py::arg("playerID"), py::arg("playlistID"),
            "Get the rank (tier/division) for a player in a playlist.")
        .def("GetPlayerMMR", py::overload_cast<UniqueIDWrapper, int>(&MMRWrapper::GetPlayerMMR),
            py::arg("playerID"), py::arg("playlistID"),
            "Get the MMR value for a player in a playlist.")
        .def("GetPlayerSkillRating", py::overload_cast<SteamID, int>(&MMRWrapper::GetPlayerSkillRating),
            py::arg("playerID"), py::arg("playlistID"))
        .def("GetPlayerRank", py::overload_cast<SteamID, int>(&MMRWrapper::GetPlayerRank),
            py::arg("playerID"), py::arg("playlistID"))
        .def("GetPlayerMMR", py::overload_cast<SteamID, int>(&MMRWrapper::GetPlayerMMR),
            py::arg("playerID"), py::arg("playlistID"))
        .def("CalculateMMR", &MMRWrapper::CalculateMMR, py::arg("sr"), py::arg("disregardPlacements"),
            "Calculate MMR from a SkillRating.")
        .def("GetCurrentPlaylist", &MMRWrapper::GetCurrentPlaylist,
            "Get the current playlist ID.")
        .def("RegisterMMRNotifier", &MMRWrapper::RegisterMMRNotifier, py::arg("callback"),
            R"doc(Subscribe to MMR update notifications.

            Args:
                callback: Callable taking a UniqueIDWrapper for the updated player.

            Returns:
                MMRNotifierToken: A token controlling the registration lifetime.
                    The callback is unregistered when the token is destroyed.
            )doc");

    // ============================================================
    // SettingsWrapper
    // ============================================================
    py::class_<SettingsWrapper>(m, "SettingsWrapper",
        R"doc(Wrapper for game settings including camera, gamepad, and video.)doc")
        .def(py::init<>())
        .def("GetCameraSettings", &SettingsWrapper::GetCameraSettings,
            "Get the player's camera settings.")
        .def("GetCameraSaveSettings", &SettingsWrapper::GetCameraSaveSettings,
            "Get the camera save settings.")
        .def("GetAllPCBindings", &SettingsWrapper::GetAllPCBindings,
            "Get all PC keyboard bindings.")
        .def("GetAllGamepadBindings", &SettingsWrapper::GetAllGamepadBindings,
            "Get all gamepad bindings.")
        .def("GetGamepadSettings", &SettingsWrapper::GetGamepadSettings,
            "Get the gamepad settings.")
        .def("GetVideoSettings", &SettingsWrapper::GetVideoSettings,
            "Get the video/graphics settings.")
        .def("GetForceDefaultColors", &SettingsWrapper::GetForceDefaultColors,
            "Check if force default colors is enabled.")
        .def("SetForceDefaultColors", &SettingsWrapper::SetForceDefaultColors, py::arg("value"),
            "Enable or disable force default colors.");

    // ============================================================
    // UniqueIDWrapper
    // ============================================================
    py::class_<UniqueIDWrapper>(m, "UniqueIDWrapper",
        R"doc(Cross-platform unique player identifier.

        Supports Steam, Epic, and other platform IDs. This wrapper can be
        safely stored for later use (unlike most other wrappers).
        )doc")
        .def(py::init<>())
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"))
        .def("GetPlatform", &UniqueIDWrapper::GetPlatform,
            "Get the player's platform (OnlinePlatform).")
        .def("GetEpicAccountID", &UniqueIDWrapper::GetEpicAccountID,
            "Get the Epic account ID string.")
        .def("GetUID", &UniqueIDWrapper::GetUID,
            "Get the raw unique ID value.")
        .def("GetSplitscreenID", &UniqueIDWrapper::GetSplitscreenID,
            "Get the splitscreen player index.")
        .def("str", &UniqueIDWrapper::str,
            "Get a string representation (EpicID if available, UID otherwise).")
        .def("GetIdString", &UniqueIDWrapper::GetIdString,
            "Get the ID in platform|accountId|splitscreenId format.")
        .def_static("FromEpicAccountID", &UniqueIDWrapper::FromEpicAccountID,
            py::arg("epicAccountID"), py::arg("uid"), py::arg("platform"),
            py::arg("splitscreenID") = (unsigned char)0,
            R"doc(Create a UniqueIDWrapper from an Epic account ID.

            Note: For some functionality you might need UID and platform too.
            Best to retrieve this from existing players when possible.
            )doc")
        .def("__lt__", &UniqueIDWrapper::operator<)
        .def("__eq__", &UniqueIDWrapper::operator==)
        .def("__ne__", &UniqueIDWrapper::operator!=)
        .def("__repr__", [](const UniqueIDWrapper& u) { return "UniqueIDWrapper(" + u.str() + ")"; });

    // ============================================================
    // PlayerControllerWrapper
    // ============================================================
    py::class_<PlayerControllerWrapper, ActorWrapper>(m, "PlayerControllerWrapper",
        R"doc(Wrapper for a player controller, providing input and player state access.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"))
        .def("GetCar", &PlayerControllerWrapper::GetCar)
        .def("SetCar", &PlayerControllerWrapper::SetCar, py::arg("newCar"))
        .def("GetPRI", &PlayerControllerWrapper::GetPRI)
        .def("SetPRI", &PlayerControllerWrapper::SetPRI, py::arg("newPRI"))
        .def("GetVehicleInput", &PlayerControllerWrapper::GetVehicleInput)
        .def("SetVehicleInput", &PlayerControllerWrapper::SetVehicleInput, py::arg("newVehicleInput"))
        .def("GetbUsingGamepad", &PlayerControllerWrapper::GetbUsingGamepad)
        .def("GetMouseSensitivity", &PlayerControllerWrapper::GetMouseSensitivity)
        .def("SetMouseSensitivity", &PlayerControllerWrapper::SetMouseSensitivity, py::arg("NewSensitivity"))
        .def("GetGameEvent", &PlayerControllerWrapper::GetGameEvent)
        .def("GetPrimaryPlayerController", &PlayerControllerWrapper::GetPrimaryPlayerController)
        .def("GetSpectatorHud", &PlayerControllerWrapper::GetSpectatorHud);

    // ============================================================
    // ControllerWrapper & AIControllerWrapper
    // ============================================================
    py::class_<ControllerWrapper, ActorWrapper>(m, "ControllerWrapper",
        R"doc(Base wrapper for controller actors.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<AIControllerWrapper, ControllerWrapper>(m, "AIControllerWrapper",
        R"doc(Wrapper for AI controllers.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    // ============================================================
    // HUD chain (needed for SpectatorHUDWrapper base)
    // ============================================================
    py::class_<HudWrapper, ActorWrapper>(m, "HudWrapper",
        R"doc(Base class for HUD wrappers.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<HUDXWrapper, HudWrapper>(m, "HUDXWrapper",
        R"doc(Extended HUD wrapper.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<HUDTABaseWrapper, HUDXWrapper>(m, "HUDTABaseWrapper",
        R"doc(Base tower attack HUD wrapper.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<HUDTAWrapper, HUDTABaseWrapper>(m, "HUDTAWrapper",
        R"doc(Tower attack HUD wrapper.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    // ============================================================
    // MatchmakingWrapper
    // ============================================================
    py::class_<MatchmakingWrapper, ObjectWrapper>(m, "MatchmakingWrapper",
        R"doc(Wrapper for matchmaking state.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    // ============================================================
    // PluginManagerWrapper
    // ============================================================
    py::class_<PluginManagerWrapper, ObjectWrapper>(m, "PluginManagerWrapper",
        R"doc(Wrapper for the plugin management system.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    // ============================================================
    // GuiManagerWrapper
    // ============================================================
    py::class_<GuiManagerWrapper, ObjectWrapper>(m, "GuiManagerWrapper",
        R"doc(Wrapper for the GUI management system.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    // ============================================================
    // MenuStackWrapper
    // ============================================================
    py::class_<MenuStackWrapper, ObjectWrapper>(m, "MenuStackWrapper",
        R"doc(Wrapper for the menu stack.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    // ============================================================
    // SpectatorHUDWrapper
    // ============================================================
    py::class_<SpectatorHUDWrapper, HUDTAWrapper>(m, "SpectatorHUDWrapper",
        R"doc(Wrapper for the spectator HUD.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    // ============================================================
    // ReplayServerWrapper
    // ============================================================
    py::class_<ReplayServerWrapper, ServerWrapper>(m, "ReplayServerWrapper",
        R"doc(Wrapper for replay server game events.)doc")
        .def(py::init<std::uintptr_t, std::uintptr_t, std::uintptr_t>(),
            py::arg("server"), py::arg("gameinfo"), py::arg("replaydirector"));

    // ============================================================
    // ReplayViewerDataWrapper
    // ============================================================
    py::class_<ReplayViewerDataWrapper, ObjectWrapper>(m, "ReplayViewerDataWrapper",
        R"doc(Wrapper for replay viewer data.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    // ============================================================
    // CareerStatsWrapper
    // ============================================================
    py::class_<CareerStatsWrapper>(m, "CareerStatsWrapper",
        R"doc(Wrapper for career/player statistics.)doc")
        .def(py::init<>());

    // ============================================================
    // ReplayManagerWrapper
    // ============================================================
    py::class_<ReplayManagerWrapper, ObjectWrapper>(m, "ReplayManagerWrapper",
        R"doc(Wrapper for replay file management.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    // ============================================================
    // StatEventWrapper
    // ============================================================
    py::class_<StatEventWrapper, ObjectWrapper>(m, "StatEventWrapper",
        R"doc(Wrapper for stat event data.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    // ============================================================
    // EnumWrapper
    // ============================================================
    py::class_<EnumWrapper, ObjectWrapper>(m, "EnumWrapper",
        R"doc(Wrapper for runtime enum introspection (30+ game enums).)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    // ============================================================
    // Sequence Wrappers (Kismet)
    // ============================================================
    py::class_<SequenceWrapper, ObjectWrapper>(m, "SequenceWrapper",
        R"doc(Wrapper for a Kismet sequence.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<SequenceObjectWrapper, ObjectWrapper>(m, "SequenceObjectWrapper",
        R"doc(Wrapper for a Kismet sequence object.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<SequenceVariableWrapper, ObjectWrapper>(m, "SequenceVariableWrapper",
        R"doc(Wrapper for a Kismet sequence variable.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<SequenceOpWrapper, SequenceObjectWrapper>(m, "SequenceOpWrapper",
        R"doc(Wrapper for a Kismet sequence operation.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    // ============================================================
    // Performance Stats Wrappers
    // ============================================================
    py::class_<SampleHistoryWrapper, ObjectWrapper>(m, "SampleHistoryWrapper",
        R"doc(Wrapper for performance sample history.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<StatGraphWrapper, ObjectWrapper>(m, "StatGraphWrapper",
        R"doc(Wrapper for stat graph data.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<StatGraphSystemWrapper, ObjectWrapper>(m, "StatGraphSystemWrapper",
        R"doc(Wrapper for the stat graph system.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<StartGraphSystemWrapper, ObjectWrapper>(m, "StartGraphSystemWrapper",
        R"doc(Wrapper for the start graph system.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<InputBufferGraphWrapper, ObjectWrapper>(m, "InputBufferGraphWrapper",
        R"doc(Wrapper for input buffer graph data.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<NetStatGraphWrapper, ObjectWrapper>(m, "NetStatGraphWrapper",
        R"doc(Wrapper for network stat graph data.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<PerfStatGraphWrapper, ObjectWrapper>(m, "PerfStatGraphWrapper",
        R"doc(Wrapper for performance stat graph data.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<SampleRecordSettingsWrapper, ObjectWrapper>(m, "SampleRecordSettingsWrapper",
        R"doc(Wrapper for sample record settings.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    // ============================================================
    // Mesh Component Wrappers
    // ============================================================
    py::class_<MeshComponentWrapper, ObjectWrapper>(m, "MeshComponentWrapper",
        R"doc(Base wrapper for mesh components.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<CarMeshComponentBaseWrapper, MeshComponentWrapper>(m, "CarMeshComponentBaseWrapper",
        R"doc(Base wrapper for car mesh components.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<SkeletalMeshComponentWrapper, MeshComponentWrapper>(m, "SkeletalMeshComponentWrapper",
        R"doc(Wrapper for skeletal mesh components.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));
}

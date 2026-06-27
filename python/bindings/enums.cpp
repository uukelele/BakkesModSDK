#include <pybind11/pybind11.h>
#include <bakkesmod/plugin/bakkesmodsdk.h>
#include <bakkesmod/wrappers/wrapperstructs.h>
#include <bakkesmod/core/loadout_structs.h>

namespace py = pybind11;

void init_enums(py::module& m) {
    // ============================================================
    // PLUGINTYPE
    // ============================================================
    py::enum_<PLUGINTYPE>(m, "PLUGINTYPE", py::arithmetic(),
        R"doc(Bitmask flags defining when a plugin is active.

        Combine multiple types with bitwise OR (|) to have the plugin
        active in multiple game modes.
        )doc")
        .value("FREEPLAY", PLUGINTYPE_FREEPLAY,
            "Active during freeplay/training.")
        .value("CUSTOM_TRAINING", PLUGINTYPE_CUSTOM_TRAINING,
            "Active during custom training.")
        .value("SPECTATOR", PLUGINTYPE_SPECTATOR,
            "Active while spectating.")
        .value("BOTAI", PLUGINTYPE_BOTAI,
            "Active for AI/bot matches.")
        .value("REPLAY", PLUGINTYPE_REPLAY,
            "Active during replay viewing.")
        .value("THREADED", PLUGINTYPE_THREADED,
            "Allow callbacks from background threads.")
        .value("THREADEDUNLOAD", PLUGINTYPE_THREADEDUNLOAD,
            "Allow unloading from background threads.")
        .export_values();

    // ============================================================
    // NOTIFIER_PERMISSION
    // ============================================================
    py::enum_<NOTIFIER_PERMISSION>(m, "NOTIFIER_PERMISSION", py::arithmetic(),
        R"doc(Flags for restricting notifier execution to specific game states.

        Combine multiple permissions with bitwise OR (|).
        )doc")
        .value("ALL", PERMISSION_ALL, "No restriction.")
        .value("MENU", PERMISSION_MENU, "Main menu.")
        .value("SOCCAR", PERMISSION_SOCCAR, "In a soccar match.")
        .value("FREEPLAY", PERMISSION_FREEPLAY, "In freeplay.")
        .value("CUSTOM_TRAINING", PERMISSION_CUSTOM_TRAINING, "In custom training.")
        .value("ONLINE", PERMISSION_ONLINE, "In an online match.")
        .value("PAUSEMENU_CLOSED", PERMISSION_PAUSEMENU_CLOSED, "Pause menu is closed.")
        .value("REPLAY", PERMISSION_REPLAY, "In a replay.")
        .value("OFFLINE", PERMISSION_OFFLINE, "Only when offline.")
        .export_values();

    // ============================================================
    // ToastType
    // ============================================================
    py::enum_<ToastType>(m, "ToastType",
        R"doc(Toast notification icon/types.)doc")
        .value("Info", ToastType_Info, "Informational toast.")
        .value("OK", ToastType_OK, "Success toast.")
        .value("Warning", ToastType_Warning, "Warning toast.")
        .value("Error", ToastType_Error, "Error toast.")
        .export_values();

    // ============================================================
    // TRADEHOLD
    // ============================================================
    py::enum_<TRADEHOLD>(m, "TRADEHOLD",
        R"doc(Trade hold restriction types for items.)doc")
        .value("P2P", TRADEHOLD_P2P, "Peer-to-peer trade hold.")
        .value("ALL", TRADEHOLD_ALL, "All trades on hold.")
        .value("NONE", TRADEHOLD_NONE, "No trade hold.")
        .export_values();

    // ============================================================
    // PRODUCTQUALITY (deprecated)
    // ============================================================
    py::enum_<PRODUCTQUALITY>(m, "PRODUCTQUALITY",
        R"doc(Item quality/rarity tiers.

        Note: Deprecated - use EnumWrapper::GetProductQualities() instead.
        )doc")
        .value("Common", Common)
        .value("Uncommon", Uncommon)
        .value("Rare", Rare)
        .value("VeryRare", VeryRare)
        .value("Import", Import)
        .value("Exotic", Exotic)
        .value("BlackMarket", BlackMarket)
        .value("Premium", Premium)
        .value("Limited", Limited)
        .value("Legacy", Legacy)
        .value("MAX", MAX)
        .export_values();

    // ============================================================
    // UNLOCKMETHOD (deprecated)
    // ============================================================
    py::enum_<UNLOCKMETHOD>(m, "UNLOCKMETHOD",
        R"doc(Item unlock method types.

        Note: Deprecated - use EnumWrapper::GetUnlockMethods() instead.
        )doc")
        .value("Default", Default)
        .value("Drop", Drop)
        .value("Special", Special)
        .value("Reward", Reward)
        .value("DLC", DLC)
        .value("Never", Never)
        .export_values();

    // ============================================================
    // CARBODY (deprecated)
    // ============================================================
    py::enum_<CARBODY>(m, "CARBODY",
        R"doc(Pre-defined car body IDs.

        Note: Deprecated - use ProductWrapper for up-to-date car body IDs.
        )doc")
        .value("BACKFIRE", CAR_BACKFIRE)
        .value("BREAKOUT", CAR_BREAKOUT)
        .value("BREAKOUTTYPES", CAR_BREAKOUTTYPES)
        .value("OCTANE", CAR_OCTANE)
        .value("OCTANEZSR", CAR_OCTANEZSR)
        .value("PALADIN", CAR_PALADIN)
        .value("ROADHOG", CAR_ROADHOG)
        .value("ROADHOGXL", CAR_ROADHOGXL)
        .value("GIZMO", CAR_GIZMO)
        .value("SWEETTOOTH", CAR_SWEETTOOTH)
        .value("XDEVIL", CAR_XDEVIL)
        .value("XDEVILMK2", CAR_XDEVILMK2)
        .value("HOTSHOT", CAR_HOTSHOT)
        .value("MERC", CAR_MERC)
        .value("VENOM", CAR_VENOM)
        .value("TAKUMI", CAR_TAKUMI)
        .value("TAKUMIRXT", CAR_TAKUMIRXT)
        .value("DOMINUS", CAR_DOMINUS)
        .value("DOMINUSGT", CAR_DOMINUSGT)
        .value("SCARAB", CAR_SCARAB)
        .value("ZIPPY", CAR_ZIPPY)
        .value("DELOREAN", CAR_DELOREAN)
        .value("RIPPER", CAR_RIPPER)
        .value("GROG", CAR_GROG)
        .value("ARMADILLO", CAR_ARMADILLO)
        .value("WARTHOG", CAR_WARTHOG)
        .value("BATMOBILE", CAR_BATMOBILE)
        .value("MASAMUNE", CAR_MASAMUNE)
        .value("MARAUDER", CAR_MARAUDER)
        .value("AFTERSHOCK", CAR_AFTERSHOCK)
        .value("ESPER", CAR_ESPER)
        .value("PROTEUS", CAR_PROTEUS)
        .value("TRITON", CAR_TRITON)
        .value("VULCAN", CAR_VULCAN)
        .value("TWINMILL", CAR_TWINMILL)
        .value("BONESHAKER", CAR_BONESHAKER)
        .value("ENDO", CAR_ENDO)
        .value("ICECHARGER", CAR_ICECHARGER)
        .value("MANTIS", CAR_MANTIS)
        .value("JOGER619RS", CAR_JOGER619RS)
        .value("CENTIO", CAR_CENTIO)
        .value("ANIMUSGP", CAR_ANIMUSGP)
        .export_values();

    // ============================================================
    // OnlinePlatform
    // ============================================================
    py::enum_<OnlinePlatform>(m, "OnlinePlatform",
        R"doc(Online platform identifiers for cross-platform identification.)doc")
        .value("Unknown", OnlinePlatform_Unknown)
        .value("Steam", OnlinePlatform_Steam)
        .value("PS4", OnlinePlatform_PS4)
        .value("PS3", OnlinePlatform_PS3)
        .value("Dingo", OnlinePlatform_Dingo)
        .value("QQ", OnlinePlatform_QQ)
        .value("OldNNX", OnlinePlatform_OldNNX)
        .value("NNX", OnlinePlatform_NNX)
        .value("PsyNet", OnlinePlatform_PsyNet)
        .value("Deleted", OnlinePlatform_Deleted)
        .value("WeGame", OnlinePlatform_WeGame)
        .value("Epic", OnlinePlatform_Epic)
        .value("MAX", OnlinePlatform_MAX)
        .export_values();

    // ============================================================
    // pluginsdk::Itempaint
    // ============================================================
    py::enum_<pluginsdk::Itempaint>(m, "Itempaint",
        R"doc(Item paint finish types.)doc")
        .value("NONE", pluginsdk::Itempaint::NONE)
        .value("CRIMSON", pluginsdk::Itempaint::CRIMSON)
        .value("LIME", pluginsdk::Itempaint::LIME)
        .value("BLACK", pluginsdk::Itempaint::BLACK)
        .value("SKYBLUE", pluginsdk::Itempaint::SKYBLUE)
        .value("COBALT", pluginsdk::Itempaint::COBALT)
        .value("BURNTSIENNA", pluginsdk::Itempaint::BURNTSIENNA)
        .value("FORESTGREEN", pluginsdk::Itempaint::FORESTGREEN)
        .value("PURPLE", pluginsdk::Itempaint::PURPLE)
        .value("PINK", pluginsdk::Itempaint::PINK)
        .value("ORANGE", pluginsdk::Itempaint::ORANGE)
        .value("GREY", pluginsdk::Itempaint::GREY)
        .value("TITANIUMWHITE", pluginsdk::Itempaint::TITANIUMWHITE)
        .value("SAFFRON", pluginsdk::Itempaint::SAFFRON)
        .value("GOLD", pluginsdk::Itempaint::GOLD)
        .value("ROSEGOLD", pluginsdk::Itempaint::ROSEGOLD)
        .value("WHITEGOLD", pluginsdk::Itempaint::WHITEGOLD)
        .value("ONYX", pluginsdk::Itempaint::ONYX)
        .value("PLATINUM", pluginsdk::Itempaint::PLATINUM)
        .export_values();

    // ============================================================
    // pluginsdk::Equipslot
    // ============================================================
    py::enum_<pluginsdk::Equipslot>(m, "Equipslot",
        R"doc(Equipment slot types for item loadouts.)doc")
        .value("BODY", pluginsdk::Equipslot::BODY)
        .value("DECAL", pluginsdk::Equipslot::DECAL)
        .value("WHEELS", pluginsdk::Equipslot::WHEELS)
        .value("ROCKETBOOST", pluginsdk::Equipslot::ROCKETBOOST)
        .value("ANTENNA", pluginsdk::Equipslot::ANTENNA)
        .value("TOPPER", pluginsdk::Equipslot::TOPPER)
        .value("BUMPER", pluginsdk::Equipslot::BUMPER)
        .value("PAINTFINISH", pluginsdk::Equipslot::PAINTFINISH)
        .value("BOT", pluginsdk::Equipslot::BOT)
        .value("LOGO", pluginsdk::Equipslot::LOGO)
        .value("UNDERGLOW", pluginsdk::Equipslot::UNDERGLOW)
        .value("CRATES", pluginsdk::Equipslot::CRATES)
        .value("CUSTOMFINISH", pluginsdk::Equipslot::CUSTOMFINISH)
        .value("ENGINEAUDIO", pluginsdk::Equipslot::ENGINEAUDIO)
        .value("TRAIL", pluginsdk::Equipslot::TRAIL)
        .value("GOALEXPLOSION", pluginsdk::Equipslot::GOALEXPLOSION)
        .value("PLAYERBANNER", pluginsdk::Equipslot::PLAYERBANNER)
        .value("GARAGECOMPLEXROW", pluginsdk::Equipslot::GARAGECOMPLEXROW)
        .value("GOALSTINGER", pluginsdk::Equipslot::GOALSTINGER)
        .value("PLAYERAVATAR", pluginsdk::Equipslot::PLAYERAVATAR)
        .value("AVATARBORDER", pluginsdk::Equipslot::AVATARBORDER)
        .value("PLAYERTITLE", pluginsdk::Equipslot::PLAYERTITLE)
        .value("ESPORTSTEAM", pluginsdk::Equipslot::ESPORTSTEAM)
        .value("ARCHIVEDITEMS", pluginsdk::Equipslot::ARCHIVEDITEMS)
        .value("BLUEPRINTS", pluginsdk::Equipslot::BLUEPRINTS)
        .value("SHOPITEM", pluginsdk::Equipslot::SHOPITEM)
        .value("CURRENCY", pluginsdk::Equipslot::CURRENCY)
        .value("MAX", pluginsdk::Equipslot::MAX)
        .export_values();
}

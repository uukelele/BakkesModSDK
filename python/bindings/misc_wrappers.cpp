#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <bakkesmod/wrappers/modals/ModalWrapper.h>
#include <bakkesmod/wrappers/modals/TextInputModalWrapper.h>
#include <bakkesmod/wrappers/http/HttpWrapper.h>
#include <bakkesmod/wrappers/ImageWrapper.h>
#include <bakkesmod/wrappers/gfx/GfxDataTrainingWrapper.h>
#include <bakkesmod/wrappers/arraywrapper.h>
#include <bakkesmod/wrappers/StructArrayWrapper.h>
#include <bakkesmod/wrappers/GameObject/RumbleComponent/RumblePickupComponentWrapper.h>
#include <bakkesmod/wrappers/GameObject/RumbleComponent/AttachmentPickup.h>
#include <bakkesmod/wrappers/GameObject/RumbleComponent/BallCarSpringPickup.h>
#include <bakkesmod/wrappers/GameObject/RumbleComponent/BallFreezePickup.h>
#include <bakkesmod/wrappers/GameObject/RumbleComponent/BallLassoPickup.h>
#include <bakkesmod/wrappers/GameObject/RumbleComponent/BattarangPickup.h>
#include <bakkesmod/wrappers/GameObject/RumbleComponent/BoostModPickup.h>
#include <bakkesmod/wrappers/GameObject/RumbleComponent/BoostOverridePickup.h>
#include <bakkesmod/wrappers/GameObject/RumbleComponent/CarSpeedPickup.h>
#include <bakkesmod/wrappers/GameObject/RumbleComponent/DemolishPickup.h>
#include <bakkesmod/wrappers/GameObject/RumbleComponent/GrapplingHookPickup.h>
#include <bakkesmod/wrappers/GameObject/RumbleComponent/GravityPickup.h>
#include <bakkesmod/wrappers/GameObject/RumbleComponent/HandbrakeOverridePickup.h>
#include <bakkesmod/wrappers/GameObject/RumbleComponent/HitForcePickup.h>
#include <bakkesmod/wrappers/GameObject/RumbleComponent/SpringPickup.h>
#include <bakkesmod/wrappers/GameObject/RumbleComponent/SwapperPickup.h>
#include <bakkesmod/wrappers/GameObject/RumbleComponent/TargetedPickup.h>
#include <bakkesmod/wrappers/GameObject/RumbleComponent/TimeBombPickup.h>
#include <bakkesmod/wrappers/GameObject/RumbleComponent/TornadoPickup.h>
#include <bakkesmod/wrappers/GameObject/RumbleComponent/VelcroPickup.h>

namespace py = pybind11;

void init_misc_wrappers(py::module& m) {
    // ============================================================
    // ModalWrapper
    // ============================================================
    py::class_<ModalWrapper, ObjectWrapper>(m, "ModalWrapper",
        R"doc(Wrapper for creating and managing modal dialogs.

        Modals can display custom HTML body content, icons, colors,
        and buttons with callbacks.
        )doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"))
        .def("ShowModal", &ModalWrapper::ShowModal,
            "Display the modal to the user.")
        .def("CloseModal", &ModalWrapper::CloseModal,
            "Close the modal.")
        .def("SetColor", &ModalWrapper::SetColor, py::arg("r"), py::arg("g"), py::arg("b"),
            "Set the modal background color (0-255).")
        .def("SetIcon", &ModalWrapper::SetIcon, py::arg("iconName"),
            "Set the modal icon by name.")
        .def("SetBody", &ModalWrapper::SetBody, py::arg("bodyHtml"),
            "Set the modal body content as HTML.")
        .def("AddButton", &ModalWrapper::AddButton, py::arg("button_text"),
            py::arg("is_cancel_button"), py::arg("click_callback") = nullptr,
            R"doc(Add a button to the modal.

            Args:
                button_text: The button label text.
                is_cancel_button: Whether this is a cancel/dismiss button.
                click_callback: Optional callback when the button is clicked.
            )doc");

    // ============================================================
    // TextInputModalWrapper
    // ============================================================
    py::class_<TextInputModalWrapper, ModalWrapper>(m, "TextInputModalWrapper",
        R"doc(Wrapper for creating text input modal dialogs.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    // ============================================================
    // HttpWrapper
    // ============================================================
    py::class_<HttpWrapper>(m, "HttpWrapper",
        R"doc(Static HTTP request wrapper for making web requests.

        Supports string, binary, and file-output responses via curl.
        Callbacks may run on background threads - wrap SDK calls in
        GameWrapper.Execute().
        )doc")
        .def_static("SendCurlRequest",
            py::overload_cast<CurlRequest, CurlRequestDoneStringReturn>(&HttpWrapper::SendCurlRequest),
            py::arg("request_data"), py::arg("on_complete"),
            py::call_guard<py::gil_scoped_release>(),
            R"doc(Send an HTTP request and get the response as a string.

            Args:
                request_data: The request configuration.
                on_complete: Callback taking (http_code, response_body_string).
                    Returns 418 on error.
            )doc")
        .def_static("SendCurlRequest",
            py::overload_cast<CurlRequest, CurlRequestDoneBinaryReturn>(&HttpWrapper::SendCurlRequest),
            py::arg("request_data"), py::arg("on_complete"),
            py::call_guard<py::gil_scoped_release>(),
            R"doc(Send an HTTP request and get the raw binary data.

            Args:
                request_data: The request configuration.
                on_complete: Callback taking (http_code, raw_data_pointer).
                    Data is only valid inside the callback - copy if needed later.
                    Returns 418 on error.
            )doc")
        .def_static("SendCurlRequest",
            py::overload_cast<CurlRequest, const std::wstring&, CurlRequestDoneFileReturn>(&HttpWrapper::SendCurlRequest),
            py::arg("request_data"), py::arg("file_output"), py::arg("on_complete"),
            py::call_guard<py::gil_scoped_release>(),
            R"doc(Send an HTTP request and save the response to a file.

            Args:
                request_data: The request configuration.
                file_output: Path where the response will be saved.
                on_complete: Callback taking (http_code, file_path).
                    Returns 418 on error.
            )doc")
        .def_static("SendCurlJsonRequest",
            &HttpWrapper::SendCurlJsonRequest,
            py::arg("request_data"), py::arg("on_complete"),
            py::call_guard<py::gil_scoped_release>(),
            R"doc(Send an HTTP JSON request with auto-set Content-Type and Accept headers.

            Args:
                request_data: The request configuration.
                on_complete: Callback taking (http_code, response_body_string).
                    Returns 418 on error.
            )doc");

    // ============================================================
    // ImageWrapper
    // ============================================================
    py::class_<ImageWrapper>(m, "ImageWrapper",
        R"doc(Wrapper for loading and using images in rendering.

        Supports loading textures for both Canvas and ImGui rendering.
        Do not make copies of this object.
        )doc")
        .def(py::init<std::string, bool, bool>(),
            py::arg("path"), py::arg("canvasLoad") = false, py::arg("imGuiLoad") = false,
            "Load an image from the given file path.")
        .def(py::init<std::wstring, bool, bool>(),
            py::arg("path"), py::arg("canvasLoad") = false, py::arg("imGuiLoad") = false,
            "Load an image from the given wide-string file path.")
        .def("LoadForCanvas", &ImageWrapper::LoadForCanvas,
            "Load the image for Canvas rendering. Returns True on success.")
        .def("IsLoadedForCanvas", &ImageWrapper::IsLoadedForCanvas,
            "Check if the image is loaded for Canvas rendering.")
        .def("GetCanvasTex", &ImageWrapper::GetCanvasTex,
            "Get the Canvas texture pointer. Loads if not already loaded.")
        .def("LoadForImGui", &ImageWrapper::LoadForImGui, py::arg("onLoaded"),
            "Load the image for ImGui rendering with a completion callback.")
        .def("IsLoadedForImGui", &ImageWrapper::IsLoadedForImGui,
            "Check if the image is loaded for ImGui rendering.")
        .def("GetImGuiTex", &ImageWrapper::GetImGuiTex,
            "Get the ImGui texture pointer. Loads if not already loaded.")
        .def("GetPath", &ImageWrapper::GetPath,
            "Get the image file path.")
        .def("GetPathW", &ImageWrapper::GetPathW,
            "Get the image file path (wide string).")
        .def("GetSize", &ImageWrapper::GetSize,
            "Get the image dimensions in pixels.")
        .def("GetSizeF", &ImageWrapper::GetSizeF,
            "Get the image dimensions as float values.");

    // ============================================================
    // GfxDataTrainingWrapper
    // ============================================================
    py::class_<GfxDataTrainingWrapper, ObjectWrapper>(m, "GfxDataTrainingWrapper",
        R"doc(Wrapper for training GFX data display.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"))
        .def("IsNull", &GfxDataTrainingWrapper::IsNull,
            "Check if this wrapper is null.")

        // Capabilities
        .def("CanUseMirroring", &GfxDataTrainingWrapper::CanUseMirroring,
            "Check if mirroring is available for the current training.")
        .def("CanUsePlaylistOverride", &GfxDataTrainingWrapper::CanUsePlaylistOverride,
            "Check if playlist override is available.")
        .def("CanUseShuffle", &GfxDataTrainingWrapper::CanUseShuffle,
            "Check if shuffle mode is available.")
        .def("CanUseTrainingUIV2", &GfxDataTrainingWrapper::CanUseTrainingUIV2,
            "Check if training UI v2 features are available.")

        // Training completion
        .def("HasCompletedTraining", &GfxDataTrainingWrapper::HasCompletedTraining,
            "Check if a specific training has been completed.",
            py::arg("inType"), py::arg("inDifficulty"))
        .def("HasPerfectedTraining", &GfxDataTrainingWrapper::HasPerfectedTraining,
            "Check if a specific training has been perfected (all stars).",
            py::arg("inType"), py::arg("inDifficulty"))
        .def("HasCompletedTutorial", &GfxDataTrainingWrapper::HasCompletedTutorial,
            "Check if a tutorial type has been completed.", py::arg("InType"))

        // Launch training
        .def("PlayFreeplayMap", &GfxDataTrainingWrapper::PlayFreeplayMap,
            "Play a freeplay map by name.", py::arg("Map"))
        .def("PlayTraining", &GfxDataTrainingWrapper::PlayTraining,
            "Play a training pack by type and difficulty.",
            py::arg("Type"), py::arg("trainingDifficulty"))
        .def("PlayTutorial", &GfxDataTrainingWrapper::PlayTutorial,
            "Play a tutorial.", py::arg("bAdvanced"))
        .def("PlayV2Tutorial", &GfxDataTrainingWrapper::PlayV2Tutorial,
            "Play a v2 tutorial.", py::arg("type"), py::arg("bAutoEntered"))

        // State queries
        .def("GetTrainingType", &GfxDataTrainingWrapper::GetTrainingType,
            "Get the current training type string.")
        .def("GetCurrentScore", &GfxDataTrainingWrapper::GetCurrentScore,
            "Get the current training score.")
        .def("GetCurrentRound", &GfxDataTrainingWrapper::GetCurrentRound,
            "Get the current round number.")
        .def("GetTotalRounds", &GfxDataTrainingWrapper::GetTotalRounds,
            "Get the total number of rounds.")
        .def("GetDifficulty", &GfxDataTrainingWrapper::GetDifficulty,
            "Get the training difficulty as a string wrapper.")
        .def("GetSelectedFreeplayMap", &GfxDataTrainingWrapper::GetSelectedFreeplayMap,
            "Get the currently selected freeplay map name.")
        .def("GetTutorialTip", &GfxDataTrainingWrapper::GetTutorialTip,
            "Get the tutorial tip text.")
        .def("GetCurrentPlaylistindex", &GfxDataTrainingWrapper::GetCurrentPlaylistindex,
            "Get the current playlist index.")
        .def("GetProgressPerShot", &GfxDataTrainingWrapper::GetProgressPerShot,
            "Get progress data per shot (StructArrayWrapper of TrainingRoundProgress).")

        // Actions
        .def("ResetTrainingProgress", &GfxDataTrainingWrapper::ResetTrainingProgress,
            "Reset all training progress.")
        .def("EndTrainingEditorPlaytest", &GfxDataTrainingWrapper::EndTrainingEditorPlaytest,
            "End the training editor playtest.")
        .def("UpdateDataFromProgress", &GfxDataTrainingWrapper::UpdateDataFromProgress,
            "Update GFX data from training progress.")
        .def("SetCurrentPlaylist", &GfxDataTrainingWrapper::SetCurrentPlaylist,
            "Set the current playlist.", py::arg("playlist"))
        .def("SetTrainingEditorGameEvent", &GfxDataTrainingWrapper::SetTrainingEditorGameEvent,
            "Set the training editor game event.", py::arg("inGameEvent"))
        .def("SetTrainingGameEvent", &GfxDataTrainingWrapper::SetTrainingGameEvent,
            "Set the training game event.", py::arg("inGameEvent"))
        .def("StartAtPlaylistIndex", &GfxDataTrainingWrapper::StartAtPlaylistIndex,
            "Start at a given playlist index.", py::arg("inPlaylistIndex"));

    // ============================================================
    // Rumble Pickup Subclasses
    // ============================================================
    py::class_<AttachmentPickup, RumblePickupComponentWrapper>(m, "AttachmentPickup",
        "Rumble pickup: attaches the ball to the car.")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<BallCarSpringPickup, RumblePickupComponentWrapper>(m, "BallCarSpringPickup",
        "Rumble pickup: spring between ball and car.")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<BallFreezePickup, RumblePickupComponentWrapper>(m, "BallFreezePickup",
        "Rumble pickup: freezes the ball.")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<BallLassoPickup, RumblePickupComponentWrapper>(m, "BallLassoPickup",
        "Rumble pickup: lasso the ball.")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<BattarangPickup, RumblePickupComponentWrapper>(m, "BattarangPickup",
        "Rumble pickup: battarang projectile.")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<BoostModPickup, RumblePickupComponentWrapper>(m, "BoostModPickup",
        "Rumble pickup: boost modifier.")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<BoostOverridePickup, RumblePickupComponentWrapper>(m, "BoostOverridePickup",
        "Rumble pickup: boost override.")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<CarSpeedPickup, RumblePickupComponentWrapper>(m, "CarSpeedPickup",
        "Rumble pickup: car speed modifier.")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<DemolishPickup, RumblePickupComponentWrapper>(m, "DemolishPickup",
        "Rumble pickup: demolish opponent.")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<GrapplingHookPickup, RumblePickupComponentWrapper>(m, "GrapplingHookPickup",
        "Rumble pickup: grappling hook.")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<GravityPickup, RumblePickupComponentWrapper>(m, "GravityPickup",
        "Rumble pickup: gravity field.")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<HandbrakeOverridePickup, RumblePickupComponentWrapper>(m, "HandbrakeOverridePickup",
        "Rumble pickup: handbrake override.")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<HitForcePickup, RumblePickupComponentWrapper>(m, "HitForcePickup",
        "Rumble pickup: hit force modifier.")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<SpringPickup, RumblePickupComponentWrapper>(m, "SpringPickup",
        "Rumble pickup: spring effect.")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<SwapperPickup, RumblePickupComponentWrapper>(m, "SwapperPickup",
        "Rumble pickup: swap positions with opponent.")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<TargetedPickup, RumblePickupComponentWrapper>(m, "TargetedPickup",
        "Rumble pickup: targeted ability.")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<TimeBombPickup, RumblePickupComponentWrapper>(m, "TimeBombPickup",
        "Rumble pickup: time bomb.")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<TornadoPickup, RumblePickupComponentWrapper>(m, "TornadoPickup",
        "Rumble pickup: tornado effect.")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<VelcroPickup, RumblePickupComponentWrapper>(m, "VelcroPickup",
        "Rumble pickup: velcro (ball sticks to car).")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    // ============================================================
    // ViewTarget
    // ============================================================
    py::class_<ViewTarget>(m, "ViewTarget",
        R"doc(View target data containing camera POV and target actor info.)doc")
        .def(py::init<>())
        .def_readwrite("Target", &ViewTarget::Target)
        .def_readwrite("Controller", &ViewTarget::Controller)
        .def_readwrite("POV", &ViewTarget::POV)
        .def_readwrite("AspectRatio", &ViewTarget::AspectRatio)
        .def_readwrite("PRI", &ViewTarget::PRI);
}

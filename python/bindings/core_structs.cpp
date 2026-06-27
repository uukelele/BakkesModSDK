#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <bakkesmod/wrappers/wrapperstructs.h>
#include <bakkesmod/core/bot_loadout_structs.h>
#include <bakkesmod/core/http_structs.h>

namespace py = pybind11;

void init_core_structs(py::module& m) {
    // ============================================================
    // CONSTANTS
    // ============================================================
    m.attr("BAKKESMOD_PLUGIN_API_VERSION") = py::int_(95);
    m.attr("CONST_RadToUnrRot") = py::float_(CONST_RadToUnrRot);
    m.attr("CONST_UnrRotToRad") = py::float_(CONST_UnrRotToRad);
    m.attr("CONST_DegToUnrRot") = py::float_(CONST_DegToUnrRot);
    m.attr("CONST_PI_F") = py::float_(CONST_PI_F);
    m.attr("CONST_PI_D") = py::float_(CONST_PI_D);

    // ============================================================
    // Vector
    // ============================================================
    py::class_<Vector>(m, "Vector",
        R"doc(Represents a 3D vector with X, Y, Z float components.

        Provides standard vector math operations including magnitude,
        normalization, dot product, cross product, lerp, and slerp.
        )doc")
        .def(py::init<>(), "Construct a zero vector (0, 0, 0).")
        .def(py::init<float, float, float>(), py::arg("x"), py::arg("y"), py::arg("z"),
            "Construct a vector from X, Y, Z components.")
        .def(py::init<float>(), py::arg("def"),
            "Construct a vector with all components set to the same value.")
        .def_readwrite("X", &Vector::X, "The X component.")
        .def_readwrite("Y", &Vector::Y, "The Y component.")
        .def_readwrite("Z", &Vector::Z, "The Z component.")
        .def("magnitude", &Vector::magnitude,
            R"doc(Calculate the magnitude (length) of the vector.

            Returns:
                float: The magnitude sqrt(X*X + Y*Y + Z*Z).
            )doc")
        .def("normalize", &Vector::normalize,
            R"doc(Normalize this vector in-place, dividing each component by the magnitude.)doc")
        .def("get_normalized", &Vector::getNormalized,
            R"doc(Return a normalized copy of this vector without modifying the original.

            Returns:
                Vector: A normalized copy.
            )doc")
        .def("clone", &Vector::clone,
            R"doc(Create a deep copy of this vector.

            Returns:
                Vector: A new vector with the same components.
            )doc")
        .def_static("dot", &Vector::dot, py::arg("v1"), py::arg("v2"),
            R"doc(Compute the dot product of two vectors.

            Args:
                v1: First vector.
                v2: Second vector.

            Returns:
                float: The dot product v1.X*v2.X + v1.Y*v2.Y + v1.Z*v2.Z.
            )doc")
        .def_static("cross", &Vector::cross, py::arg("v1"), py::arg("v2"),
            R"doc(Compute the cross product of two vectors.

            Args:
                v1: First vector.
                v2: Second vector.

            Returns:
                Vector: The cross product.
            )doc")
        .def_static("lerp", &Vector::lerp, py::arg("v1"), py::arg("v2"), py::arg("t"),
            R"doc(Linearly interpolate between two vectors.

            Args:
                v1: Starting vector (t=0).
                v2: Ending vector (t=1).
                t: Interpolation factor [0, 1].

            Returns:
                Vector: The interpolated vector.
            )doc")
        .def_static("slerp", &Vector::slerp, py::arg("v1"), py::arg("v2"), py::arg("t"),
            R"doc(Spherically interpolate between two vectors.

            Args:
                v1: Starting vector (t=0).
                v2: Ending vector (t=1).
                t: Interpolation factor [0, 1].

            Returns:
                Vector: The spherically interpolated vector.
            )doc")
        .def(py::self + py::self)
        .def(py::self - py::self)
        .def(py::self * py::self)
        .def(py::self / py::self)
        .def(py::self + float())
        .def(py::self - float())
        .def(py::self * float())
        .def(py::self / float())
        .def(py::self += py::self)
        .def(py::self -= py::self)
        .def(py::self *= py::self)
        .def(py::self /= py::self)
        .def(py::self += float())
        .def(py::self -= float())
        .def(py::self *= float())
        .def(py::self /= float())
        .def("__repr__", [](const Vector& v) {
            return "Vector(" + std::to_string(v.X) + ", " + std::to_string(v.Y) + ", " + std::to_string(v.Z) + ")";
        });

    // ============================================================
    // Rotator
    // ============================================================
    py::class_<Rotator>(m, "Rotator",
        R"doc(Represents a rotation with Pitch, Yaw, Roll integer components.

        Uses Unreal Engine's rotation units where 65536 = 360 degrees.
        )doc")
        .def(py::init<>(), "Construct a zero rotator (0, 0, 0).")
        .def(py::init<int, int, int>(), py::arg("pitch"), py::arg("yaw"), py::arg("roll"),
            "Construct a rotator from Pitch, Yaw, Roll.")
        .def(py::init<int>(), py::arg("def"),
            "Construct a rotator with all components set to the same value.")
        .def_readwrite("Pitch", &Rotator::Pitch, "Pitch component (rotation around Y axis).")
        .def_readwrite("Yaw", &Rotator::Yaw, "Yaw component (rotation around Z axis).")
        .def_readwrite("Roll", &Rotator::Roll, "Roll component (rotation around X axis).")
        .def(py::self += py::self)
        .def(py::self -= py::self)
        .def(py::self *= py::self)
        .def(py::self /= py::self)
        .def("__repr__", [](const Rotator& r) {
            return "Rotator(" + std::to_string(r.Pitch) + ", " + std::to_string(r.Yaw) + ", " + std::to_string(r.Roll) + ")";
        });

    // ============================================================
    // Quat
    // ============================================================
    py::class_<Quat>(m, "Quat",
        R"doc(Represents a quaternion with W, X, Y, Z float components.

        Used for rotation representation and interpolation.
        )doc")
        .def(py::init<>(), "Construct an identity quaternion (1, 0, 0, 0).")
        .def(py::init<float, float, float, float>(),
            py::arg("w"), py::arg("x"), py::arg("y"), py::arg("z"),
            "Construct a quaternion from W, X, Y, Z components.")
        .def_readwrite("X", &Quat::X, "The X component.")
        .def_readwrite("Y", &Quat::Y, "The Y component.")
        .def_readwrite("Z", &Quat::Z, "The Z component.")
        .def_readwrite("W", &Quat::W, "The W (scalar) component.")
        .def("conjugate", &Quat::conjugate,
            R"doc(Return the conjugate of this quaternion.

            The conjugate negates the X, Y, Z components.

            Returns:
                Quat: The conjugate quaternion.
            )doc")
        .def("normalize", &Quat::normalize,
            R"doc(Normalize this quaternion in-place to unit length.

            Returns:
                Quat: Self after normalization.
            )doc")
        .def(py::self * py::self)
        .def(py::self *= py::self)
        .def("__repr__", [](const Quat& q) {
            return "Quat(" + std::to_string(q.W) + ", " + std::to_string(q.X) + ", "
                + std::to_string(q.Y) + ", " + std::to_string(q.Z) + ")";
        });

    // ============================================================
    // Vector2
    // ============================================================
    py::class_<Vector2>(m, "Vector2",
        R"doc(Represents a 2D vector with integer X, Y components.

        Commonly used for screen coordinates.
        )doc")
        .def(py::init<>())
        .def(py::init<int, int>(), py::arg("x"), py::arg("y"))
        .def_readwrite("X", &Vector2::X, "The X component.")
        .def_readwrite("Y", &Vector2::Y, "The Y component.")
        .def("minus", &Vector2::minus, py::arg("other"),
            "Return a new vector = self - other.")
        .def(py::self += py::self)
        .def(py::self -= py::self)
        .def(py::self *= py::self)
        .def(py::self /= py::self)
        .def(py::self += int())
        .def(py::self -= int())
        .def(py::self *= int())
        .def(py::self /= int())
        .def("__repr__", [](const Vector2& v) {
            return "Vector2(" + std::to_string(v.X) + ", " + std::to_string(v.Y) + ")";
        });

    // ============================================================
    // Vector2F
    // ============================================================
    py::class_<Vector2F>(m, "Vector2F",
        R"doc(Represents a 2D vector with float X, Y components.

        Used for floating-point screen coordinates and sizes.
        )doc")
        .def(py::init<>())
        .def(py::init<float, float>(), py::arg("x"), py::arg("y"))
        .def_readwrite("X", &Vector2F::X, "The X component.")
        .def_readwrite("Y", &Vector2F::Y, "The Y component.")
        .def("minus", py::overload_cast<Vector2>(&Vector2F::minus), py::arg("other"))
        .def("minus", py::overload_cast<Vector2F>(&Vector2F::minus), py::arg("other"))
        .def(py::self += Vector2())
        .def(py::self -= Vector2())
        .def(py::self *= Vector2())
        .def(py::self /= Vector2())
        .def(py::self += Vector2F())
        .def(py::self -= Vector2F())
        .def(py::self *= Vector2F())
        .def(py::self /= Vector2F())
        .def(py::self += float())
        .def(py::self -= float())
        .def(py::self *= float())
        .def(py::self /= float())
        .def("__repr__", [](const Vector2F& v) {
            return "Vector2F(" + std::to_string(v.X) + ", " + std::to_string(v.Y) + ")";
        });

    // ============================================================
    // LinearColor
    // ============================================================
    py::class_<LinearColor>(m, "LinearColor",
        R"doc(Represents a color with float R, G, B, A components in [0, 1] range.

        Used throughout the rendering and UI systems.
        )doc")
        .def(py::init<>())
        .def(py::init<float, float, float, float>(),
            py::arg("r"), py::arg("g"), py::arg("b"), py::arg("a"),
            "Construct a LinearColor from R, G, B, A (0-1 range).")
        .def_readwrite("R", &LinearColor::R, "Red component (0-1).")
        .def_readwrite("G", &LinearColor::G, "Green component (0-1).")
        .def_readwrite("B", &LinearColor::B, "Blue component (0-1).")
        .def_readwrite("A", &LinearColor::A, "Alpha component (0-1).")
        .def(py::self * float())
        .def(py::self / float())
        .def(py::self *= float())
        .def(py::self /= float())
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def("__repr__", [](const LinearColor& c) {
            return "LinearColor(" + std::to_string(c.R) + ", " + std::to_string(c.G)
                + ", " + std::to_string(c.B) + ", " + std::to_string(c.A) + ")";
        });

    // ============================================================
    // UnrealColor
    // ============================================================
    py::class_<UnrealColor>(m, "UnrealColor",
        R"doc(Represents a color with unsigned byte B, G, R, A components (0-255).

        Note the byte order is B, G, R, A matching Unreal Engine convention.
        )doc")
        .def(py::init<>())
        .def(py::init<unsigned char, unsigned char, unsigned char, unsigned char>(),
            py::arg("b"), py::arg("g"), py::arg("r"), py::arg("a"))
        .def_readwrite("B", &UnrealColor::B, "Blue component (0-255).")
        .def_readwrite("G", &UnrealColor::G, "Green component (0-255).")
        .def_readwrite("R", &UnrealColor::R, "Red component (0-255).")
        .def_readwrite("A", &UnrealColor::A, "Alpha component (0-255).")
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def("__repr__", [](const UnrealColor& c) {
            return "UnrealColor(R=" + std::to_string(c.R) + ", G=" + std::to_string(c.G)
                + ", B=" + std::to_string(c.B) + ", A=" + std::to_string(c.A) + ")";
        });

    // ============================================================
    // PredictionInfo
    // ============================================================
    py::class_<PredictionInfo>(m, "PredictionInfo",
        R"doc(Contains ball trajectory prediction data at a specific time step.

        Provides position, velocity, and ground/wall hit flags at a predicted
        future time.
        )doc")
        .def(py::init<>())
        .def_readwrite("Time", &PredictionInfo::Time, "The predicted time in seconds.")
        .def_readwrite("ArchTopTime", &PredictionInfo::ArchTopTime, "Time at which the ball reaches the apex of this prediction step.")
        .def_readwrite("Location", &PredictionInfo::Location, "Predicted location at this time.")
        .def_readwrite("Velocity", &PredictionInfo::Velocity, "Predicted velocity at this time.")
        .def_readwrite("ArchTop", &PredictionInfo::ArchTop, "Location of the arch top (apex).")
        .def_readwrite("ArchTopVelocity", &PredictionInfo::ArchTopVelocity, "Velocity at the arch top.")
        .def_readwrite("bHitWall", &PredictionInfo::bHitWall, "Whether the ball will hit a wall before this prediction time.")
        .def_readwrite("bHitGround", &PredictionInfo::bHitGround, "Whether the ball will hit the ground before this prediction time.")
        .def("__repr__", [](const PredictionInfo& p) {
            return "PredictionInfo(Time=" + std::to_string(p.Time) + ")";
        });

    // ============================================================
    // SteamID
    // ============================================================
    py::class_<SteamID>(m, "SteamID",
        R"doc(Represents a Steam ID (deprecated, use UniqueIDWrapper for cross-platform).)doc")
        .def(py::init<>())
        .def_readwrite("ID", &SteamID::ID, "The 64-bit Steam ID value.")
        .def("__repr__", [](const SteamID& s) { return "SteamID(" + std::to_string(s.ID) + ")"; })
        .def("__eq__", [](const SteamID& a, const SteamID& b) { return a.ID == b.ID; })
        .def("__ne__", [](const SteamID& a, const SteamID& b) { return a.ID != b.ID; });

    // ============================================================
    // SkillRating
    // ============================================================
    py::class_<SkillRating>(m, "SkillRating",
        R"doc(Represents a player's skill rating using mu/sigma (TrueSkill).)doc")
        .def(py::init<>())
        .def_readwrite("Mu", &SkillRating::Mu, "The mu (mean) skill value.")
        .def_readwrite("Sigma", &SkillRating::Sigma, "The sigma (standard deviation) of the skill rating.")
        .def("__repr__", [](const SkillRating& s) {
            return "SkillRating(Mu=" + std::to_string(s.Mu) + ", Sigma=" + std::to_string(s.Sigma) + ")";
        });

    // ============================================================
    // SkillRank
    // ============================================================
    py::class_<SkillRank>(m, "SkillRank",
        R"doc(Represents a player's competitive rank including tier, division, and matches played.)doc")
        .def(py::init<>())
        .def_readwrite("Tier", &SkillRank::Tier, "Competitive tier (e.g. 0=Unranked, 3=Gold, etc.).")
        .def_readwrite("Division", &SkillRank::Division, "Division within the tier (0-3).")
        .def_readwrite("MatchesPlayed", &SkillRank::MatchesPlayed, "Number of matches played in this playlist.")
        .def("__repr__", [](const SkillRank& r) {
            return "SkillRank(Tier=" + std::to_string(r.Tier) + ", Division="
                + std::to_string(r.Division) + ", MatchesPlayed=" + std::to_string(r.MatchesPlayed) + ")";
        });

    // ============================================================
    // ControllerInput
    // ============================================================
    py::class_<ControllerInput>(m, "ControllerInput",
        R"doc(Represents a player's controller input state including throttle, steering, and actions.)doc")
        .def(py::init<>())
        .def_readwrite("Throttle", &ControllerInput::Throttle, "Throttle value (-1 to 1).")
        .def_readwrite("Steer", &ControllerInput::Steer, "Steer value (-1 to 1).")
        .def_readwrite("Pitch", &ControllerInput::Pitch, "Pitch input (-1 to 1).")
        .def_readwrite("Yaw", &ControllerInput::Yaw, "Yaw input (-1 to 1).")
        .def_readwrite("Roll", &ControllerInput::Roll, "Roll input (-1 to 1).")
        .def_readwrite("DodgeForward", &ControllerInput::DodgeForward, "Dodge forward direction.")
        .def_readwrite("DodgeStrafe", &ControllerInput::DodgeStrafe, "Dodge strafe direction.")
        .def_readwrite("Handbrake", &ControllerInput::Handbrake, "Handbrake button state.")
        .def_readwrite("Jump", &ControllerInput::Jump, "Jump button state.")
        .def_readwrite("ActivateBoost", &ControllerInput::ActivateBoost, "Boost activation state.")
        .def_readwrite("HoldingBoost", &ControllerInput::HoldingBoost, "Whether boost is being held.")
        .def_readwrite("Jumped", &ControllerInput::Jumped, "Whether jump was triggered.")
        .def("__repr__", [](const ControllerInput& c) {
            return "ControllerInput(Throttle=" + std::to_string(c.Throttle) + ", Steer=" + std::to_string(c.Steer) + ")";
        });

    // ============================================================
    // RBState
    // ============================================================
    py::class_<RBState>(m, "RBState",
        R"doc(Represents a rigid body's full physics state including transform and velocity.)doc")
        .def(py::init<>())
        .def_readwrite("Quaternion", &RBState::Quaternion, "Rotation as a quaternion.")
        .def_readwrite("Location", &RBState::Location, "World-space location.")
        .def_readwrite("LinearVelocity", &RBState::LinearVelocity, "Linear velocity.")
        .def_readwrite("AngularVelocity", &RBState::AngularVelocity, "Angular velocity.")
        .def_readwrite("Time", &RBState::Time, "Physics time stamp.")
        .def_readwrite("bSleeping", &RBState::bSleeping, "Whether the body is sleeping (physics optimized out).")
        .def_readwrite("bNewData", &RBState::bNewData, "Whether new physics data is available.");

    // ============================================================
    // WorldContactData
    // ============================================================
    py::class_<WorldContactData>(m, "WorldContactData",
        R"doc(Data about an actor's contact with the world geometry.)doc")
        .def(py::init<>())
        .def_readwrite("bHasContact", &WorldContactData::bHasContact, "Whether contact exists.")
        .def_readwrite("Location", &WorldContactData::Location, "Contact location.")
        .def_readwrite("Velocity", &WorldContactData::Velocity, "Velocity at contact point.")
        .def_readwrite("Normal", &WorldContactData::Normal, "Contact normal direction.");

    // ============================================================
    // StickyForceData
    // ============================================================
    py::class_<StickyForceData>(m, "StickyForceData",
        R"doc(Sticky force values for ground and wall contact.)doc")
        .def(py::init<>())
        .def_readwrite("Ground", &StickyForceData::Ground, "Ground sticky force.")
        .def_readwrite("Wall", &StickyForceData::Wall, "Wall sticky force.");

    // ============================================================
    // WheelContactData
    // ============================================================
    py::class_<WheelContactData>(m, "WheelContactData",
        R"doc(Data about a car wheel's contact with a surface.)doc")
        .def(py::init<>())
        .def_readwrite("bHasContact", &WheelContactData::bHasContact, "Whether the wheel has contact.")
        .def_readwrite("bHasContactWithWorldGeometry", &WheelContactData::bHasContactWithWorldGeometry,
            "Whether contact is with world geometry.")
        .def_readwrite("HasContactChangeTime", &WheelContactData::HasContactChangeTime,
            "Time when contact state last changed.")
        .def_readwrite("Location", &WheelContactData::Location, "Contact location.")
        .def_readwrite("Normal", &WheelContactData::Normal, "Contact normal.")
        .def_readwrite("LatDirection", &WheelContactData::LatDirection, "Lateral direction.")
        .def_readwrite("LongDirection", &WheelContactData::LongDirection, "Longitudinal direction.");

    // ============================================================
    // ReplayScoreData
    // ============================================================
    py::class_<ReplayScoreData>(m, "ReplayScoreData",
        R"doc(Data about a scored goal in a replay.)doc")
        .def(py::init<>())
        .def_readwrite("ScoredBy", &ReplayScoreData::ScoredBy, "Pointer/address of the scoring player's PriWrapper.")
        .def_readwrite("AssistedBy", &ReplayScoreData::AssistedBy, "Pointer/address of the assisting player's PriWrapper.")
        .def_readwrite("Speed", &ReplayScoreData::Speed, "Speed of the shot.")
        .def_readwrite("Time", &ReplayScoreData::Time, "Time when the goal was scored.")
        .def_readwrite("ScoreTeam", &ReplayScoreData::ScoreTeam, "Team that scored (0 or 1).");

    // ============================================================
    // ProfileCameraSettings
    // ============================================================
    py::class_<ProfileCameraSettings>(m, "ProfileCameraSettings",
        R"doc(Represents a player's camera settings profile.)doc")
        .def(py::init<>())
        .def_readwrite("FOV", &ProfileCameraSettings::FOV, "Field of view.")
        .def_readwrite("Height", &ProfileCameraSettings::Height, "Camera height.")
        .def_readwrite("Pitch", &ProfileCameraSettings::Pitch, "Camera pitch.")
        .def_readwrite("Distance", &ProfileCameraSettings::Distance, "Camera distance from car.")
        .def_readwrite("Stiffness", &ProfileCameraSettings::Stiffness, "Camera stiffness.")
        .def_readwrite("SwivelSpeed", &ProfileCameraSettings::SwivelSpeed, "Camera swivel speed.")
        .def_readwrite("TransitionSpeed", &ProfileCameraSettings::TransitionSpeed, "Camera transition speed.")
        .def("__repr__", [](const ProfileCameraSettings& s) {
            return "ProfileCameraSettings(FOV=" + std::to_string(s.FOV) + ", Height="
                + std::to_string(s.Height) + ", Pitch=" + std::to_string(s.Pitch) + ")";
        });

    // ============================================================
    // GamepadSettings
    // ============================================================
    py::class_<GamepadSettings>(m, "GamepadSettings",
        R"doc(Represents a player's gamepad/controller settings.)doc")
        .def(py::init<>())
        .def_readwrite("ControllerDeadzone", &GamepadSettings::ControllerDeadzone, "Controller deadzone.")
        .def_readwrite("DodgeInputThreshold", &GamepadSettings::DodgeInputThreshold, "Dodge input threshold.")
        .def_readwrite("SteeringSensitivity", &GamepadSettings::SteeringSensitivity, "Steering sensitivity.")
        .def_readwrite("AirControlSensitivity", &GamepadSettings::AirControlSensitivity, "Air control sensitivity.");

    // ============================================================
    // VideoSettings
    // ============================================================
    py::class_<VideoSettings>(m, "VideoSettings",
        R"doc(Represents the game's video/graphics settings.)doc")
        .def(py::init<>())
        .def_readwrite("bVsync", &VideoSettings::bVsync, "Whether VSync is enabled.")
        .def_readwrite("bShowWeatherFX", &VideoSettings::bShowWeatherFX, "Show weather effects.")
        .def_readwrite("bShowLightShafts", &VideoSettings::bShowLightShafts, "Show light shafts.")
        .def_readwrite("bTranslucentArenaShaders", &VideoSettings::bTranslucentArenaShaders,
            "Use translucent arena shaders.")
        .def_readwrite("bShowLensFlares", &VideoSettings::bShowLensFlares, "Show lens flares.")
        .def_readwrite("bEnableHDRSideBySideVisualizer", &VideoSettings::bEnableHDRSideBySideVisualizer,
            "Enable HDR side-by-side visualizer.")
        .def_readwrite("bUncappedFramerate", &VideoSettings::bUncappedFramerate, "Uncap framerate.")
        .def_readwrite("HDRBrightnessScale", &VideoSettings::HDRBrightnessScale, "HDR brightness scale.")
        .def_readwrite("HDRPaperWhiteScale", &VideoSettings::HDRPaperWhiteScale, "HDR paper white scale.")
        .def_readwrite("HDRGammaScale", &VideoSettings::HDRGammaScale, "HDR gamma scale.")
        .def_readwrite("WindowMode", &VideoSettings::WindowMode, "Window mode (0=windowed, 1=borderless, 2=fullscreen).")
        .def_readwrite("Resolution", &VideoSettings::Resolution, "Screen resolution string.")
        .def_readwrite("MaxFPS", &VideoSettings::MaxFPS, "Maximum FPS limit.")
        .def_readwrite("VideoOptions", &VideoSettings::VideoOptions, "Additional video options map.");

    // ============================================================
    // POV
    // ============================================================
    py::class_<POV>(m, "POV",
        R"doc(Represents a point of view with location, rotation, and FOV.)doc")
        .def(py::init<>())
        .def_readwrite("location", &POV::location, "Camera location.")
        .def_readwrite("rotation", &POV::rotation, "Camera rotation.")
        .def_readwrite("FOV", &POV::FOV, "Camera field of view.");

    // ============================================================
    // CameraSave
    // ============================================================
    py::class_<CameraSave>(m, "CameraSave",
        R"doc(Saved camera settings state.)doc")
        .def(py::init<>())
        .def_readwrite("InvertSwivelPitch", &CameraSave::InvertSwivelPitch, "Invert swivel pitch.")
        .def_readwrite("CameraShake", &CameraSave::CameraShake, "Camera shake enabled.");

    // ============================================================
    // ProductInstanceID
    // ============================================================
    py::class_<ProductInstanceID>(m, "ProductInstanceID",
        R"doc(Uniquely identifies an instance of a product/item.)doc")
        .def(py::init<>())
        .def_readwrite("upper_bits", &ProductInstanceID::upper_bits, "Upper 64 bits of the ID.")
        .def_readwrite("lower_bits", &ProductInstanceID::lower_bits, "Lower 64 bits of the ID.")
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def("__repr__", [](const ProductInstanceID& id) {
            return "ProductInstanceID(0x" + std::to_string(id.upper_bits)
                + " 0x" + std::to_string(id.lower_bits) + ")";
        });

    // ============================================================
    // GUIDWrapper
    // ============================================================
    py::class_<GUIDWrapper> guid_wrapper(m, "GUIDWrapper",
        R"doc(Wrapper for a 128-bit GUID/UUID identifier.

        Provides various string formatting options for the GUID.
        )doc");

    py::enum_<GUIDWrapper::EGuidFormats>(guid_wrapper, "EGuidFormats",
        R"doc(Format options for GUID string representation.)doc")
        .value("Digits", GUIDWrapper::EGuidFormats::Digits)
        .value("DigitsWithHyphens", GUIDWrapper::EGuidFormats::DigitsWithHyphens)
        .value("DigitsWithHyphensInBraces", GUIDWrapper::EGuidFormats::DigitsWithHyphensInBraces)
        .value("DigitsWithHyphensInParentheses", GUIDWrapper::EGuidFormats::DigitsWithHyphensInParentheses)
        .value("HexValuesInBraces", GUIDWrapper::EGuidFormats::HexValuesInBraces)
        .value("UniqueObjectGuid", GUIDWrapper::EGuidFormats::UniqueObjectGuid)
        .export_values();

    guid_wrapper
        .def(py::init<>())
        .def(py::init<int32_t, int32_t, int32_t, int32_t>(),
            py::arg("a"), py::arg("b"), py::arg("c"), py::arg("d"))
        .def_static("Create", &GUIDWrapper::Create,
            "Create a new random GUID.")
        .def_readwrite("A", &GUIDWrapper::A)
        .def_readwrite("B", &GUIDWrapper::B)
        .def_readwrite("C", &GUIDWrapper::C)
        .def_readwrite("D", &GUIDWrapper::D)
        .def("IsValid", &GUIDWrapper::IsValid,
            "Check whether this GUID is non-zero.")
        .def("ToString", &GUIDWrapper::ToString, py::arg("format") = GUIDWrapper::EGuidFormats::UniqueObjectGuid,
            R"doc(Convert this GUID to a string in the specified format.

            Args:
                format: The string format to use.

            Returns:
                str: The formatted GUID string.
            )doc");

    // ============================================================
    // TrainingRoundProgress
    // ============================================================
    py::class_<TrainingRoundProgress>(m, "TrainingRoundProgress",
        R"doc(Progress data for a training round.)doc")
        .def(py::init<>())
        .def_readwrite("RoundNumber", &TrainingRoundProgress::RoundNumber, "The round number.")
        .def_readwrite("Status", &TrainingRoundProgress::Status, "Round status (see EnumWrapper::GetTrainingRoundAttempts).");

    // ============================================================
    // RecordedSample
    // ============================================================
    py::class_<RecordedSample>(m, "RecordedSample",
        R"doc(A recorded performance sample with low/high values.)doc")
        .def(py::init<>())
        .def_readwrite("Low", &RecordedSample::Low, "Low value.")
        .def_readwrite("High", &RecordedSample::High, "High value.");

    // ============================================================
    // FREE FUNCTIONS: Vector/Rotator/Quat conversions
    // ============================================================
    m.def("VectorToRotator", &VectorToRotator, py::arg("v"),
        R"doc(Convert a direction vector to a rotator.

        Args:
            v: The direction vector.

        Returns:
            Rotator: The corresponding rotator.
        )doc");

    m.def("RotatorToVector", &RotatorToVector, py::arg("r"),
        R"doc(Convert a rotator to a direction vector.

        Args:
            r: The rotator.

        Returns:
            Vector: The forward direction vector.
        )doc");

    m.def("RotateVectorWithQuat", &RotateVectorWithQuat, py::arg("v"), py::arg("q"),
        R"doc(Rotate a vector by a quaternion.

        Args:
            v: The vector to rotate.
            q: The rotation quaternion.

        Returns:
            Vector: The rotated vector.
        )doc");

    m.def("RotatorToQuat", &RotatorToQuat, py::arg("rot"),
        R"doc(Convert a rotator to a quaternion.

        Args:
            rot: The rotator to convert.

        Returns:
            Quat: The equivalent quaternion.
        )doc");

    m.def("QuatToRotator", &QuatToRotator, py::arg("q"),
        R"doc(Convert a quaternion to a rotator.

        Args:
            q: The quaternion to convert.

        Returns:
            Rotator: The equivalent rotator.
        )doc");

    m.def("QuatSlerp", &QuatSlerp, py::arg("q1"), py::arg("q2"), py::arg("percent"),
        R"doc(Spherically interpolate between two quaternions.

        Args:
            q1: Starting quaternion (percent=0).
            q2: Ending quaternion (percent=1).
            percent: Interpolation factor [0, 1].

        Returns:
            Quat: The interpolated quaternion.
        )doc");

    // ============================================================
    // BotLoadoutData
    // ============================================================
    py::class_<BotLoadoutData>(m, "BotLoadoutData",
        R"doc(Loadout data for bot players.

        Contains product IDs and their attributes for bot customization.
        )doc")
        .def(py::init<>())
        .def_readwrite("products", &BotLoadoutData::products,
            "Map of equipment slot to product ID.")
        .def_readwrite("product_attributes", &BotLoadoutData::product_attributes,
            "Map of product ID to vector of attributes.");

    // ============================================================
    // CurlRequest
    // ============================================================
    py::class_<CurlRequest>(m, "CurlRequest",
        R"doc(HTTP request data structure for use with HttpWrapper.

        Encapsulates URL, verb/method, headers, body, and form data.
        )doc")
        .def(py::init<>())
        .def_readwrite("url", &CurlRequest::url, "The request URL.")
        .def_readwrite("verb", &CurlRequest::verb, "HTTP verb (GET, POST, PUT, DELETE, etc.).")
        .def_readwrite("headers", &CurlRequest::headers, "HTTP headers as a map of key:value pairs.")
        .def_readwrite("body", &CurlRequest::body, "Request body content.")
        .def_readwrite("form_data", &CurlRequest::form_data, "Form fields for multipart requests.");

    // ============================================================
    // FormField
    // ============================================================
    py::enum_<FormField::Type>(m, "FormFieldType",
        "The type of a form field (string or file).")
        .value("String", FormField::Type::kString)
        .value("File", FormField::Type::kFile)
        .export_values();

    py::class_<FormField>(m, "FormField",
        R"doc(A single form field for multipart HTTP requests.)doc")
        .def(py::init<>())
        .def_readwrite("field_type", &FormField::field_type, "Field type (String or File).")
        .def_readwrite("data", &FormField::data, "Field value (or file path if type is File).")
        .def_readwrite("name", &FormField::name, "Field name.");

    // ============================================================
    // PluginInfo
    // ============================================================
    py::class_<BakkesMod::Plugin::PluginInfo>(m, "PluginInfo",
        R"doc(Information about a loaded BakkesMod plugin.)doc")
        .def(py::init<>())
        .def_readwrite("apiBuildVersion", &BakkesMod::Plugin::PluginInfo::apiBuildVersion,
            "The BakkesMod API version this plugin was built for.")
        .def_readwrite("fileName", &BakkesMod::Plugin::PluginInfo::fileName,
            "The filename of the built DLL.")
        .def_readwrite("className", &BakkesMod::Plugin::PluginInfo::className,
            "The class name of the plugin.")
        .def_readwrite("pluginName", &BakkesMod::Plugin::PluginInfo::pluginName,
            "Display name of the plugin.")
        .def_readwrite("pluginVersion", &BakkesMod::Plugin::PluginInfo::pluginVersion,
            "Version string of the plugin.")
        .def_readwrite("pluginType", &BakkesMod::Plugin::PluginInfo::pluginType,
            "Plugin type bitmask (see PLUGINTYPE flags).");
}

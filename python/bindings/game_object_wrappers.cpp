#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <bakkesmod/wrappers/GameObject/VehicleWrapper.h>
#include <bakkesmod/wrappers/GameObject/CarWrapper.h>
#include <bakkesmod/wrappers/GameObject/BallWrapper.h>
#include <bakkesmod/wrappers/GameObject/PriWrapper.h>
#include <bakkesmod/wrappers/GameObject/CarComponent/CarComponentWrapper.h>
#include <bakkesmod/wrappers/GameObject/CarComponent/BoostWrapper.h>
#include <bakkesmod/wrappers/GameObject/CarComponent/DodgeComponentWrapper.h>
#include <bakkesmod/wrappers/GameObject/CarComponent/JumpComponentWrapper.h>
#include <bakkesmod/wrappers/GameObject/CarComponent/DoubleJumpComponentWrapper.h>
#include <bakkesmod/wrappers/GameObject/CarComponent/FlipCarComponentWrapper.h>
#include <bakkesmod/wrappers/GameObject/CarComponent/AirControlComponentWrapper.h>
#include <bakkesmod/wrappers/GameObject/CarComponent/PrimitiveComponentWrapper.h>
#include <bakkesmod/wrappers/GameObject/CarComponent/VehicleSimWrapper.h>
#include <bakkesmod/wrappers/GameObject/CarComponent/WheelWrapper.h>
#include <bakkesmod/wrappers/GameObject/TeamWrapper.h>
#include <bakkesmod/wrappers/GameObject/GoalWrapper.h>
#include <bakkesmod/wrappers/GameObject/BoostPickupWrapper.h>
#include <bakkesmod/wrappers/GameObject/CameraWrapper.h>
#include <bakkesmod/wrappers/GameObject/CameraXWrapper.h>
#include <bakkesmod/wrappers/GameObject/BaseCameraWrapper.h>
#include <bakkesmod/wrappers/GameObject/CameraSettingsActorWrapper.h>
#include <bakkesmod/wrappers/GameObject/FXActorWrapper.h>
#include <bakkesmod/wrappers/GameObject/ClubDetailsWrapper.h>
#include <bakkesmod/wrappers/GameObject/MapDataWrapper.h>
#include <bakkesmod/wrappers/GameObject/MapListWrapper.h>
#include <bakkesmod/wrappers/GameObject/PhysicalMaterialPropertyWrapper.h>
#include <bakkesmod/wrappers/GameObject/TeamInfoWrapper.h>
#include <bakkesmod/wrappers/GameObject/VehiclePickupWrapper.h>
#include <bakkesmod/wrappers/GameObject/BreakOutActorPlatformWrapper.h>
#include <bakkesmod/wrappers/GameObject/BallBreakoutWrapper.h>
#include <bakkesmod/wrappers/GameObject/BallGodWrapper.h>
#include <bakkesmod/wrappers/GameObject/BallHauntedWrapper.h>
#include <bakkesmod/wrappers/GameObject/GoalHoopsWrapper.h>
#include <bakkesmod/wrappers/GameObject/CameraStates/CameraStateWrapper.h>
#include <bakkesmod/wrappers/GameObject/CameraStates/CameraStateBlenderWrapper.h>
#include <bakkesmod/wrappers/GameObject/CameraStates/CameraStateCarWrapper.h>
#include <bakkesmod/wrappers/GameObject/CameraStates/CameraStateCarRefWrapper.h>
#include <bakkesmod/wrappers/GameObject/CameraStates/CameraStateXWrapper.h>
#include <bakkesmod/wrappers/GameObject/RumbleComponent/RumblePickupComponentWrapper.h>

namespace py = pybind11;

void init_game_object_wrappers(py::module& m) {
    // ============================================================
    // VehicleWrapper
    // ============================================================
    py::class_<VehicleWrapper, RBActorWrapper>(m, "VehicleWrapper",
        R"doc(Base wrapper for vehicle actors (cars).

        Provides access to vehicle-specific properties like driving state,
        boost, jump, dodge components, and controller access.
        )doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"))
        .def("GetbCanJump", &VehicleWrapper::GetbCanJump)
        .def("SetbCanJump", &VehicleWrapper::SetbCanJump, py::arg("newbCanJump"))
        .def("GetVehicleSim", &VehicleWrapper::GetVehicleSim)
        .def("SetVehicleSim", &VehicleWrapper::SetVehicleSim, py::arg("newVehicleSim"))
        .def("GetStickyForce", &VehicleWrapper::GetStickyForce)
        .def("SetStickyForce", &VehicleWrapper::SetStickyForce, py::arg("newStickyForce"))
        .def("GetbDriving", &VehicleWrapper::GetbDriving)
        .def("SetbDriving", &VehicleWrapper::SetbDriving, py::arg("newbDriving"))
        .def("GetbReplicatedHandbrake", &VehicleWrapper::GetbReplicatedHandbrake)
        .def("SetbReplicatedHandbrake", &VehicleWrapper::SetbReplicatedHandbrake, py::arg("newbReplicatedHandbrake"))
        .def("GetbJumped", &VehicleWrapper::GetbJumped)
        .def("SetbJumped", &VehicleWrapper::SetbJumped, py::arg("newbJumped"))
        .def("GetbDoubleJumped", &VehicleWrapper::GetbDoubleJumped)
        .def("SetbDoubleJumped", &VehicleWrapper::SetbDoubleJumped, py::arg("newbDoubleJumped"))
        .def("GetbOnGround", &VehicleWrapper::GetbOnGround)
        .def("SetbOnGround", &VehicleWrapper::SetbOnGround, py::arg("newbOnGround"))
        .def("GetbSuperSonic", &VehicleWrapper::GetbSuperSonic)
        .def("SetbSuperSonic", &VehicleWrapper::SetbSuperSonic, py::arg("newbSuperSonic"))
        .def("GetbPodiumMode", &VehicleWrapper::GetbPodiumMode)
        .def("SetbPodiumMode", &VehicleWrapper::SetbPodiumMode, py::arg("newbPodiumMode"))
        .def("GetInput", &VehicleWrapper::GetInput)
        .def("SetInput", &VehicleWrapper::SetInput, py::arg("newInput"))
        .def("GetReplicatedThrottle", &VehicleWrapper::GetReplicatedThrottle)
        .def("SetReplicatedThrottle", &VehicleWrapper::SetReplicatedThrottle, py::arg("newReplicatedThrottle"))
        .def("GetReplicatedSteer", &VehicleWrapper::GetReplicatedSteer)
        .def("SetReplicatedSteer", &VehicleWrapper::SetReplicatedSteer, py::arg("newReplicatedSteer"))
        .def("GetAIController", &VehicleWrapper::GetAIController)
        .def("GetPlayerController", &VehicleWrapper::GetPlayerController)
        .def("SetPlayerController", &VehicleWrapper::SetPlayerController, py::arg("newPlayerController"))
        .def("GetPRI", &VehicleWrapper::GetPRI)
        .def("SetPRI", &VehicleWrapper::SetPRI, py::arg("newPRI"))
        .def("GetVehicleUpdateTag", &VehicleWrapper::GetVehicleUpdateTag)
        .def("SetVehicleUpdateTag", &VehicleWrapper::SetVehicleUpdateTag, py::arg("newVehicleUpdateTag"))
        .def("GetLocalCollisionOffset", &VehicleWrapper::GetLocalCollisionOffset)
        .def("SetLocalCollisionOffset", &VehicleWrapper::SetLocalCollisionOffset, py::arg("newLocalCollisionOffset"))
        .def("GetLocalCollisionExtent", &VehicleWrapper::GetLocalCollisionExtent)
        .def("SetLocalCollisionExtent", &VehicleWrapper::SetLocalCollisionExtent, py::arg("newLocalCollisionExtent"))
        .def("GetLastBallTouchFrame", &VehicleWrapper::GetLastBallTouchFrame)
        .def("SetLastBallTouchFrame", &VehicleWrapper::SetLastBallTouchFrame, py::arg("newLastBallTouchFrame"))
        .def("GetLastBallImpactFrame", &VehicleWrapper::GetLastBallImpactFrame)
        .def("SetLastBallImpactFrame", &VehicleWrapper::SetLastBallImpactFrame, py::arg("newLastBallImpactFrame"))
        .def("GetBoostComponent", &VehicleWrapper::GetBoostComponent)
        .def("GetDodgeComponent", &VehicleWrapper::GetDodgeComponent)
        .def("GetAirControlComponent", &VehicleWrapper::GetAirControlComponent)
        .def("GetJumpComponent", &VehicleWrapper::GetJumpComponent)
        .def("GetDoubleJumpComponent", &VehicleWrapper::GetDoubleJumpComponent)
        .def("SetDoubleJumpComponent", &VehicleWrapper::SetDoubleJumpComponent, py::arg("newDoubleJumpComponent"))
        .def("GetTimeBelowSupersonicSpeed", &VehicleWrapper::GetTimeBelowSupersonicSpeed)
        .def("SetTimeBelowSupersonicSpeed", &VehicleWrapper::SetTimeBelowSupersonicSpeed, py::arg("newTimeBelowSupersonicSpeed"))
        .def("GetForwardSpeed", &VehicleWrapper::GetForwardSpeed)
        .def("GetTimeOffGround", &VehicleWrapper::GetTimeOffGround)
        .def("GetTimeOnGround", &VehicleWrapper::GetTimeOnGround)
        .def("GetGroundNormal", &VehicleWrapper::GetGroundNormal)
        .def("IsOnWall", &VehicleWrapper::IsOnWall)
        .def("IsOnGround", &VehicleWrapper::IsOnGround)
        .def("GetNumWheelWorldContacts", &VehicleWrapper::GetNumWheelWorldContacts)
        .def("GetNumWheelContacts", &VehicleWrapper::GetNumWheelContacts);

    // ============================================================
    // CarWrapper
    // ============================================================
    py::class_<CarWrapper, VehicleWrapper>(m, "CarWrapper",
        R"doc(Wrapper for a player's car.

        Provides full access to car state: boost, rotation, demolition,
        loadout, input, and car-specific physics interactions.
        )doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"))
        .def("IsBoostCheap", &CarWrapper::IsBoostCheap,
            "Check if boost is in 'cheap' mode (infinite).")
        .def("SetBoostCheap", &CarWrapper::SetBoostCheap, py::arg("b"),
            "Enable or disable cheap (infinite) boost.")
        .def("SetCarRotation", &CarWrapper::SetCarRotation, py::arg("rotation"),
            "Set the car's rotation directly.")
        .def("ForceBoost", &CarWrapper::ForceBoost, py::arg("force"),
            "Force boost on or off.")
        .def("GetOwnerName", &CarWrapper::GetOwnerName,
            "Get the owner player's name.")
        .def("Unfreeze", &CarWrapper::Unfreeze,
            "Unfreeze the car if it was frozen.")
        .def("GetInput", &CarWrapper::GetInput,
            "Get the current controller input for this car.")
        .def("SetInput", &CarWrapper::SetInput, py::arg("input"),
            "Set controller input for this car.")
        .def("Destroy", &CarWrapper::Destroy,
            "Destroy the car actor.")
        .def("Demolish", &CarWrapper::Demolish,
            "Demolish/explode the car.")
        .def("HasFlip", &CarWrapper::HasFlip,
            "Check whether the car still has its flip available.")
        .def("GetLoadoutBody", &CarWrapper::GetLoadoutBody,
            "Get the product ID of the car body in the loadout.")
        .def("SetCarColor", &CarWrapper::SetCarColor,
            py::arg("mainColor"), py::arg("secondaryColor"),
            "Set the car's primary and secondary colors (freeplay only).")
        .def("GetDefaultCarComponents", &CarWrapper::GetDefaultCarComponents)
        .def("GetFlipComponent", &CarWrapper::GetFlipComponent)
        .def("GetDemolishTarget", &CarWrapper::GetDemolishTarget)
        .def("SetDemolishTarget", &CarWrapper::SetDemolishTarget, py::arg("newDemolishTarget"))
        .def("GetDemolishSpeed", &CarWrapper::GetDemolishSpeed)
        .def("SetDemolishSpeed", &CarWrapper::SetDemolishSpeed, py::arg("newDemolishSpeed"))
        .def("GetbLoadoutSet", &CarWrapper::GetbLoadoutSet)
        .def("SetbLoadoutSet", &CarWrapper::SetbLoadoutSet, py::arg("newbLoadoutSet"))
        .def("GetbDemolishOnOpposingGround", &CarWrapper::GetbDemolishOnOpposingGround)
        .def("GetbWasOnOpposingGround", &CarWrapper::GetbWasOnOpposingGround)
        .def("GetbDemolishOnGoalZone", &CarWrapper::GetbDemolishOnGoalZone)
        .def("GetbOverrideHandbrakeOn", &CarWrapper::GetbOverrideHandbrakeOn)
        .def("GetbCollidesWithVehicles", &CarWrapper::GetbCollidesWithVehicles)
        .def("GetbOverrideBoostOn", &CarWrapper::GetbOverrideBoostOn)
        .def("GetMaxTimeForDodge", &CarWrapper::GetMaxTimeForDodge)
        .def("GetLastWheelsHitBallTime", &CarWrapper::GetLastWheelsHitBallTime)
        .def("GetReplicatedCarScale", &CarWrapper::GetReplicatedCarScale)
        .def("GetBodyFXActor", &CarWrapper::GetBodyFXActor)
        .def("GetAttackerPRI", &CarWrapper::GetAttackerPRI)
        .def("GetMouseAccel", &CarWrapper::GetMouseAccel)
        .def("GetMouseAirAccel", &CarWrapper::GetMouseAirAccel)
        .def("GetAttachedPickup", &CarWrapper::GetAttachedPickup)
        .def("GetReplayFocusOffset", &CarWrapper::GetReplayFocusOffset)
        .def("GetAddedBallForceMultiplier", &CarWrapper::GetAddedBallForceMultiplier)
        .def("GetAddedCarForceMultiplier", &CarWrapper::GetAddedCarForceMultiplier)
        .def("GetGameEvent", &CarWrapper::GetGameEvent)
        .def("GetMaxDriveBackwardsSpeed", &CarWrapper::GetMaxDriveBackwardsSpeed)
        .def("GetMaxDriveForwardSpeed", &CarWrapper::GetMaxDriveForwardSpeed)
        .def("Teleport", &CarWrapper::Teleport, py::arg("SpawnLocation"), py::arg("SpawnRotation"),
            py::arg("bStopVelocity"), py::arg("bUpdateRotation"), py::arg("ExtraForce"))
        .def("IsDodging", &CarWrapper::IsDodging)
        .def("AnyWheelTouchingGround", &CarWrapper::AnyWheelTouchingGround);

    // ============================================================
    // BallWrapper
    // ============================================================
    py::class_<BallWrapper, RBActorWrapper>(m, "BallWrapper",
        R"doc(Wrapper for the ball in a match.

        Provides access to ball physics, prediction, and event interaction data.
        )doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"))
        .def("GetLastTouchTime", &BallWrapper::GetLastTouchTime,
            "Get the time since the ball was last touched by a car.")
        .def("PredictPosition", &BallWrapper::PredictPosition, py::arg("timeAhead"),
            R"doc(Predict the ball's position at a future time.

            Args:
                timeAhead: Time in seconds to predict ahead.

            Returns:
                PredictionInfo: The predicted state at the given time.
            )doc")
        .def("IsDropshotBall", &BallWrapper::IsDropshotBall,
            "Check if this is a Dropshot mode ball.")
        .def("IsHauntedtBall", &BallWrapper::IsHauntedtBall,
            "Check if this is a Haunted (Ghost Hunt) ball.")
        .def("IsGodBall", &BallWrapper::IsGodBall,
            "Check if this is a God Ball (custom training).")
        .def("FadeOutBall", &BallWrapper::FadeOutBall,
            "Fade the ball out visually.")
        .def("FadeInBall", &BallWrapper::FadeInBall,
            "Fade the ball in visually.")
        .def("GetEndOfGameFXArchetype", &BallWrapper::GetEndOfGameFXArchetype)
        .def("SetEndOfGameFXArchetype", &BallWrapper::SetEndOfGameFXArchetype, py::arg("newEndOfGameFXArchetype"))
        .def("GetbAllowPlayerExplosionOverride", &BallWrapper::GetbAllowPlayerExplosionOverride)
        .def("GetbNotifyGroundHit", &BallWrapper::GetbNotifyGroundHit)
        .def("GetbEndOfGameHidden", &BallWrapper::GetbEndOfGameHidden)
        .def("GetbFadeIn", &BallWrapper::GetbFadeIn)
        .def("GetbFadeOut", &BallWrapper::GetbFadeOut)
        .def("GetbPredictionOnGround", &BallWrapper::GetbPredictionOnGround)
        .def("GetbCanBeAttached", &BallWrapper::GetbCanBeAttached)
        .def("GetbItemFreeze", &BallWrapper::GetbItemFreeze)
        .def("GetMagnusCoefficient", &BallWrapper::GetMagnusCoefficient)
        .def("SetMagnusCoefficient", &BallWrapper::SetMagnusCoefficient, py::arg("newMagnusCoefficient"))
        .def("GetRadius", &BallWrapper::GetRadius)
        .def("SetRadius", &BallWrapper::SetRadius, py::arg("newRadius"))
        .def("GetVisualRadius", &BallWrapper::GetVisualRadius)
        .def("SetVisualRadius", &BallWrapper::SetVisualRadius, py::arg("newVisualRadius"))
        .def("GetLastCalculateCarHit", &BallWrapper::GetLastCalculateCarHit)
        .def("GetInitialLocation", &BallWrapper::GetInitialLocation)
        .def("SetInitialLocation", &BallWrapper::SetInitialLocation, py::arg("newInitialLocation"))
        .def("SetInitialRotation", &BallWrapper::SetInitialRotation, py::arg("newInitialRotation"))
        .def("GetLastHitWorldTime", &BallWrapper::GetLastHitWorldTime)
        .def("GetReplicatedBallScale", &BallWrapper::GetReplicatedBallScale)
        .def("SetReplicatedBallScale", &BallWrapper::SetReplicatedBallScale, py::arg("newReplicatedBallScale"))
        .def("GetReplicatedWorldBounceScale", &BallWrapper::GetReplicatedWorldBounceScale)
        .def("GetReplicatedBallGravityScale", &BallWrapper::GetReplicatedBallGravityScale)
        .def("GetReplicatedBallMaxLinearSpeedScale", &BallWrapper::GetReplicatedBallMaxLinearSpeedScale)
        .def("GetReplicatedAddedCarBounceScale", &BallWrapper::GetReplicatedAddedCarBounceScale)
        .def("GetAdditionalCarGroundBounceScaleZ", &BallWrapper::GetAdditionalCarGroundBounceScaleZ)
        .def("GetAdditionalCarGroundBounceScaleXY", &BallWrapper::GetAdditionalCarGroundBounceScaleXY)
        .def("GetHitTeamNum", &BallWrapper::GetHitTeamNum)
        .def("SetHitTeamNum", &BallWrapper::SetHitTeamNum, py::arg("newHitTeamNum"))
        .def("GetGameEvent", &BallWrapper::GetGameEvent)
        .def("GetExplosionTime", &BallWrapper::GetExplosionTime)
        .def("SetExplosionTime", &BallWrapper::SetExplosionTime, py::arg("newExplosionTime"))
        .def("GetOldLocation", &BallWrapper::GetOldLocation)
        .def("SetOldLocation", &BallWrapper::SetOldLocation, py::arg("newOldLocation"))
        .def("GetPredictionTimestep", &BallWrapper::GetPredictionTimestep)
        .def("SetPredictionTimestep", &BallWrapper::SetPredictionTimestep, py::arg("newPredictionTimestep"))
        .def("GetLastPredictionTime", &BallWrapper::GetLastPredictionTime)
        .def("SetLastPredictionTime", &BallWrapper::SetLastPredictionTime, py::arg("newLastPredictionTime"))
        .def("GetGroundForce", &BallWrapper::GetGroundForce)
        .def("SetGroundForce", &BallWrapper::SetGroundForce, py::arg("newGroundForce"))
        .def("GetCurrentAffector", &BallWrapper::GetCurrentAffector)
        .def("SetCurrentAffector", &BallWrapper::SetCurrentAffector, py::arg("newCurrentAffector"))
        .def("ShouldDrawTrajectory", &BallWrapper::ShouldDrawTrajectory)
        .def("GetTrajectoryStartVelocity", &BallWrapper::GetTrajectoryStartVelocity)
        .def("GetTrajectoryStartRotation", &BallWrapper::GetTrajectoryStartRotation)
        .def("GetTrajectoryStartLocation", &BallWrapper::GetTrajectoryStartLocation);

    // ============================================================
    // PriWrapper
    // ============================================================
    py::class_<PriWrapper, PriXWrapper>(m, "PriWrapper",
        R"doc(Wrapper for PlayerReplicationInfo (PRI), providing player state data.

        Includes match stats, camera settings, party info, club info, and
        player state flags.
        )doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"))
        .def("GetMatchScore", &PriWrapper::GetMatchScore)
        .def("SetMatchScore", &PriWrapper::SetMatchScore, py::arg("newMatchScore"))
        .def("GetMatchGoals", &PriWrapper::GetMatchGoals)
        .def("SetMatchGoals", &PriWrapper::SetMatchGoals, py::arg("newMatchGoals"))
        .def("GetMatchOwnGoals", &PriWrapper::GetMatchOwnGoals)
        .def("SetMatchOwnGoals", &PriWrapper::SetMatchOwnGoals, py::arg("newMatchOwnGoals"))
        .def("GetMatchAssists", &PriWrapper::GetMatchAssists)
        .def("SetMatchAssists", &PriWrapper::SetMatchAssists, py::arg("newMatchAssists"))
        .def("GetMatchSaves", &PriWrapper::GetMatchSaves)
        .def("SetMatchSaves", &PriWrapper::SetMatchSaves, py::arg("newMatchSaves"))
        .def("GetMatchShots", &PriWrapper::GetMatchShots)
        .def("SetMatchShots", &PriWrapper::SetMatchShots, py::arg("newMatchShots"))
        .def("GetMatchDemolishes", &PriWrapper::GetMatchDemolishes)
        .def("SetMatchDemolishes", &PriWrapper::SetMatchDemolishes, py::arg("newMatchDemolishes"))
        .def("GetMatchBonusXP", &PriWrapper::GetMatchBonusXP)
        .def("SetMatchBonusXP", &PriWrapper::SetMatchBonusXP, py::arg("newMatchBonusXP"))
        .def("GetMatchBreakoutDamage", &PriWrapper::GetMatchBreakoutDamage)
        .def("SetMatchBreakoutDamage", &PriWrapper::SetMatchBreakoutDamage, py::arg("newMatchBreakoutDamage"))
        .def("GetbMatchMVP", &PriWrapper::GetbMatchMVP)
        .def("GetbMatchAdmin", &PriWrapper::GetbMatchAdmin)
        .def("GetbReady", &PriWrapper::GetbReady)
        .def("GetbUsingSecondaryCamera", &PriWrapper::GetbUsingSecondaryCamera)
        .def("GetbUsingBehindView", &PriWrapper::GetbUsingBehindView)
        .def("GetbUsingFreecam", &PriWrapper::GetbUsingFreecam)
        .def("GetbUsingItems", &PriWrapper::GetbUsingItems)
        .def("GetGameEvent", &PriWrapper::GetGameEvent)
        .def("GetReplicatedGameEvent", &PriWrapper::GetReplicatedGameEvent)
        .def("GetCar", &PriWrapper::GetCar)
        .def("SetCar", &PriWrapper::SetCar, py::arg("newCar"))
        .def("GetRespawnTimeRemaining", &PriWrapper::GetRespawnTimeRemaining)
        .def("SetRespawnTimeRemaining", &PriWrapper::SetRespawnTimeRemaining, py::arg("newRespawnTimeRemaining"))
        .def("GetTotalGameTimePlayed", &PriWrapper::GetTotalGameTimePlayed)
        .def("GetCameraSettings", &PriWrapper::GetCameraSettings)
        .def("SetCameraSettings", &PriWrapper::SetCameraSettings, py::arg("newCameraSettings"))
        .def("GetCameraPitch", &PriWrapper::GetCameraPitch)
        .def("GetCameraYaw", &PriWrapper::GetCameraYaw)
        .def("GetPawnType", &PriWrapper::GetPawnType)
        .def("GetPartyLeaderID", &PriWrapper::GetPartyLeaderID)
        .def("SetPartyLeader", py::overload_cast<UniqueIDWrapper>(&PriWrapper::SetPartyLeader),
            py::arg("newPartyLeader"))
        .def("GetDodgeInputThreshold", &PriWrapper::GetDodgeInputThreshold)
        .def("GetSteeringSensitivity", &PriWrapper::GetSteeringSensitivity)
        .def("GetAirControlSensitivity", &PriWrapper::GetAirControlSensitivity)
        .def("GetBoostPickups", &PriWrapper::GetBoostPickups)
        .def("GetBallTouches", &PriWrapper::GetBallTouches)
        .def("GetCarTouches", &PriWrapper::GetCarTouches)
        .def("GetClubID", &PriWrapper::GetClubID)
        .def("GetClubDetails", &PriWrapper::GetClubDetails)
        .def("GetLastTeamIndex", &PriWrapper::GetLastTeamIndex)
        .def("IsPlayer", &PriWrapper::IsPlayer)
        .def("IsEditor", &PriWrapper::IsEditor)
        .def("IsSpectator", &PriWrapper::IsSpectator)
        .def("IsLocalPlayerPRI", &PriWrapper::IsLocalPlayerPRI)
        .def("IsClientPlayerPRI", &PriWrapper::IsClientPlayerPRI);

    // ============================================================
    // CarComponentWrapper
    // ============================================================
    py::class_<CarComponentWrapper, ObjectWrapper>(m, "CarComponentWrapper",
        R"doc(Base wrapper for car components (boost, dodge, jump, etc.).)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<BoostWrapper, CarComponentWrapper>(m, "BoostWrapper",
        R"doc(Wrapper for the car's boost component.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<DodgeComponentWrapper, CarComponentWrapper>(m, "DodgeComponentWrapper",
        R"doc(Wrapper for the car's dodge component.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<JumpComponentWrapper, CarComponentWrapper>(m, "JumpComponentWrapper",
        R"doc(Wrapper for the car's jump component.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<DoubleJumpComponentWrapper, CarComponentWrapper>(m, "DoubleJumpComponentWrapper",
        R"doc(Wrapper for the car's double jump component.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<FlipCarComponentWrapper, CarComponentWrapper>(m, "FlipCarComponentWrapper",
        R"doc(Wrapper for the car's flip (dodge) component.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<AirControlComponentWrapper, CarComponentWrapper>(m, "AirControlComponentWrapper",
        R"doc(Wrapper for the car's air control component.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<PrimitiveComponentWrapper, ObjectWrapper>(m, "PrimitiveComponentWrapper",
        R"doc(Wrapper for primitive (mesh) components.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<VehicleSimWrapper, ObjectWrapper>(m, "VehicleSimWrapper",
        R"doc(Wrapper for vehicle simulation data.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<WheelWrapper, ObjectWrapper>(m, "WheelWrapper",
        R"doc(Wrapper for a car's wheel component.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    // ============================================================
    // Other Game Object Wrappers
    // ============================================================
    py::class_<TeamWrapper, ObjectWrapper>(m, "TeamWrapper",
        R"doc(Wrapper for a team in a game event.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<GoalWrapper, ActorWrapper>(m, "GoalWrapper",
        R"doc(Wrapper for a goal actor on the field.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<BoostPickupWrapper, ActorWrapper>(m, "BoostPickupWrapper",
        R"doc(Wrapper for a boost pickup on the field.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<BaseCameraWrapper, ActorWrapper>(m, "BaseCameraWrapper",
        R"doc(Base wrapper for camera actors.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<CameraXWrapper, BaseCameraWrapper>(m, "CameraXWrapper",
        R"doc(Extended camera wrapper.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<CameraWrapper, CameraXWrapper>(m, "CameraWrapper",
        R"doc(Wrapper for the game camera.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<CameraSettingsActorWrapper, ObjectWrapper>(m, "CameraSettingsActorWrapper",
        R"doc(Wrapper for camera settings actors.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<FXActorWrapper, ActorWrapper>(m, "FXActorWrapper",
        R"doc(Wrapper for special effects (FX) actors.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<ClubDetailsWrapper, ObjectWrapper>(m, "ClubDetailsWrapper",
        R"doc(Wrapper for club details data.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<MapDataWrapper, ObjectWrapper>(m, "MapDataWrapper",
        R"doc(Wrapper for map/arena data.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<MapListWrapper, ObjectWrapper>(m, "MapListWrapper",
        R"doc(Wrapper for the map list.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<PhysicalMaterialPropertyWrapper, ObjectWrapper>(m, "PhysicalMaterialPropertyWrapper",
        R"doc(Wrapper for physical material properties.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<TeamInfoWrapper, ObjectWrapper>(m, "TeamInfoWrapper",
        R"doc(Wrapper for team info data.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<VehiclePickupWrapper, ActorWrapper>(m, "VehiclePickupWrapper",
        R"doc(Wrapper for vehicle pickups.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<BreakOutActorPlatformWrapper, ActorWrapper>(m, "BreakOutActorPlatformWrapper",
        R"doc(Wrapper for Breakout (Dropshot) platform actors.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<BallBreakoutWrapper, BallWrapper>(m, "BallBreakoutWrapper",
        R"doc(Wrapper for the Dropshot mode ball.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<BallGodWrapper, BallWrapper>(m, "BallGodWrapper",
        R"doc(Wrapper for the custom training 'God Ball'.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<BallHauntedWrapper, BallWrapper>(m, "BallHauntedWrapper",
        R"doc(Wrapper for the Haunted (Ghost Hunt) ball.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<GoalHoopsWrapper, GoalWrapper>(m, "GoalHoopsWrapper",
        R"doc(Wrapper for the Hoops mode goal.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    // ============================================================
    // Camera State Wrappers
    // ============================================================
    py::class_<CameraStateWrapper, ObjectWrapper>(m, "CameraStateWrapper",
        R"doc(Base wrapper for camera states.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<CameraStateBlenderWrapper, CameraStateWrapper>(m, "CameraStateBlenderWrapper",
        R"doc(Wrapper for camera state blending.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<CameraStateCarWrapper, CameraStateWrapper>(m, "CameraStateCarWrapper",
        R"doc(Wrapper for car-following camera state.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<CameraStateCarRefWrapper, CameraStateWrapper>(m, "CameraStateCarRefWrapper",
        R"doc(Wrapper for car reference camera state.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<CameraStateXWrapper, CameraStateWrapper>(m, "CameraStateXWrapper",
        R"doc(Extended camera state wrapper.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    // ============================================================
    // Rumble Pickup Wrappers
    // ============================================================
    py::class_<RumblePickupComponentWrapper, ObjectWrapper>(m, "RumblePickupComponentWrapper",
        R"doc(Base wrapper for Rumble mode pickups.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));
}

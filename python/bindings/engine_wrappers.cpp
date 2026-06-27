#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <bakkesmod/wrappers/Engine/ObjectWrapper.h>
#include <bakkesmod/wrappers/Engine/ActorWrapper.h>
#include <bakkesmod/wrappers/GameObject/RBActorWrapper.h>
#include <bakkesmod/wrappers/GameObject/PriXWrapper.h>
#include <bakkesmod/wrappers/GameObject/PlayerReplicationInfoWrapper.h>
#include <bakkesmod/wrappers/Engine/EngineTAWrapper.h>
#include <bakkesmod/wrappers/Engine/UnrealStringWrapper.h>
#include <bakkesmod/wrappers/Engine/WorldInfoWrapper.h>
#include <bakkesmod/wrappers/UniqueIDWrapper.h>

namespace py = pybind11;

void init_engine_wrappers(py::module& m) {
    // ============================================================
    // ObjectWrapper (base class for all wrappers)
    // ============================================================
    py::class_<ObjectWrapper>(m, "ObjectWrapper",
        R"doc(Base wrapper class for all BakkesMod wrapper types.

        Holds a memory_address pointing to the underlying Unreal Engine object.
        All game object wrappers inherit from this class.
        )doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"),
            "Construct from a raw memory address.")
        .def_readwrite("memory_address", &ObjectWrapper::memory_address,
            "The underlying Unreal Engine object memory address.");

    // ============================================================
    // ActorWrapper
    // ============================================================
    py::class_<ActorWrapper, ObjectWrapper>(m, "ActorWrapper",
        R"doc(Wrapper for an Unreal Engine Actor.

        Provides access to location, rotation, velocity, physics, collision,
        and other common actor properties.
        )doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"))

        // Self-implemented methods
        .def("GetLocation", &ActorWrapper::GetLocation,
            "Get the actor's world-space location.")
        .def("SetLocation", &ActorWrapper::SetLocation, py::arg("location"),
            "Set the actor's world-space location.")
        .def("GetVelocity", &ActorWrapper::GetVelocity,
            "Get the actor's linear velocity.")
        .def("SetVelocity", &ActorWrapper::SetVelocity, py::arg("velocity"),
            "Set the actor's linear velocity.")
        .def("AddVelocity", &ActorWrapper::AddVelocity, py::arg("velocity"),
            "Add velocity to the actor's current velocity.")
        .def("GetRotation", &ActorWrapper::GetRotation,
            "Get the actor's rotation.")
        .def("SetRotation", &ActorWrapper::SetRotation, py::arg("rotation"),
            "Set the actor's rotation.")
        .def("SetTorque", &ActorWrapper::SetTorque, py::arg("torq"),
            "Apply torque to the actor.")
        .def("Stop", &ActorWrapper::Stop,
            "Stop all movement immediately.")
        .def("GetAngularVelocity", &ActorWrapper::GetAngularVelocity,
            "Get the actor's angular velocity.")
        .def("SetAngularVelocity", &ActorWrapper::SetAngularVelocity,
            py::arg("v"), py::arg("addToCurrent"),
            "Set or add to the angular velocity.")
        .def("IsNull", py::overload_cast<>(&ActorWrapper::IsNull, py::const_),
            "Check if the underlying pointer is null.")
        .def("GetWorldInfo", &ActorWrapper::GetWorldInfo,
            "Get the world info for this actor.")

        // Auto-generated field getters/setters
        .def("GetDrawScale", &ActorWrapper::GetDrawScale)
        .def("SetDrawScale", &ActorWrapper::SetDrawScale, py::arg("newDrawScale"))
        .def("GetDrawScale3D", &ActorWrapper::GetDrawScale3D)
        .def("SetDrawScale3D", &ActorWrapper::SetDrawScale3D, py::arg("newDrawScale3D"))
        .def("GetPrePivot", &ActorWrapper::GetPrePivot)
        .def("SetPrePivot", &ActorWrapper::SetPrePivot, py::arg("newPrePivot"))
        .def("GetEditorIconColor", &ActorWrapper::GetEditorIconColor)
        .def("SetEditorIconColor", &ActorWrapper::SetEditorIconColor, py::arg("newEditorIconColor"))
        .def("GetCustomTimeDilation", &ActorWrapper::GetCustomTimeDilation)
        .def("SetCustomTimeDilation", &ActorWrapper::SetCustomTimeDilation, py::arg("newCustomTimeDilation"))
        .def("GetPhysics", &ActorWrapper::GetPhysics)
        .def("SetPhysics", &ActorWrapper::SetPhysics, py::arg("newPhysics"))
        .def("GetRemoteRole", &ActorWrapper::GetRemoteRole)
        .def("SetRemoteRole", &ActorWrapper::SetRemoteRole, py::arg("newRemoteRole"))
        .def("GetRole", &ActorWrapper::GetRole)
        .def("SetRole", &ActorWrapper::SetRole, py::arg("newRole"))
        .def("GetCollisionType", &ActorWrapper::GetCollisionType)
        .def("SetCollisionType", &ActorWrapper::SetCollisionType, py::arg("newCollisionType"))
        .def("GetReplicatedCollisionType", &ActorWrapper::GetReplicatedCollisionType)
        .def("SetReplicatedCollisionType", &ActorWrapper::SetReplicatedCollisionType, py::arg("newReplicatedCollisionType"))
        .def("GetOwner", &ActorWrapper::GetOwner)
        .def("GetBase", &ActorWrapper::GetBase)
        .def("GetbStatic", &ActorWrapper::GetbStatic)
        .def("GetbHidden", &ActorWrapper::GetbHidden)
        .def("GetbHiddenSelf", &ActorWrapper::GetbHiddenSelf)
        .def("SetbHiddenSelf", &ActorWrapper::SetbHiddenSelf, py::arg("newbHiddenSelf"))
        .def("GetbNoDelete", &ActorWrapper::GetbNoDelete)
        .def("SetbNoDelete", &ActorWrapper::SetbNoDelete, py::arg("newbNoDelete"))
        .def("GetbDeleteMe", &ActorWrapper::GetbDeleteMe)
        .def("SetbDeleteMe", &ActorWrapper::SetbDeleteMe, py::arg("newbDeleteMe"))
        .def("GetbTicked", &ActorWrapper::GetbTicked)
        .def("SetbTicked", &ActorWrapper::SetbTicked, py::arg("newbTicked"))
        .def("GetbOnlyOwnerSee", &ActorWrapper::GetbOnlyOwnerSee)
        .def("SetbOnlyOwnerSee", &ActorWrapper::SetbOnlyOwnerSee, py::arg("newbOnlyOwnerSee"))
        .def("GetbTickIsDisabled", &ActorWrapper::GetbTickIsDisabled)
        .def("SetbTickIsDisabled", &ActorWrapper::SetbTickIsDisabled, py::arg("newbTickIsDisabled"))
        .def("GetbWorldGeometry", &ActorWrapper::GetbWorldGeometry)
        .def("SetbWorldGeometry", &ActorWrapper::SetbWorldGeometry, py::arg("newbWorldGeometry"))
        .def("GetbIgnoreRigidBodyPawns", &ActorWrapper::GetbIgnoreRigidBodyPawns)
        .def("SetbIgnoreRigidBodyPawns", &ActorWrapper::SetbIgnoreRigidBodyPawns, py::arg("newbIgnoreRigidBodyPawns"))
        .def("GetbOrientOnSlope", &ActorWrapper::GetbOrientOnSlope)
        .def("SetbOrientOnSlope", &ActorWrapper::SetbOrientOnSlope, py::arg("newbOrientOnSlope"))
        .def("GetbIsMoving", &ActorWrapper::GetbIsMoving)
        .def("GetbAlwaysEncroachCheck", &ActorWrapper::GetbAlwaysEncroachCheck)
        .def("SetbAlwaysEncroachCheck", &ActorWrapper::SetbAlwaysEncroachCheck, py::arg("newbAlwaysEncroachCheck"))
        .def("GetbHasAlternateTargetLocation", &ActorWrapper::GetbHasAlternateTargetLocation)
        .def("GetbAlwaysRelevant", &ActorWrapper::GetbAlwaysRelevant)
        .def("GetbReplicateInstigator", &ActorWrapper::GetbReplicateInstigator)
        .def("GetbReplicateMovement", &ActorWrapper::GetbReplicateMovement)
        .def("GetbUpdateSimulatedPosition", &ActorWrapper::GetbUpdateSimulatedPosition)
        .def("SetbUpdateSimulatedPosition", &ActorWrapper::SetbUpdateSimulatedPosition, py::arg("newbUpdateSimulatedPosition"))
        .def("GetbDemoRecording", &ActorWrapper::GetbDemoRecording)
        .def("SetbDemoRecording", &ActorWrapper::SetbDemoRecording, py::arg("newbDemoRecording"))
        .def("GetbDemoOwner", &ActorWrapper::GetbDemoOwner)
        .def("SetbDemoOwner", &ActorWrapper::SetbDemoOwner, py::arg("newbDemoOwner"))
        .def("GetbForceDemoRelevant", &ActorWrapper::GetbForceDemoRelevant)
        .def("SetbForceDemoRelevant", &ActorWrapper::SetbForceDemoRelevant, py::arg("newbForceDemoRelevant"))
        .def("GetbNetInitialRotation", &ActorWrapper::GetbNetInitialRotation)
        .def("SetbNetInitialRotation", &ActorWrapper::SetbNetInitialRotation, py::arg("newbNetInitialRotation"))
        .def("GetbReplicateRigidBodyLocation", &ActorWrapper::GetbReplicateRigidBodyLocation)
        .def("SetbReplicateRigidBodyLocation", &ActorWrapper::SetbReplicateRigidBodyLocation, py::arg("newbReplicateRigidBodyLocation"))
        .def("GetbKillDuringLevelTransition", &ActorWrapper::GetbKillDuringLevelTransition)
        .def("SetbKillDuringLevelTransition", &ActorWrapper::SetbKillDuringLevelTransition, py::arg("newbKillDuringLevelTransition"))
        .def("GetbPostRenderIfNotVisible", &ActorWrapper::GetbPostRenderIfNotVisible)
        .def("SetbPostRenderIfNotVisible", &ActorWrapper::SetbPostRenderIfNotVisible, py::arg("newbPostRenderIfNotVisible"))
        .def("GetbForceNetUpdate", &ActorWrapper::GetbForceNetUpdate)
        .def("SetbForceNetUpdate", &ActorWrapper::SetbForceNetUpdate, py::arg("newbForceNetUpdate"))
        .def("GetbForcePacketUpdate", &ActorWrapper::GetbForcePacketUpdate)
        .def("SetbForcePacketUpdate", &ActorWrapper::SetbForcePacketUpdate, py::arg("newbForcePacketUpdate"))
        .def("GetbPendingNetUpdate", &ActorWrapper::GetbPendingNetUpdate)
        .def("SetbPendingNetUpdate", &ActorWrapper::SetbPendingNetUpdate, py::arg("newbPendingNetUpdate"))
        .def("GetbGameRelevant", &ActorWrapper::GetbGameRelevant)
        .def("SetbGameRelevant", &ActorWrapper::SetbGameRelevant, py::arg("newbGameRelevant"))
        .def("GetbMovable", &ActorWrapper::GetbMovable)
        .def("SetbMovable", &ActorWrapper::SetbMovable, py::arg("newbMovable"))
        .def("GetbCanTeleport", &ActorWrapper::GetbCanTeleport)
        .def("SetbCanTeleport", &ActorWrapper::SetbCanTeleport, py::arg("newbCanTeleport"))
        .def("GetbAlwaysTick", &ActorWrapper::GetbAlwaysTick)
        .def("SetbAlwaysTick", &ActorWrapper::SetbAlwaysTick, py::arg("newbAlwaysTick"))
        .def("GetbBlocksNavigation", &ActorWrapper::GetbBlocksNavigation)
        .def("SetbBlocksNavigation", &ActorWrapper::SetbBlocksNavigation, py::arg("newbBlocksNavigation"))
        .def("GetBlockRigidBody", &ActorWrapper::GetBlockRigidBody)
        .def("SetBlockRigidBody", &ActorWrapper::SetBlockRigidBody, py::arg("newBlockRigidBody"))
        .def("GetbCollideWhenPlacing", &ActorWrapper::GetbCollideWhenPlacing)
        .def("SetbCollideWhenPlacing", &ActorWrapper::SetbCollideWhenPlacing, py::arg("newbCollideWhenPlacing"))
        .def("GetbCollideActors", &ActorWrapper::GetbCollideActors)
        .def("SetbCollideActors", &ActorWrapper::SetbCollideActors, py::arg("newbCollideActors"))
        .def("GetbCollideWorld", &ActorWrapper::GetbCollideWorld)
        .def("SetbCollideWorld", &ActorWrapper::SetbCollideWorld, py::arg("newbCollideWorld"))
        .def("GetbCollideComplex", &ActorWrapper::GetbCollideComplex)
        .def("SetbCollideComplex", &ActorWrapper::SetbCollideComplex, py::arg("newbCollideComplex"))
        .def("GetbBlockActors", &ActorWrapper::GetbBlockActors)
        .def("SetbBlockActors", &ActorWrapper::SetbBlockActors, py::arg("newbBlockActors"))
        .def("GetbBlocksTeleport", &ActorWrapper::GetbBlocksTeleport)
        .def("SetbBlocksTeleport", &ActorWrapper::SetbBlocksTeleport, py::arg("newbBlocksTeleport"))
        .def("GetbPhysRigidBodyOutOfWorldCheck", &ActorWrapper::GetbPhysRigidBodyOutOfWorldCheck)
        .def("SetbPhysRigidBodyOutOfWorldCheck", &ActorWrapper::SetbPhysRigidBodyOutOfWorldCheck, py::arg("newbPhysRigidBodyOutOfWorldCheck"))
        .def("GetbComponentOutsideWorld", &ActorWrapper::GetbComponentOutsideWorld)
        .def("GetbRigidBodyWasAwake", &ActorWrapper::GetbRigidBodyWasAwake)
        .def("SetbRigidBodyWasAwake", &ActorWrapper::SetbRigidBodyWasAwake, py::arg("newbRigidBodyWasAwake"))
        .def("GetbCallRigidBodyWakeEvents", &ActorWrapper::GetbCallRigidBodyWakeEvents)
        .def("SetbCallRigidBodyWakeEvents", &ActorWrapper::SetbCallRigidBodyWakeEvents, py::arg("newbCallRigidBodyWakeEvents"))
        .def("GetbBounce", &ActorWrapper::GetbBounce)
        .def("SetbBounce", &ActorWrapper::SetbBounce, py::arg("newbBounce"))
        .def("GetbEditable", &ActorWrapper::GetbEditable)
        .def("SetbEditable", &ActorWrapper::SetbEditable, py::arg("newbEditable"))
        .def("GetbLockLocation", &ActorWrapper::GetbLockLocation)
        .def("SetbLockLocation", &ActorWrapper::SetbLockLocation, py::arg("newbLockLocation"))
        .def("GetNetUpdateTime", &ActorWrapper::GetNetUpdateTime)
        .def("SetNetUpdateTime", &ActorWrapper::SetNetUpdateTime, py::arg("newNetUpdateTime"))
        .def("GetNetUpdateFrequency", &ActorWrapper::GetNetUpdateFrequency)
        .def("SetNetUpdateFrequency", &ActorWrapper::SetNetUpdateFrequency, py::arg("newNetUpdateFrequency"))
        .def("GetNetPriority", &ActorWrapper::GetNetPriority)
        .def("SetNetPriority", &ActorWrapper::SetNetPriority, py::arg("newNetPriority"))
        .def("GetLastNetUpdateTime", &ActorWrapper::GetLastNetUpdateTime)
        .def("GetLastForcePacketUpdateTime", &ActorWrapper::GetLastForcePacketUpdateTime)
        .def("SetLastForcePacketUpdateTime", &ActorWrapper::SetLastForcePacketUpdateTime, py::arg("newLastForcePacketUpdateTime"))
        .def("GetTimeSinceLastTick", &ActorWrapper::GetTimeSinceLastTick)
        .def("GetLifeSpan", &ActorWrapper::GetLifeSpan)
        .def("GetCreationTime", &ActorWrapper::GetCreationTime)
        .def("GetLastRenderTime", &ActorWrapper::GetLastRenderTime)
        .def("GetHiddenEditorViews", &ActorWrapper::GetHiddenEditorViews)
        .def("SetHiddenEditorViews", &ActorWrapper::SetHiddenEditorViews, py::arg("newHiddenEditorViews"))
        .def("GetAttached", &ActorWrapper::GetAttached)
        .def("GetRelativeLocation", &ActorWrapper::GetRelativeLocation)
        .def("SetRelativeLocation", &ActorWrapper::SetRelativeLocation, py::arg("newRelativeLocation"))
        .def("GetRelativeRotation", &ActorWrapper::GetRelativeRotation)
        .def("SetRelativeRotation", &ActorWrapper::SetRelativeRotation, py::arg("newRelativeRotation"))
        .def("GetCollisionComponent", &ActorWrapper::GetCollisionComponent)

        // Auto-generated function proxies
        .def("ForceNetUpdatePacket", &ActorWrapper::ForceNetUpdatePacket)
        .def("ForceNetUpdate2", &ActorWrapper::ForceNetUpdate2)
        .def("WillOverlap", &ActorWrapper::WillOverlap, py::arg("PosA"), py::arg("VelA"),
            py::arg("PosB"), py::arg("VelB"), py::arg("StepSize"), py::arg("Radius"), py::arg("Time"))
        .def("IsInPersistentLevel", &ActorWrapper::IsInPersistentLevel, py::arg("bIncludeLevelStreamingPersistent"))
        .def("GetTargetLocation", &ActorWrapper::GetTargetLocation, py::arg("RequestedBy"), py::arg("bRequestAlternateLoc"))
        .def("GetTeamNum2", &ActorWrapper::GetTeamNum2)
        .def("IsPlayerOwned", &ActorWrapper::IsPlayerOwned)
        .def("IsStationary", &ActorWrapper::IsStationary)
        .def("GetGravityAcceleration", &ActorWrapper::GetGravityAcceleration)
        .def("GetGravityDirection", &ActorWrapper::GetGravityDirection)
        .def("GetGravityZ", &ActorWrapper::GetGravityZ)
        .def("IsOverlapping", &ActorWrapper::IsOverlapping, py::arg("A"))
        .def("ContainsPoint", &ActorWrapper::ContainsPoint, py::arg("Spot"))
        .def("SetTickIsDisabled", &ActorWrapper::SetTickIsDisabled, py::arg("bInDisabled"))
        .def("SetPhysics2", &ActorWrapper::SetPhysics2, py::arg("newPhysics"))
        .def("SetHidden2", &ActorWrapper::SetHidden2, py::arg("bNewHidden"))
        .def("ChartData", &ActorWrapper::ChartData, py::arg("DataName"), py::arg("DataValue"))
        .def("DrawDebugString", &ActorWrapper::DrawDebugString, py::arg("TextLocation"),
            py::arg("Text"), py::arg("TestBaseActor"), py::arg("TextColor"), py::arg("Duration"))
        .def("DrawDebugCone", &ActorWrapper::DrawDebugCone, py::arg("Origin"), py::arg("Direction"),
            py::arg("Length"), py::arg("AngleWidth"), py::arg("AngleHeight"), py::arg("NumSides"),
            py::arg("DrawColor"), py::arg("bPersistentLines"))
        .def("GetAggregateBaseVelocity", &ActorWrapper::GetAggregateBaseVelocity, py::arg("TestBase"))
        .def("IsOwnedBy", &ActorWrapper::IsOwnedBy, py::arg("TestActor"))
        .def("IsBasedOn", &ActorWrapper::IsBasedOn, py::arg("TestActor"))
        .def("GetTerminalVelocity", &ActorWrapper::GetTerminalVelocity);

    // ============================================================
    // RBActorWrapper
    // ============================================================
    py::class_<RBActorWrapper, ActorWrapper>(m, "RBActorWrapper",
        R"doc(Wrapper for rigid body (physics-enabled) actors.

        Provides access to physics state including RBState, forces, sleeping,
        and rigid body properties.
        )doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"))
        .def("GetMaxLinearSpeed", &RBActorWrapper::GetMaxLinearSpeed)
        .def("SetMaxLinearSpeed", &RBActorWrapper::SetMaxLinearSpeed, py::arg("newMaxLinearSpeed"))
        .def("GetMaxAngularSpeed", &RBActorWrapper::GetMaxAngularSpeed)
        .def("SetMaxAngularSpeed", &RBActorWrapper::SetMaxAngularSpeed, py::arg("newMaxAngularSpeed"))
        .def("GetbDisableSleeping", &RBActorWrapper::GetbDisableSleeping)
        .def("SetbDisableSleeping", &RBActorWrapper::SetbDisableSleeping, py::arg("newbDisableSleeping"))
        .def("GetbReplayActor", &RBActorWrapper::GetbReplayActor)
        .def("SetbReplayActor", &RBActorWrapper::SetbReplayActor, py::arg("newbReplayActor"))
        .def("GetbFrozen", &RBActorWrapper::GetbFrozen)
        .def("SetbFrozen", &RBActorWrapper::SetbFrozen, py::arg("newbFrozen"))
        .def("GetbIgnoreSyncing", &RBActorWrapper::GetbIgnoreSyncing)
        .def("SetbIgnoreSyncing", &RBActorWrapper::SetbIgnoreSyncing, py::arg("newbIgnoreSyncing"))
        .def("GetbPhysInitialized", &RBActorWrapper::GetbPhysInitialized)
        .def("GetOldRBState", &RBActorWrapper::GetOldRBState)
        .def("SetOldRBState", &RBActorWrapper::SetOldRBState, py::arg("newOldRBState"))
        .def("GetRBState", &RBActorWrapper::GetRBState)
        .def("SetRBState", &RBActorWrapper::SetRBState, py::arg("newRBState"))
        .def("GetReplicatedRBState", &RBActorWrapper::GetReplicatedRBState)
        .def("SetReplicatedRBState", &RBActorWrapper::SetReplicatedRBState, py::arg("newReplicatedRBState"))
        .def("GetClientCorrectionRBState", &RBActorWrapper::GetClientCorrectionRBState)
        .def("SetClientCorrectionRBState", &RBActorWrapper::SetClientCorrectionRBState, py::arg("newClientCorrectionRBState"))
        .def("GetWorldContact", &RBActorWrapper::GetWorldContact)
        .def("SetWorldContact", &RBActorWrapper::SetWorldContact, py::arg("newWorldContact"))
        .def("GetSyncErrorLocation", &RBActorWrapper::GetSyncErrorLocation)
        .def("GetSyncErrorAngle", &RBActorWrapper::GetSyncErrorAngle)
        .def("GetSyncErrorAxis", &RBActorWrapper::GetSyncErrorAxis)
        .def("GetFXActorArchetype", &RBActorWrapper::GetFXActorArchetype)
        .def("SetFXActorArchetype", &RBActorWrapper::SetFXActorArchetype, py::arg("newFXActorArchetype"))
        .def("GetFXActor", &RBActorWrapper::GetFXActor)
        .def("SetFXActor", &RBActorWrapper::SetFXActor, py::arg("newFXActor"))
        .def("GetLastRBCollisionsFrame", &RBActorWrapper::GetLastRBCollisionsFrame)
        .def("GetWeldedActor", &RBActorWrapper::GetWeldedActor)
        .def("GetWeldedTo", &RBActorWrapper::GetWeldedTo)
        .def("GetPreWeldMass", &RBActorWrapper::GetPreWeldMass)
        .def("SetMass", &RBActorWrapper::SetMass, py::arg("NewMass"))
        .def("SetConstrained3D", &RBActorWrapper::SetConstrained3D, py::arg("LinearLower"),
            py::arg("LinearUpper"), py::arg("AngularLower"), py::arg("AngularUpper"))
        .def("SetConstrained2D", &RBActorWrapper::SetConstrained2D, py::arg("bConstrain2D"))
        .def("SetPhysicsState", &RBActorWrapper::SetPhysicsState, py::arg("NewState"))
        .def("SetFrozen", &RBActorWrapper::SetFrozen, py::arg("bEnabled"))
        .def("SetMaxAngularSpeed2", &RBActorWrapper::SetMaxAngularSpeed2, py::arg("NewMaxSpeed"))
        .def("SetMaxLinearSpeed2", &RBActorWrapper::SetMaxLinearSpeed2, py::arg("NewMaxSpeed"))
        .def("AddTorque", &RBActorWrapper::AddTorque, py::arg("Torque"), py::arg("ForceMode"))
        .def("AddForce", &RBActorWrapper::AddForce, py::arg("Force"), py::arg("ForceMode"))
        .def("UnWeldRBActor", &RBActorWrapper::UnWeldRBActor, py::arg("Other"))
        .def("WeldRBActor2", &RBActorWrapper::WeldRBActor2, py::arg("Other"),
            py::arg("WeldOffset"), py::arg("WeldRotation"))
        .def("ReInitRBPhys", &RBActorWrapper::ReInitRBPhys)
        .def("TerminateRBPhys", &RBActorWrapper::TerminateRBPhys)
        .def("GetCurrentRBLocation", &RBActorWrapper::GetCurrentRBLocation)
        .def("GetCurrentRBState", &RBActorWrapper::GetCurrentRBState)
        .def("GetPhysicsFrame", &RBActorWrapper::GetPhysicsFrame)
        .def("GetPhysicsTime", &RBActorWrapper::GetPhysicsTime);

    // ============================================================
    // PlayerReplicationInfoWrapper
    // ============================================================
    py::class_<PlayerReplicationInfoWrapper, ActorWrapper>(m, "PlayerReplicationInfoWrapper",
        R"doc(Wrapper for Unreal Engine PlayerReplicationInfo (base PRI).

        Provides access to player identity, score, team, ping, name,
        and platform information.
        )doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"))
        .def("GetPlatform", &PlayerReplicationInfoWrapper::GetPlatform,
            "Get the player's online platform.")
        .def("ChangeNameForScoreboardAndNameplateInReplay",
            &PlayerReplicationInfoWrapper::ChangeNameForScoreboardAndNameplateInReplay,
            py::arg("name"),
            "Change the player's displayed name in replays.")
        .def("GetScore", &PlayerReplicationInfoWrapper::GetScore)
        .def("SetScore", &PlayerReplicationInfoWrapper::SetScore, py::arg("newScore"))
        .def("GetDeaths", &PlayerReplicationInfoWrapper::GetDeaths)
        .def("SetDeaths", &PlayerReplicationInfoWrapper::SetDeaths, py::arg("newDeaths"))
        .def("GetPing", &PlayerReplicationInfoWrapper::GetPing)
        .def("SetPing", &PlayerReplicationInfoWrapper::SetPing, py::arg("newPing"))
        .def("GetNumLives", &PlayerReplicationInfoWrapper::GetNumLives)
        .def("SetNumLives", &PlayerReplicationInfoWrapper::SetNumLives, py::arg("newNumLives"))
        .def("GetPlayerName", &PlayerReplicationInfoWrapper::GetPlayerName)
        .def("GetOldName", &PlayerReplicationInfoWrapper::GetOldName)
        .def("GetPlayerID", &PlayerReplicationInfoWrapper::GetPlayerID)
        .def("SetPlayerID", &PlayerReplicationInfoWrapper::SetPlayerID, py::arg("newPlayerID"))
        .def("GetTeam", &PlayerReplicationInfoWrapper::GetTeam)
        .def("SetTeam", &PlayerReplicationInfoWrapper::SetTeam, py::arg("newTeam"))
        .def("GetbAdmin", &PlayerReplicationInfoWrapper::GetbAdmin)
        .def("SetbAdmin", &PlayerReplicationInfoWrapper::SetbAdmin, py::arg("newbAdmin"))
        .def("GetbIsSpectator", &PlayerReplicationInfoWrapper::GetbIsSpectator)
        .def("SetbIsSpectator", &PlayerReplicationInfoWrapper::SetbIsSpectator, py::arg("newbIsSpectator"))
        .def("GetbOnlySpectator", &PlayerReplicationInfoWrapper::GetbOnlySpectator)
        .def("SetbOnlySpectator", &PlayerReplicationInfoWrapper::SetbOnlySpectator, py::arg("newbOnlySpectator"))
        .def("GetbWaitingPlayer", &PlayerReplicationInfoWrapper::GetbWaitingPlayer)
        .def("SetbWaitingPlayer", &PlayerReplicationInfoWrapper::SetbWaitingPlayer, py::arg("newbWaitingPlayer"))
        .def("GetbReadyToPlay", &PlayerReplicationInfoWrapper::GetbReadyToPlay)
        .def("SetbReadyToPlay", &PlayerReplicationInfoWrapper::SetbReadyToPlay, py::arg("newbReadyToPlay"))
        .def("GetbOutOfLives", &PlayerReplicationInfoWrapper::GetbOutOfLives)
        .def("SetbOutOfLives", &PlayerReplicationInfoWrapper::SetbOutOfLives, py::arg("newbOutOfLives"))
        .def("GetbBot", &PlayerReplicationInfoWrapper::GetbBot)
        .def("SetbBot", &PlayerReplicationInfoWrapper::SetbBot, py::arg("newbBot"))
        .def("GetbIsInactive", &PlayerReplicationInfoWrapper::GetbIsInactive)
        .def("SetbIsInactive", &PlayerReplicationInfoWrapper::SetbIsInactive, py::arg("newbIsInactive"))
        .def("GetbFromPreviousLevel", &PlayerReplicationInfoWrapper::GetbFromPreviousLevel)
        .def("SetbFromPreviousLevel", &PlayerReplicationInfoWrapper::SetbFromPreviousLevel, py::arg("newbFromPreviousLevel"))
        .def("GetbTimedOut", &PlayerReplicationInfoWrapper::GetbTimedOut)
        .def("SetbTimedOut", &PlayerReplicationInfoWrapper::SetbTimedOut, py::arg("newbTimedOut"))
        .def("GetbUnregistered", &PlayerReplicationInfoWrapper::GetbUnregistered)
        .def("SetbUnregistered", &PlayerReplicationInfoWrapper::SetbUnregistered, py::arg("newbUnregistered"))
        .def("GetStartTime", &PlayerReplicationInfoWrapper::GetStartTime)
        .def("SetStartTime", &PlayerReplicationInfoWrapper::SetStartTime, py::arg("newStartTime"))
        .def("GetKills", &PlayerReplicationInfoWrapper::GetKills)
        .def("SetKills", &PlayerReplicationInfoWrapper::SetKills, py::arg("newKills"))
        .def("GetExactPing", &PlayerReplicationInfoWrapper::GetExactPing)
        .def("SetExactPing", &PlayerReplicationInfoWrapper::SetExactPing, py::arg("newExactPing"))
        .def("GetUniqueIdWrapper", &PlayerReplicationInfoWrapper::GetUniqueIdWrapper)
        .def("SetUniqueId", py::overload_cast<UniqueIDWrapper>(&PlayerReplicationInfoWrapper::SetUniqueId),
            py::arg("newUniqueId"))
        .def("GetTeamNum", &PlayerReplicationInfoWrapper::GetTeamNum);

    // ============================================================
    // PriXWrapper
    // ============================================================
    py::class_<PriXWrapper, PlayerReplicationInfoWrapper>(m, "PriXWrapper",
        R"doc(Extended player replication info wrapper.

        Base class for PriWrapper providing extended player data handling.
        )doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"))
        .def("SetPlayerTeam", &PriXWrapper::SetPlayerTeam, py::arg("NewTeam"))
        .def("eventSetPlayerName", &PriXWrapper::eventSetPlayerName, py::arg("S"));

    // ============================================================
    // EngineTAWrapper
    // ============================================================
    py::class_<EngineTAWrapper, ObjectWrapper>(m, "EngineTAWrapper",
        R"doc(Wrapper for the game's physics engine state.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    // ============================================================
    // UnrealStringWrapper
    // ============================================================
    py::class_<UnrealStringWrapper>(m, "UnrealStringWrapper",
        R"doc(Wrapper for an Unreal Engine FString, providing string data access.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"))
        .def("ToString", &UnrealStringWrapper::ToString,
            "Convert the Unreal string to a std::string.")
        .def("__repr__", [](const UnrealStringWrapper& s) { return s.ToString(); })
        .def("__str__", [](const UnrealStringWrapper& s) { return s.ToString(); });

    // ============================================================
    // WorldInfoWrapper
    // ============================================================
    py::class_<WorldInfoWrapper, ObjectWrapper>(m, "WorldInfoWrapper",
        R"doc(Wrapper for the world's info object, providing world state data.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));
}

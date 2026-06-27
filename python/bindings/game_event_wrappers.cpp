#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <bakkesmod/wrappers/GameEvent/GameEventWrapper.h>
#include <bakkesmod/wrappers/GameEvent/TeamGameEventWrapper.h>
#include <bakkesmod/wrappers/GameEvent/ServerWrapper.h>
#include <bakkesmod/wrappers/GameEvent/ReplayWrapper.h>
#include <bakkesmod/wrappers/GameEvent/ReplayDirectorWrapper.h>
#include <bakkesmod/wrappers/GameEvent/ReplaySoccarWrapper.h>
#include <bakkesmod/wrappers/GameEvent/TrainingEditorWrapper.h>
#include <bakkesmod/wrappers/GameEvent/TutorialWrapper.h>
#include <bakkesmod/wrappers/GameEvent/GameEditorWrapper.h>
#include <bakkesmod/wrappers/GameEvent/GameSettingPlaylistWrapper.h>
#include <bakkesmod/wrappers/GameEvent/SaveData/SaveDataWrapper.h>
#include <bakkesmod/wrappers/GameEvent/SaveData/GameEditorSaveDataWrapper.h>
#include <bakkesmod/wrappers/GameEvent/SaveData/TrainingEditorSaveDataWrapper.h>

namespace py = pybind11;

void init_game_event_wrappers(py::module& m) {
    // ============================================================
    // GameEventWrapper
    // ============================================================
    py::class_<GameEventWrapper, ActorWrapper>(m, "GameEventWrapper",
        R"doc(Base wrapper for game events (matches, training, replays, etc.).

        Provides access to common game event properties: players, cars,
        match state, timers, and game configuration.
        )doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"))
        .def("GetMatchTypeName", &GameEventWrapper::GetMatchTypeName,
            "Get the match type name string.")
        .def("GetReplicatedStateIndex", &GameEventWrapper::GetReplicatedStateIndex)
        .def("SetReplicatedStateIndex", &GameEventWrapper::SetReplicatedStateIndex, py::arg("newReplicatedStateIndex"))
        .def("GetCarArchetype", &GameEventWrapper::GetCarArchetype)
        .def("SetCarArchetype", &GameEventWrapper::SetCarArchetype, py::arg("newCarArchetype"))
        .def("GetCountdownTime", &GameEventWrapper::GetCountdownTime)
        .def("SetCountdownTime", &GameEventWrapper::SetCountdownTime, py::arg("newCountdownTime"))
        .def("GetFinishTime", &GameEventWrapper::GetFinishTime)
        .def("SetFinishTime", &GameEventWrapper::SetFinishTime, py::arg("newFinishTime"))
        .def("GetbMultiplayer", &GameEventWrapper::GetbMultiplayer)
        .def("SetbMultiplayer", &GameEventWrapper::SetbMultiplayer, py::arg("newbMultiplayer"))
        .def("GetbCountdownMessagesDisabled", &GameEventWrapper::GetbCountdownMessagesDisabled)
        .def("GetbFillWithAI", &GameEventWrapper::GetbFillWithAI)
        .def("GetbAllowQueueSaveReplay", &GameEventWrapper::GetbAllowQueueSaveReplay)
        .def("GetbAllowReadyUp", &GameEventWrapper::GetbAllowReadyUp)
        .def("GetbRestartingMatch", &GameEventWrapper::GetbRestartingMatch)
        .def("GetbRandomizedBotLoadouts", &GameEventWrapper::GetbRandomizedBotLoadouts)
        .def("GetbHasLeaveMatchPenalty", &GameEventWrapper::GetbHasLeaveMatchPenalty)
        .def("GetbCanVoteToForfeit", &GameEventWrapper::GetbCanVoteToForfeit)
        .def("GetbDisableAimAssist", &GameEventWrapper::GetbDisableAimAssist)
        .def("GetbAwardAchievements", &GameEventWrapper::GetbAwardAchievements)
        .def("GetMinPlayers", &GameEventWrapper::GetMinPlayers)
        .def("SetMinPlayers", &GameEventWrapper::SetMinPlayers, py::arg("newMinPlayers"))
        .def("GetMaxPlayers", &GameEventWrapper::GetMaxPlayers)
        .def("SetMaxPlayers", &GameEventWrapper::SetMaxPlayers, py::arg("newMaxPlayers"))
        .def("GetSpawnPoints", &GameEventWrapper::GetSpawnPoints)
        .def("GetBotSkill", &GameEventWrapper::GetBotSkill)
        .def("SetBotSkill", &GameEventWrapper::SetBotSkill, py::arg("newBotSkill"))
        .def("GetRespawnTime", &GameEventWrapper::GetRespawnTime)
        .def("SetRespawnTime", &GameEventWrapper::SetRespawnTime, py::arg("newRespawnTime"))
        .def("GetMatchTimeDilation", &GameEventWrapper::GetMatchTimeDilation)
        .def("SetMatchTimeDilation", &GameEventWrapper::SetMatchTimeDilation, py::arg("newMatchTimeDilation"))
        .def("GetActivator", &GameEventWrapper::GetActivator)
        .def("SetActivator", &GameEventWrapper::SetActivator, py::arg("newActivator"))
        .def("GetActivatorCar", &GameEventWrapper::GetActivatorCar)
        .def("SetActivatorCar", &GameEventWrapper::SetActivatorCar, py::arg("newActivatorCar"))
        .def("GetPlayers", &GameEventWrapper::GetPlayers)
        .def("GetPRIs", &GameEventWrapper::GetPRIs)
        .def("GetCars", &GameEventWrapper::GetCars)
        .def("GetLocalPlayers", &GameEventWrapper::GetLocalPlayers)
        .def("GetStartPointIndex", &GameEventWrapper::GetStartPointIndex)
        .def("SetStartPointIndex", &GameEventWrapper::SetStartPointIndex, py::arg("newStartPointIndex"))
        .def("GetGameStateTimeRemaining", &GameEventWrapper::GetGameStateTimeRemaining)
        .def("GetReplicatedGameStateTimeRemaining", &GameEventWrapper::GetReplicatedGameStateTimeRemaining)
        .def("GetGameOwner", &GameEventWrapper::GetGameOwner)
        .def("SetGameOwner", &GameEventWrapper::SetGameOwner, py::arg("newGameOwner"))
        .def("GetReplicatedRoundCountDownNumber", &GameEventWrapper::GetReplicatedRoundCountDownNumber)
        .def("IsPlayingTraining", &GameEventWrapper::IsPlayingTraining)
        .def("IsPlayingLan", &GameEventWrapper::IsPlayingLan)
        .def("IsPlayingOffline", &GameEventWrapper::IsPlayingOffline)
        .def("IsPlayingPrivate", &GameEventWrapper::IsPlayingPrivate)
        .def("IsPlayingPublic", &GameEventWrapper::IsPlayingPublic)
        .def("IsOnlineMultiplayer", &GameEventWrapper::IsOnlineMultiplayer)
        .def("IsFinished", &GameEventWrapper::IsFinished)
        .def("CanUseBallCam", &GameEventWrapper::CanUseBallCam)
        .def("GetLocalPrimaryPlayer", &GameEventWrapper::GetLocalPrimaryPlayer)
        .def("GetPlaylist", &GameEventWrapper::GetPlaylist)
        .def("ForceMatchStart", &GameEventWrapper::ForceMatchStart);

    // ============================================================
    // TeamGameEventWrapper
    // ============================================================
    py::class_<TeamGameEventWrapper, GameEventWrapper>(m, "TeamGameEventWrapper",
        R"doc(Wrapper for team-based game events (extends GameEventWrapper).

        Provides access to teams, team sizes, bot counts, and team-specific
        match configuration.
        )doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"))
        .def("GetTeamArchetypes", &TeamGameEventWrapper::GetTeamArchetypes)
        .def("GetTeams", &TeamGameEventWrapper::GetTeams)
        .def("GetMaxTeamSize", &TeamGameEventWrapper::GetMaxTeamSize)
        .def("SetMaxTeamSize", &TeamGameEventWrapper::SetMaxTeamSize, py::arg("newMaxTeamSize"))
        .def("GetNumBots", &TeamGameEventWrapper::GetNumBots)
        .def("SetNumBots", &TeamGameEventWrapper::SetNumBots, py::arg("newNumBots"))
        .def("GetbMuteOppositeTeams", &TeamGameEventWrapper::GetbMuteOppositeTeams)
        .def("GetbDisableMutingOtherTeam", &TeamGameEventWrapper::GetbDisableMutingOtherTeam)
        .def("GetbForfeit", &TeamGameEventWrapper::GetbForfeit)
        .def("GetbUnfairTeams", &TeamGameEventWrapper::GetbUnfairTeams)
        .def("GetbAlwaysAutoSelectTeam", &TeamGameEventWrapper::GetbAlwaysAutoSelectTeam)
        .def("EndGame", &TeamGameEventWrapper::EndGame)
        .def("ForceNoContest", &TeamGameEventWrapper::ForceNoContest);

    // ============================================================
    // ServerWrapper (the main game state class)
    // ============================================================
    py::class_<ServerWrapper, TeamGameEventWrapper>(m, "ServerWrapper",
        R"doc(Wrapper for the game server state - the main game state class.

        Provides comprehensive access to match state: ball, cars, teams,
        goals, scores, timers, and game flow functions.
        )doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"))

        // Self-implemented
        .def("GetBall", &ServerWrapper::GetBall,
            "Get the ball in the match.")
        .def("SpawnCar", &ServerWrapper::SpawnCar, py::arg("carBody"), py::arg("name"),
            "Spawn a car with given body ID and player name.")
        .def("SpawnBot", &ServerWrapper::SpawnBot, py::arg("carBody"), py::arg("name"),
            "Spawn a bot with given car body and name.")
        .def("SpawnBall", &ServerWrapper::SpawnBall, py::arg("position"), py::arg("wake"), py::arg("spawnCannon"),
            "Spawn a ball at the given position.")
        .def("HasAuthority", &ServerWrapper::HasAuthority,
            "Check whether this client has authority over the game state.")
        .def("GetGameSpeed", &ServerWrapper::GetGameSpeed)
        .def("SetGameSpeed", &ServerWrapper::SetGameSpeed, py::arg("val"))
        .def("GetSecondsElapsed", &ServerWrapper::GetSecondsElapsed)
        .def("SetSecondsElapsed", &ServerWrapper::SetSecondsElapsed, py::arg("val"))
        .def("GetGameCar", &ServerWrapper::GetGameCar)
        .def("IsBallMovingTowardsGoal", &ServerWrapper::IsBallMovingTowardsGoal,
            py::arg("goalNo"), py::arg("bw"))
        .def("IsInGoal", &ServerWrapper::IsInGoal, py::arg("vec"))
        .def("DisableGoalReset", &ServerWrapper::DisableGoalReset)
        .def("EnableGoalReset", &ServerWrapper::EnableGoalReset)
        .def("GenerateShot", &ServerWrapper::GenerateShot,
            py::arg("startPos"), py::arg("destination"), py::arg("speed"),
            "Generate a shot velocity from start to destination at given speed.")
        .def("GenerateGoalAimLocation", &ServerWrapper::GenerateGoalAimLocation,
            py::arg("goalNumber"), py::arg("currentBallLocation"))
        .def("GetGoalExtent", &ServerWrapper::GetGoalExtent, py::arg("goalNumber") = 0)
        .def("GetGoalLocation", &ServerWrapper::GetGoalLocation, py::arg("goalNumber") = 0)

        // Auto-generated fields
        .def("GetTestCarArchetype", &ServerWrapper::GetTestCarArchetype)
        .def("SetTestCarArchetype", &ServerWrapper::SetTestCarArchetype, py::arg("newTestCarArchetype"))
        .def("GetBallArchetype", &ServerWrapper::GetBallArchetype)
        .def("SetBallArchetype", &ServerWrapper::SetBallArchetype, py::arg("newBallArchetype"))
        .def("GetBallSpawnPoint", &ServerWrapper::GetBallSpawnPoint)
        .def("SetBallSpawnPoint", &ServerWrapper::SetBallSpawnPoint, py::arg("newBallSpawnPoint"))
        .def("GetSeriesLength", &ServerWrapper::GetSeriesLength)
        .def("SetSeriesLength", &ServerWrapper::SetSeriesLength, py::arg("newSeriesLength"))
        .def("GetGameTime", &ServerWrapper::GetGameTime)
        .def("SetGameTime", &ServerWrapper::SetGameTime, py::arg("newGameTime"))
        .def("GetWarmupTime", &ServerWrapper::GetWarmupTime)
        .def("SetWarmupTime", &ServerWrapper::SetWarmupTime, py::arg("newWarmupTime"))
        .def("GetMaxScore", &ServerWrapper::GetMaxScore)
        .def("SetMaxScore", &ServerWrapper::SetMaxScore, py::arg("newMaxScore"))
        .def("GetAutoBalanceDifference", &ServerWrapper::GetAutoBalanceDifference)
        .def("SetAutoBalanceDifference", &ServerWrapper::SetAutoBalanceDifference, py::arg("newAutoBalanceDifference"))
        .def("GetScoreSlomoTime", &ServerWrapper::GetScoreSlomoTime)
        .def("SetScoreSlomoTime", &ServerWrapper::SetScoreSlomoTime, py::arg("newScoreSlomoTime"))
        .def("GetGameTimeRemaining", &ServerWrapper::GetGameTimeRemaining)
        .def("SetGameTimeRemaining", &ServerWrapper::SetGameTimeRemaining, py::arg("newGameTimeRemaining"))
        .def("GetSecondsRemaining", &ServerWrapper::GetSecondsRemaining)
        .def("SetSecondsRemaining", &ServerWrapper::SetSecondsRemaining, py::arg("newSecondsRemaining"))
        .def("GetWaitTimeRemaining", &ServerWrapper::GetWaitTimeRemaining)
        .def("SetWaitTimeRemaining", &ServerWrapper::SetWaitTimeRemaining, py::arg("newWaitTimeRemaining"))
        .def("GetTotalGameTimePlayed", &ServerWrapper::GetTotalGameTimePlayed)
        .def("SetTotalGameTimePlayed", &ServerWrapper::SetTotalGameTimePlayed, py::arg("newTotalGameTimePlayed"))
        .def("GetOvertimeTimePlayed", &ServerWrapper::GetOvertimeTimePlayed)
        .def("SetOvertimeTimePlayed", &ServerWrapper::SetOvertimeTimePlayed, py::arg("newOvertimeTimePlayed"))
        .def("GetbRoundActive", &ServerWrapper::GetbRoundActive)
        .def("GetbPlayReplays", &ServerWrapper::GetbPlayReplays)
        .def("GetbBallHasBeenHit", &ServerWrapper::GetbBallHasBeenHit)
        .def("GetbOverTime", &ServerWrapper::GetbOverTime)
        .def("GetbUnlimitedTime", &ServerWrapper::GetbUnlimitedTime)
        .def("GetbNoContest", &ServerWrapper::GetbNoContest)
        .def("GetbDisableGoalDelay", &ServerWrapper::GetbDisableGoalDelay)
        .def("GetbShowNoScorerGoalMessage", &ServerWrapper::GetbShowNoScorerGoalMessage)
        .def("GetbMatchEnded", &ServerWrapper::GetbMatchEnded)
        .def("GetbShowIntroScene", &ServerWrapper::GetbShowIntroScene)
        .def("GetbClubMatch", &ServerWrapper::GetbClubMatch)
        .def("GetNextSpawnIndex", &ServerWrapper::GetNextSpawnIndex)
        .def("SetNextSpawnIndex", &ServerWrapper::SetNextSpawnIndex, py::arg("newNextSpawnIndex"))
        .def("GetReplayDirectorArchetype", &ServerWrapper::GetReplayDirectorArchetype)
        .def("GetReplayDirector", &ServerWrapper::GetReplayDirector)
        .def("GetGameBalls", &ServerWrapper::GetGameBalls)
        .def("GetTotalGameBalls", &ServerWrapper::GetTotalGameBalls)
        .def("SetTotalGameBalls", &ServerWrapper::SetTotalGameBalls, py::arg("newTotalGameBalls"))
        .def("GetPostGoalTime", &ServerWrapper::GetPostGoalTime)
        .def("GetGoals", &ServerWrapper::GetGoals)
        .def("GetSecondsRemainingCountdown", &ServerWrapper::GetSecondsRemainingCountdown)
        .def("GetFieldCenter", &ServerWrapper::GetFieldCenter)
        .def("GetGameWinner", &ServerWrapper::GetGameWinner)
        .def("GetMatchWinner", &ServerWrapper::GetMatchWinner)
        .def("GetMVP", &ServerWrapper::GetMVP)
        .def("GetFastestGoalPlayer", &ServerWrapper::GetFastestGoalPlayer)
        .def("GetSlowestGoalPlayer", &ServerWrapper::GetSlowestGoalPlayer)
        .def("GetFurthestGoalPlayer", &ServerWrapper::GetFurthestGoalPlayer)
        .def("GetFastestGoalSpeed", &ServerWrapper::GetFastestGoalSpeed)
        .def("GetSlowestGoalSpeed", &ServerWrapper::GetSlowestGoalSpeed)
        .def("GetFurthestGoal", &ServerWrapper::GetFurthestGoal)
        .def("GetReplicatedScoredOnTeam", &ServerWrapper::GetReplicatedScoredOnTeam)
        .def("GetReplicatedServerPerformanceState", &ServerWrapper::GetReplicatedServerPerformanceState)
        .def("GetRoundNum", &ServerWrapper::GetRoundNum)
        .def("GetAssistMaxTime", &ServerWrapper::GetAssistMaxTime)
        .def("GetBallHasBeenHitStartDelay", &ServerWrapper::GetBallHasBeenHitStartDelay)
        .def("GetPodiumDelay", &ServerWrapper::GetPodiumDelay)
        .def("GetPodiumTime", &ServerWrapper::GetPodiumTime)
        .def("GetLobbyEndCountdown", &ServerWrapper::GetLobbyEndCountdown)
        .def("GetLobbyCountdown", &ServerWrapper::GetLobbyCountdown)
        .def("GetLobbyTime", &ServerWrapper::GetLobbyTime)
        .def("GetLobbySpawnRestartTime", &ServerWrapper::GetLobbySpawnRestartTime)
        .def("GetPauser", &ServerWrapper::GetPauser)

        // Auto-generated function proxies
        .def("SetNumPlayers", &ServerWrapper::SetNumPlayers, py::arg("numPlayers"))
        .def("GetNumPlayers", &ServerWrapper::GetNumPlayers)
        .def("GetPlayerCarCount", &ServerWrapper::GetPlayerCarCount)
        .def("CheckStart", &ServerWrapper::CheckStart)
        .def("StartLobbyTimer", &ServerWrapper::StartLobbyTimer)
        .def("CanSpawnBots", &ServerWrapper::CanSpawnBots)
        .def("StartRound", &ServerWrapper::StartRound)
        .def("EndRound", &ServerWrapper::EndRound)
        .def("CanAwardPoints", &ServerWrapper::CanAwardPoints)
        .def("StartReplay", &ServerWrapper::StartReplay)
        .def("ForceMatchStart", py::overload_cast<>(&ServerWrapper::ForceMatchStart))
        .def("ForceOvertime", &ServerWrapper::ForceOvertime)
        .def("StartOvertime", &ServerWrapper::StartOvertime)
        .def("GetWinningTeam", &ServerWrapper::GetWinningTeam)
        .def("ResetPickups", &ServerWrapper::ResetPickups)
        .def("ResetPlayers", &ServerWrapper::ResetPlayers)
        .def("DestroyCars", &ServerWrapper::DestroyCars)
        .def("FreezePawns", &ServerWrapper::FreezePawns)
        .def("DestroyBalls", &ServerWrapper::DestroyBalls)
        .def("ResetGame", &ServerWrapper::ResetGame)
        .def("StartNewGame", &ServerWrapper::StartNewGame)
        .def("EndGame", py::overload_cast<>(&ServerWrapper::EndGame))
        .def("HasWinner", &ServerWrapper::HasWinner)
        .def("GetMatchGUID", &ServerWrapper::GetMatchGUID)
        .def("SetMatchGUID", &ServerWrapper::SetMatchGUID, py::arg("s"));

    // ============================================================
    // Replay Wrapper Types
    // ============================================================
    py::class_<ReplayWrapper, GameEventWrapper>(m, "ReplayWrapper",
        R"doc(Wrapper for replay game events.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<ReplayDirectorWrapper, ActorWrapper>(m, "ReplayDirectorWrapper",
        R"doc(Wrapper for replay director/camera control.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<ReplaySoccarWrapper, ReplayWrapper>(m, "ReplaySoccarWrapper",
        R"doc(Wrapper for soccar (soccer) replay events.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<TrainingEditorWrapper, GameEventWrapper>(m, "TrainingEditorWrapper",
        R"doc(Wrapper for the training editor game event.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"))

        // Round / scoring
        .def("GetTotalRounds", &TrainingEditorWrapper::GetTotalRounds,
            "Get the total number of rounds in this training pack.")
        .def("GetScore", &TrainingEditorWrapper::GetScore,
            "Get the current score (total points for all rounds).")
        .def("GetShotAttempt", &TrainingEditorWrapper::GetShotAttempt,
            "Get the current shot attempt number.")
        .def("GetPointsScoredThisRound", &TrainingEditorWrapper::GetPointsScoredThisRound,
            "Get points scored in the current round.")
        .def("GetGoalieScore", &TrainingEditorWrapper::GetGoalieScore,
            "Get the goalie score.")
        .def("SetPointsScoredThisRound", &TrainingEditorWrapper::SetPointsScoredThisRound,
            "Set points scored in the current round.", py::arg("newPointsScoredThisRound"))

        // Round management
        .def("StartNewRound", &TrainingEditorWrapper::StartNewRound,
            "Start a new round.")
        .def("SetupDefaultRound", &TrainingEditorWrapper::SetupDefaultRound,
            "Set up the default round state.")
        .def("IncrementRound", &TrainingEditorWrapper::IncrementRound,
            "Increment to the next round.", py::arg("bLoop") = false)
        .def("SwitchToRoundNumber", &TrainingEditorWrapper::SwitchToRoundNumber,
            "Switch to a specific round number.", py::arg("RoundNumber"), py::arg("BackupCurrentRound") = false)
        .def("DuplicateRound", &TrainingEditorWrapper::DuplicateRound,
            "Duplicate a round at the given index.", py::arg("Index"))
        .def("ReorderRound", &TrainingEditorWrapper::ReorderRound,
            "Move a round from one index to another.", py::arg("FromIndex"), py::arg("ToIndex"))
        .def("DeleteRound", &TrainingEditorWrapper::DeleteRound,
            "Delete a round by index.", py::arg("RoundToDelete"))
        .def("IsValidRoundIndex", &TrainingEditorWrapper::IsValidRoundIndex,
            "Check if a round index is valid.", py::arg("ArrayIndex"))

        // Ball/actor placement
        .def("SpawnBallAndStartPointAt", &TrainingEditorWrapper::SpawnBallAndStartPointAt,
            "Spawn the ball at a given location and the car start point at another.",
            py::arg("BallSpawnLocation"), py::arg("BallSpawnRotation"),
            py::arg("StartPointSpawnLocation"), py::arg("StartPointSpawnRotation"))
        .def("DestroyBall", &TrainingEditorWrapper::DestroyBall,
            "Destroy the current ball.", py::arg("Ball"))
        .def("ResetBalls", &TrainingEditorWrapper::ResetBalls,
            "Reset all balls to their initial positions.")
        .def("BroadcastGoMessage", &TrainingEditorWrapper::BroadcastGoMessage,
            "Broadcast the GO! message to all players.")

        // Time settings
        .def("GetMinRoundTime", &TrainingEditorWrapper::GetMinRoundTime,
            "Get the minimum round time limit in seconds.")
        .def("SetMinRoundTime", &TrainingEditorWrapper::SetMinRoundTime,
            "Set the minimum round time limit in seconds.", py::arg("newMinRoundTime"))
        .def("GetMaxRoundTime", &TrainingEditorWrapper::GetMaxRoundTime,
            "Get the maximum round time limit in seconds.")
        .def("SetMaxRoundTime", &TrainingEditorWrapper::SetMaxRoundTime,
            "Set the maximum round time limit in seconds.", py::arg("newMaxRoundTime"))
        .def("SetRoundTimeLimit", &TrainingEditorWrapper::SetRoundTimeLimit,
            "Set the round time limit.", py::arg("NewRoundTime"))

        // Difficulty / training info
        .def("GetDifficulty", &TrainingEditorWrapper::GetDifficulty,
            "Get the training difficulty (0=rookie, 1=pro, 2=all-star).")
        .def("GetTrainingType", &TrainingEditorWrapper::GetTrainingType,
            "Get the training type.")
        .def("GetTrainingFileName", &TrainingEditorWrapper::GetTrainingFileName,
            "Get the training file name.")

        // Playtest
        .def("StartPlayTest", &TrainingEditorWrapper::StartPlayTest,
            "Start a playtest.", py::arg("InPlayTestType"))
        .def("EndPlayTest", &TrainingEditorWrapper::EndPlayTest,
            "End the current playtest.")
        .def("RestartPlayTest", &TrainingEditorWrapper::RestartPlayTest,
            "Restart the current playtest.")
        .def("CanPlayTestRoundNumber", &TrainingEditorWrapper::CanPlayTestRoundNumber,
            "Check if a round number can be playtested.", py::arg("RoundNumber"))

        // Save/load
        .def("Save2", &TrainingEditorWrapper::Save2,
            "Save the training pack.")
        .def("Load2", &TrainingEditorWrapper::Load2,
            "Load a training pack by name.", py::arg("SaveName"), py::arg("PC"))

        // Misc
        .def("AllowDynamicCrowd", &TrainingEditorWrapper::AllowDynamicCrowd,
            "Check if dynamic crowd is allowed.")
        .def("RemovePointsFromScore", &TrainingEditorWrapper::RemovePointsFromScore,
            "Remove points from a team's score.", py::arg("PointsToRemove"), py::arg("TeamIndex"))
        .def("RemoveAllPointsFromScore", &TrainingEditorWrapper::RemoveAllPointsFromScore,
            "Remove all points from a team's score.", py::arg("TeamIndex"))
        .def("GetbUnsavedChanges", &TrainingEditorWrapper::GetbUnsavedChanges,
            "Check if there are unsaved changes.")
        .def("SetUnsavedChanges", &TrainingEditorWrapper::SetUnsavedChanges,
            "Set the unsaved changes flag.", py::arg("bInUnsavedChanges"))
        .def("SetTracedCrosshairActor", &TrainingEditorWrapper::SetTracedCrosshairActor,
            "Set the traced crosshair actor for a player controller.",
            py::arg("PC"), py::arg("NewActor"))
        .def("HandleNextGame", &TrainingEditorWrapper::HandleNextGame,
            "Handle transitioning to the next game.")
        .def("GetPlayerTeamNumber", &TrainingEditorWrapper::GetPlayerTeamNumber,
            "Get the local player's team number.");

    py::class_<TutorialWrapper, GameEventWrapper>(m, "TutorialWrapper",
        R"doc(Wrapper for tutorial game events.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<GameEditorWrapper, GameEventWrapper>(m, "GameEditorWrapper",
        R"doc(Wrapper for game editor events.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<GameSettingPlaylistWrapper, ObjectWrapper>(m, "GameSettingPlaylistWrapper",
        R"doc(Wrapper for game settings/playlist data.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    // ============================================================
    // Save Data Wrappers
    // ============================================================
    py::class_<SaveDataWrapper, ObjectWrapper>(m, "SaveDataWrapper",
        R"doc(Base wrapper for save data.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<GameEditorSaveDataWrapper, SaveDataWrapper>(m, "GameEditorSaveDataWrapper",
        R"doc(Wrapper for game editor save data.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<TrainingEditorSaveDataWrapper, SaveDataWrapper>(m, "TrainingEditorSaveDataWrapper",
        R"doc(Wrapper for training editor save data.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));
}

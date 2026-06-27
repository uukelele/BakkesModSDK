"""
Game State Access
=================
Demonstrates how to read the complete game state: ball, cars, teams,
scores, timers, and player information.

Key objects:
  - GameWrapper:       Entry point for getting the current game/server state
  - ServerWrapper:     The main game state (ball, cars, teams, goals, timers)
  - BallWrapper:       Ball position, velocity, prediction
  - CarWrapper:        Car physics, controls, state
  - PriWrapper:        Player replication info (stats, name, platform)
  - TeamWrapper:       Team data
"""

from bakkesmod import (
    GameWrapper,
    ServerWrapper,
    BallWrapper,
    CarWrapper,
    PriWrapper,
    TeamWrapper,
    Vector,
    Rotator,
    RBState,
    ControllerInput,
    PredictionInfo,
)


def print_game_state(game_wrapper: GameWrapper) -> None:
    """Print the current game state to the BakkesMod console.

    This function should be called during an active match (freeplay,
    online, or custom training).

    Args:
        game_wrapper: The main SDK entry point.
    """
    # --- Check what mode we're in ---
    if game_wrapper.IsInFreeplay():
        print("[GameState] Mode: Freeplay")
    elif game_wrapper.IsInOnlineGame():
        print("[GameState] Mode: Online Match")
    elif game_wrapper.IsInCustomTraining():
        print("[GameState] Mode: Custom Training")
    elif game_wrapper.IsInReplay():
        print("[GameState] Mode: Replay")
    else:
        print("[GameState] Mode: Unknown/Not in game")

    print(f"[GameState] Paused: {game_wrapper.IsPaused()}")
    print(f"[GameState] Current Map: {game_wrapper.GetCurrentMap()}")

    # --- Access the server/game state ---
    try:
        server: ServerWrapper = game_wrapper.GetCurrentGameState()
        if server.IsNull():
            print("[GameState] No active server state")
            return
    except Exception:
        print("[GameState] Failed to get server state")
        return

    _print_server_state(server)
    _print_ball_state(server)
    _print_cars_state(server)
    _print_teams(server)
    _print_timers(server)


def _print_server_state(server: ServerWrapper) -> None:
    """Print general server/match state.

    Args:
        server: The current game server wrapper.
    """
    print(f"[GameState] Match GUID: {server.GetMatchGUID()}")
    print(f"[GameState] Round: {server.GetRoundNum()}")
    print(f"[GameState] Overtime: {bool(server.GetbOverTime())}")
    print(f"[GameState] Match Ended: {bool(server.GetbMatchEnded())}")
    print(f"[GameState] Game Speed: {server.GetGameSpeed():.2f}x")
    print(f"[GameState] Has Authority: {server.HasAuthority()}")
    print(f"[GameState] Total Players: {server.GetNumPlayers()}")
    print(f"[GameState] Player Car Count: {server.GetPlayerCarCount()}")

    # Goals
    goals = server.GetGoals()
    print(f"[GameState] Goals on field: {goals.Count()}")

    # Winner
    winner: TeamWrapper = server.GetGameWinner()
    if winner is not None and not winner.IsNull():
        print(f"[GameState] Game Winner exists")


def _print_ball_state(server: ServerWrapper) -> None:
    """Print ball position, velocity, and prediction.

    Args:
        server: The current game server wrapper.
    """
    try:
        ball: BallWrapper = server.GetBall()
        if ball.IsNull():
            print("[GameState] Ball: Not available")
            return
    except Exception:
        print("[GameState] Ball: Error getting ball")
        return

    pos: Vector = ball.GetLocation()
    vel: Vector = ball.GetVelocity()
    rb: RBState = ball.GetRBState()

    print(f"[GameState] Ball Position: ({pos.X:.1f}, {pos.Y:.1f}, {pos.Z:.1f})")
    print(f"[GameState] Ball Velocity: ({vel.X:.1f}, {vel.Y:.1f}, {vel.Z:.1f})")
    print(f"[GameState] Ball Speed: {vel.magnitude():.1f} uu/s")
    print(f"[GameState] Ball Radius: {ball.GetRadius():.1f}")
    print(f"[GameState] Ball RBState Location: "
          f"({rb.Location.X:.1f}, {rb.Location.Y:.1f}, {rb.Location.Z:.1f})")
    print(f"[GameState] Ball Sleeping: {bool(rb.bSleeping)}")
    print(f"[GameState] Ball Last Touch Time: {ball.GetLastTouchTime():.3f}s")

    # Ball prediction (predict 1 second ahead)
    pred: PredictionInfo = ball.PredictPosition(1.0)
    print(f"[GameState] Ball Predicted (1s): "
          f"({pred.Location.X:.1f}, {pred.Location.Y:.1f}, {pred.Location.Z:.1f})")
    print(f"[GameState] Ball Predicted Speed: {pred.Velocity.magnitude():.1f}")
    print(f"[GameState] Ball Hit Wall: {bool(pred.bHitWall)}")
    print(f"[GameState] Ball Hit Ground: {bool(pred.bHitGround)}")

    # Check ball type
    if ball.IsDropshotBall():
        print("[GameState] Ball Type: Dropshot")
    elif ball.IsHauntedtBall():
        print("[GameState] Ball Type: Haunted (Ghost Hunt)")
    elif ball.IsGodBall():
        print("[GameState] Ball Type: God Ball (Custom Training)")


def _print_cars_state(server: ServerWrapper) -> None:
    """Print all cars on the field and their physics state.

    Args:
        server: The current game server wrapper.
    """
    print("\n[GameState] --- Cars ---")
    cars = server.GetCars()
    for i in range(cars.Count()):
        car: CarWrapper = cars.Get(i)
        if car.IsNull():
            continue

        pos: Vector = car.GetLocation()
        vel: Vector = car.GetVelocity()
        rot: Rotator = car.GetRotation()
        rb: RBState = car.GetRBState()
        pri: PriWrapper = car.GetPRI()

        player_name = ""
        if pri is not None and not pri.IsNull():
            player_name = pri.GetPlayerName().ToString()

        inp: ControllerInput = car.GetInput()

        print(f"  Car {i}: {player_name or '(no PRI)'}")
        print(f"    Position: ({pos.X:.1f}, {pos.Y:.1f}, {pos.Z:.1f})")
        print(f"    Velocity: ({vel.X:.1f}, {vel.Y:.1f}, {vel.Z:.1f})")
        print(f"    Speed: {vel.magnitude():.1f} uu/s")
        print(f"    Rotation: (P={rot.Pitch}, Y={rot.Yaw}, R={rot.Roll})")
        print(f"    On Ground: {bool(car.GetbOnGround())}")
        print(f"    Super Sonic: {bool(car.GetbSuperSonic())}")
        print(f"    Boost: {bool(car.IsBoostCheap())}")
        print(f"    Has Flip: {bool(car.HasFlip())}")
        print(f"    Dodge: {bool(car.IsDodging())}")
        print(f"    RBState Sleeping: {bool(rb.bSleeping)}")
        print(f"    Input: Throttle={inp.Throttle:.2f}, Steer={inp.Steer:.2f}")


def _print_teams(server: ServerWrapper) -> None:
    """Print team information.

    Args:
        server: The current game server wrapper.
    """
    print("\n[GameState] --- Teams ---")
    teams = server.GetTeams()
    for i in range(teams.Count()):
        team: TeamWrapper = teams.Get(i)
        if team.IsNull():
            continue
        print(f"  Team {i}: exists")


def _print_timers(server: ServerWrapper) -> None:
    """Print match timers and elapsed time.

    Args:
        server: The current game server wrapper.
    """
    print(f"\n[GameState] --- Timers ---")
    print(f"  Seconds Elapsed: {server.GetSecondsElapsed():.1f}")
    print(f"  Game Time Remaining: {server.GetGameTimeRemaining():.1f}")
    print(f"  Seconds Remaining: {server.GetSecondsRemaining()}")
    print(f"  Overtime Played: {server.GetOvertimeTimePlayed():.1f}")
    print(f"  Total Game Time Played: {server.GetTotalGameTimePlayed():.1f}")
    print(f"  Round Active: {bool(server.GetbRoundActive())}")
    print(f"  Round Number: {server.GetRoundNum()}")


def print_local_player_info(game_wrapper: GameWrapper) -> None:
    """Print detailed information about the local player.

    Args:
        game_wrapper: The main SDK entry point.
    """
    print("\n[LocalPlayer] --- Local Player Info ---")

    # Basic identification
    print(f"  Player Name: {game_wrapper.GetPlayerName().ToString()}")
    print(f"  Steam ID: {game_wrapper.GetSteamID()}")
    print(f"  Epic ID: {game_wrapper.GetEpicID()}")
    print(f"  Unique ID: {game_wrapper.GetUniqueID().str()}")
    print(f"  Platform: {game_wrapper.GetUniqueID().GetPlatform()}")
    print(f"  Using Epic: {game_wrapper.IsUsingEpicVersion()}")
    print(f"  Using Steam: {game_wrapper.IsUsingSteamVersion()}")

    # Local car
    local_car: CarWrapper = game_wrapper.GetLocalCar()
    if local_car is not None and not local_car.IsNull():
        pos: Vector = local_car.GetLocation()
        vel: Vector = local_car.GetVelocity()
        print(f"  Local Car Position: ({pos.X:.1f}, {pos.Y:.1f}, {pos.Z:.1f})")
        print(f"  Local Car Speed: {vel.magnitude():.1f}")
        print(f"  Local Car Body ID: {local_car.GetLoadoutBody()}")

    # Controller
    pc = game_wrapper.GetPlayerController()
    if pc is not None and not pc.IsNull():
        print(f"  Using Gamepad: {pc.GetbUsingGamepad()}")
        print(f"  Mouse Sensitivity: {pc.GetMouseSensitivity():.2f}")

    # Screen / display
    screen_size = game_wrapper.GetScreenSize()
    print(f"  Screen Size: ({screen_size.X}, {screen_size.Y})")
    print(f"  Display Scale: {game_wrapper.GetDisplayScale():.2f}")
    print(f"  Interface Scale: {game_wrapper.GetInterfaceScale():.2f}")
    print(f"  UI Language: {game_wrapper.GetUILanguage().ToString()}")
    print(f"  Color Blind: {game_wrapper.GetbColorBlind()}")
    print(f"  Metric: {game_wrapper.GetbMetric()}")

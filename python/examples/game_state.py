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
    CVarManagerWrapper,
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
    NOTIFIER_PERMISSION,
)


class GameStatePlugin:
    """Prints game state to BakkesMod console via notifier commands."""

    def __init__(self) -> None:
        self.game_wrapper: GameWrapper | None = None
        self.cvar_manager: CVarManagerWrapper | None = None

    def on_load(
        self,
        game_wrapper: GameWrapper,
        cvar_manager: CVarManagerWrapper,
    ) -> None:
        """Register game state notifiers.

        Args:
            game_wrapper: The main SDK entry point.
            cvar_manager: Console variable manager.
        """
        self.game_wrapper = game_wrapper
        self.cvar_manager = cvar_manager

        cvar_manager.registerNotifier(
            "gs_state",
            self._on_print_state,
            "Print the current game state (ball, cars, teams, timers).",
            NOTIFIER_PERMISSION.SOCCAR | NOTIFIER_PERMISSION.FREEPLAY | NOTIFIER_PERMISSION.TRAINING,
        )
        cvar_manager.registerNotifier(
            "gs_player",
            self._on_print_player,
            "Print local player info (name, platform, car, screen).",
            NOTIFIER_PERMISSION.ALL,
        )
        cvar_manager.registerNotifier(
            "gs_mode",
            self._on_print_mode,
            "Print current game mode (freeplay, online, replay, etc.).",
            NOTIFIER_PERMISSION.ALL,
        )

        game_wrapper.LogToChatbox(
            "GameStatePlugin loaded. Try: gs_state, gs_player, gs_mode",
            "GameStatePlugin",
        )

    def _on_print_state(self, args: list[str]) -> None:
        """Print ball, cars, teams, and timers.

        Args:
            args: Command arguments.
        """
        if self.game_wrapper is None:
            return

        try:
            server: ServerWrapper = self.game_wrapper.GetCurrentGameState()
            if server.IsNull():
                self._log("No active server state")
                return
        except Exception:
            self._log("Failed to get server state")
            return

        self._print_server_state(server)
        self._print_ball_state(server)
        self._print_cars_state(server)
        self._print_timers(server)

    def _on_print_player(self, args: list[str]) -> None:
        """Print local player info.

        Args:
            args: Command arguments.
        """
        if self.game_wrapper is None:
            return

        gw = self.game_wrapper

        self._log(f"Player Name: {gw.GetPlayerName().ToString()}")
        self._log(f"Steam ID: {gw.GetSteamID()}")
        self._log(f"Epic ID: {gw.GetEpicID()}")
        self._log(f"Platform: {gw.GetUniqueID().GetPlatform()}")

        local_car = gw.GetLocalCar()
        if local_car is not None and not local_car.IsNull():
            pos = local_car.GetLocation()
            vel = local_car.GetVelocity()
            self._log(f"Car Position: ({pos.X:.0f}, {pos.Y:.0f}, {pos.Z:.0f})")
            self._log(f"Car Speed: {vel.magnitude():.0f}")

        screen = gw.GetScreenSize()
        self._log(f"Screen: {screen.X}x{screen.Y}")

    def _on_print_mode(self, args: list[str]) -> None:
        """Print current game mode.

        Args:
            args: Command arguments.
        """
        if self.game_wrapper is None:
            return

        gw = self.game_wrapper

        if gw.IsInFreeplay():
            self._log("Mode: Freeplay")
        elif gw.IsInOnlineGame():
            self._log("Mode: Online Match")
        elif gw.IsInCustomTraining():
            self._log("Mode: Custom Training")
        elif gw.IsInReplay():
            self._log("Mode: Replay")
        else:
            self._log("Mode: Not in game")

        self._log(f"Map: {gw.GetCurrentMap()}")
        self._log(f"Paused: {gw.IsPaused()}")

    def _print_server_state(self, server: ServerWrapper) -> None:
        """Print server match state.

        Args:
            server: The current server wrapper.
        """
        self._log(f"Match GUID: {server.GetMatchGUID()}")
        self._log(f"Round: {server.GetRoundNum()}")
        self._log(f"Overtime: {bool(server.GetbOverTime())}")
        self._log(f"Match Ended: {bool(server.GetbMatchEnded())}")
        self._log(f"Game Speed: {server.GetGameSpeed():.2f}x")
        self._log(f"Players: {server.GetNumPlayers()}")
        self._log(f"Goals: {server.GetGoals().Count()}")

    def _print_ball_state(self, server: ServerWrapper) -> None:
        """Print ball position, velocity, prediction.

        Args:
            server: The current server wrapper.
        """
        try:
            ball: BallWrapper = server.GetBall()
            if ball.IsNull():
                self._log("Ball: Not available")
                return
        except Exception:
            self._log("Ball: Error getting ball")
            return

        pos = ball.GetLocation()
        vel = ball.GetVelocity()
        rb = ball.GetRBState()

        self._log(f"Ball: ({pos.X:.1f}, {pos.Y:.1f}, {pos.Z:.1f})")
        self._log(f"Ball Speed: {vel.magnitude():.1f}")
        self._log(f"Ball Radius: {ball.GetRadius():.1f}")
        self._log(f"Ball Sleeping: {bool(rb.bSleeping)}")

        pred: PredictionInfo = ball.PredictPosition(1.0)
        self._log(f"Ball Predicted (1s): ({pred.Location.X:.1f}, {pred.Location.Y:.1f}, {pred.Location.Z:.1f})")

    def _print_cars_state(self, server: ServerWrapper) -> None:
        """Print all cars on the field.

        Args:
            server: The current server wrapper.
        """
        cars = server.GetCars()
        for i in range(cars.Count()):
            car: CarWrapper = cars.Get(i)
            if car.IsNull():
                continue

            pos = car.GetLocation()
            vel = car.GetVelocity()
            pri: PriWrapper = car.GetPRI()
            name = pri.GetPlayerName().ToString() if pri and not pri.IsNull() else "(no PRI)"

            self._log(f"Car {i}: {name}")
            self._log(f"  Position: ({pos.X:.1f}, {pos.Y:.1f}, {pos.Z:.1f})")
            self._log(f"  Speed: {vel.magnitude():.1f}")
            self._log(f"  On Ground: {bool(car.GetbOnGround())}")
            self._log(f"  Supersonic: {bool(car.GetbSuperSonic())}")
            self._log(f"  Has Flip: {bool(car.HasFlip())}")

    def _print_timers(self, server: ServerWrapper) -> None:
        """Print match timers.

        Args:
            server: The current server wrapper.
        """
        self._log(f"Seconds Elapsed: {server.GetSecondsElapsed():.1f}")
        self._log(f"Time Remaining: {server.GetGameTimeRemaining():.1f}")
        self._log(f"Overtime Played: {server.GetOvertimeTimePlayed():.1f}")

    def _log(self, message: str) -> None:
        """Log a message.

        Args:
            message: The message to log.
        """
        if self.cvar_manager:
            self.cvar_manager.log(f"[GameState] {message}")

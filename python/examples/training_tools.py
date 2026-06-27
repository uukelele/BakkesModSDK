"""
Training Tools
==============
Demonstrates training-related APIs: freeplay ball/car manipulation,
custom training round control, ball prediction for shot practice,
and GFX training data queries.

Key APIs:
  - GameWrapper.IsInFreeplay / IsInCustomTraining
  - ServerWrapper.SetBallSpawnPoint / SetBallArchetype
  - CarWrapper.SetCarRotation / SetCarColor / ForceBoost
  - TrainingEditorWrapper: round control, ball reset, scoring
  - GfxDataTrainingWrapper: launch training, check progress
  - GameWrapper.GetBallPrediction: predict ball trajectory
"""

import math
from typing import Any

from bakkesmod import (
    GameWrapper,
    CVarManagerWrapper,
    ServerWrapper,
    TrainingEditorWrapper,
    GfxDataTrainingWrapper,
    CarWrapper,
    BallWrapper,
    Vector,
    Rotator,
    LinearColor,
    NOTIFIER_PERMISSION,
)


class TrainingTools:
    """Demonstrates training-related features."""

    def __init__(self) -> None:
        self.game_wrapper: GameWrapper | None = None
        self.cvar_manager: CVarManagerWrapper | None = None
        self._fast_forward_enabled: bool = False

    def on_load(
        self,
        game_wrapper: GameWrapper,
        cvar_manager: CVarManagerWrapper,
    ) -> None:
        """Register training tool notifiers.

        Args:
            game_wrapper: The main SDK entry point.
            cvar_manager: Console variable manager.
        """
        self.game_wrapper = game_wrapper
        self.cvar_manager = cvar_manager

        cvar_manager.registerNotifier(
            "training_info",
            self._on_training_info,
            "Show current training state information.",
            NOTIFIER_PERMISSION.ALL,
        )
        cvar_manager.registerNotifier(
            "training_ball_here",
            self._on_ball_here,
            "Place the ball at your car's position.",
            NOTIFIER_PERMISSION.FREEPLAY,
        )
        cvar_manager.registerNotifier(
            "training_reset_ball",
            self._on_reset_ball,
            "Reset the ball spawn point to default.",
            NOTIFIER_PERMISSION.FREEPLAY,
        )
        cvar_manager.registerNotifier(
            "training_next_round",
            self._on_next_round,
            "Advance to the next training round.",
            NOTIFIER_PERMISSION.TRAINING,
        )
        cvar_manager.registerNotifier(
            "training_round_info",
            self._on_round_info,
            "Show current round info (index, score, shot).",
            NOTIFIER_PERMISSION.TRAINING,
        )
        cvar_manager.registerNotifier(
            "training_ball_prediction",
            self._on_ball_prediction,
            "Show where the ball will be in N seconds. Usage: training_ball_prediction [seconds=2.0]",
            NOTIFIER_PERMISSION.FREEPLAY,
        )

        game_wrapper.LogToChatbox(
            "TrainingTools loaded. Try: training_info, training_ball_here, "
            "training_next_round, training_ball_prediction",
            "TrainingTools",
        )

    def _on_training_info(self, args: list[str]) -> None:
        """Display current training mode info.

        Args:
            args: Command arguments.
        """
        if self.game_wrapper is None:
            return

        gw = self.game_wrapper

        if gw.IsInFreeplay():
            self._log("Mode: Freeplay")
            if self._get_server():
                server = self._get_server()
                spawn_point = server.GetBallSpawnPoint()
                self._log(f"  Ball spawn point: ({spawn_point.X:.0f}, {spawn_point.Y:.0f}, {spawn_point.Z:.0f})")

                # Training data
                gfx: GfxDataTrainingWrapper = gw.GetGfxTrainingData()
                if gfx and not gfx.IsNull():
                    freeplay_map = gfx.GetSelectedFreeplayMap()
                    self._log(f"  Freeplay map: {freeplay_map}")

        elif gw.IsInCustomTraining():
            self._log("Mode: Custom Training")
            gfx = gw.GetGfxTrainingData()
            if gfx and not gfx.IsNull():
                self._log(f"  Training type: {gfx.GetTrainingType()}")
                self._log(f"  Current round: {gfx.GetCurrentRound()} / {gfx.GetTotalRounds()}")
                self._log(f"  Score: {gfx.GetCurrentScore()}")
                difficulty = gfx.GetDifficulty().ToString() if gfx.GetDifficulty() else "Unknown"
                self._log(f"  Difficulty: {difficulty}")
        else:
            self._log("Mode: Not in training (in a match or menu)")

    def _on_ball_here(self, args: list[str]) -> None:
        """Place the ball at the local car's position.

        Args:
            args: Command arguments.
        """
        if self.game_wrapper is None:
            return

        car = self._get_local_car()
        if car is None or car.IsNull():
            self._log("No local car found.")
            return

        location = car.GetLocation()
        rotation = car.GetRotation()

        server = self._get_server()
        if server is None:
            return

        server.SetBallSpawnPoint(location)
        self._log(f"Ball spawn point set to ({location.X:.0f}, {location.Y:.0f}, {location.Z:.0f})")

        # Also visually respawn the ball at this location
        ball = self._get_ball()
        if ball and not ball.IsNull():
            ball.SetLocation(location)
            ball.SetRotation(rotation)
            self._log("Ball moved to position.")

    def _on_reset_ball(self, args: list[str]) -> None:
        """Reset the ball to the default spawn point.

        Args:
            args: Command arguments.
        """
        server = self._get_server()
        if server is None:
            return

        server.SetBallSpawnPoint(Vector(0, 0, 93.15))
        self._log("Ball spawn point reset to center field.")

    def _on_next_round(self, args: list[str]) -> None:
        """Advance to the next round in custom training.

        Args:
            args: Command arguments.
        """
        training = self._get_training_editor()
        if training is None or training.IsNull():
            self._log("Not in a custom training session.")
            return

        gfx = self._get_gfx_training()
        if gfx is None or gfx.IsNull():
            return

        current = gfx.GetCurrentRound()
        total = gfx.GetTotalRounds()

        if current < total:
            training.SwitchToRoundNumber(current + 1)
            self._log(f"Switched to round {current + 1}/{total}")
        else:
            self._log(f"Already at the last round ({current}/{total})")

    def _on_round_info(self, args: list[str]) -> None:
        """Show current round details.

        Args:
            args: Command arguments.
        """
        training = self._get_training_editor()
        if training is None or training.IsNull():
            self._log("Not in a custom training session.")
            return

        self._log(f"Total rounds: {training.GetTotalRounds()}")
        self._log(f"Score: {training.GetScore()}")
        self._log(f"Shot attempt: {training.GetShotAttempt()}")
        self._log(f"Difficulty: {training.GetDifficulty()}")
        self._log(f"Unsaved changes: {training.GetbUnsavedChanges()}")
        self._log(f"Min round time: {training.GetMinRoundTime():.1f}s")
        self._log(f"Max round time: {training.GetMaxRoundTime():.1f}s")

        # File name
        fname = training.GetTrainingFileName()
        if fname and not fname.IsNull():
            self._log(f"Training file: {fname.ToString()}")

    def _on_ball_prediction(self, args: list[str]) -> None:
        """Predict where the ball will be in N seconds.

        Args:
            args: [command_name, seconds]
        """
        if self.game_wrapper is None:
            return

        seconds = float(args[1]) if len(args) > 1 else 2.0

        prediction = self.game_wrapper.GetBallPrediction(seconds)
        if prediction.IsNull():
            self._log("No ball prediction available.")
            return

        time_steps = int(seconds / 0.016)  # ~60 prediction steps

        self._log(f"=== Ball Prediction ({seconds}s) ===")
        self._log(f"Location: ({prediction.Location.X:.1f}, {prediction.Location.Y:.1f}, {prediction.Location.Z:.1f})")
        self._log(f"Velocity: ({prediction.Velocity.X:.1f}, {prediction.Velocity.Y:.1f}, {prediction.Velocity.Z:.1f})")
        self._log(f"Time: {prediction.Time:.3f}s")

        # Estimate where to meet the ball
        car = self._get_local_car()
        if car and not car.IsNull():
            car_loc = car.GetLocation()
            dx = prediction.Location.X - car_loc.X
            dy = prediction.Location.Y - car_loc.Y
            dz = prediction.Location.Z - car_loc.Z
            distance = math.sqrt(dx * dx + dy * dy + dz * dz)
            self._log(f"Distance from car: {distance:.0f} units")

            if distance > 0 and seconds > 0:
                speed_needed = distance / seconds
                self._log(f"Required speed: {speed_needed:.0f} u/s")
                self._log(f"Car speed: {car.GetVelocity().Magnitude():.0f} u/s")

    def _get_server(self) -> ServerWrapper | None:
        """Get the current game server.

        Returns:
            The server wrapper, or None.
        """
        if self.game_wrapper is None:
            return None
        game_event = self.game_wrapper.GetCurrentGameEvent()
        if game_event.IsNull():
            return None
        return game_event.GetServer()

    def _get_local_car(self) -> CarWrapper | None:
        """Get the local player's car.

        Returns:
            The car wrapper, or None.
        """
        if self.game_wrapper is None:
            return None
        car = self.game_wrapper.GetLocalCar()
        if car.IsNull():
            return None
        return car

    def _get_ball(self) -> BallWrapper | None:
        """Get the ball.

        Returns:
            The ball wrapper, or None.
        """
        if self.game_wrapper is None:
            return None
        game_event = self.game_wrapper.GetCurrentGameEvent()
        if game_event.IsNull():
            return None
        server = game_event.GetServer()
        if server.IsNull():
            return None
        ball = server.GetBall()
        if ball.IsNull():
            return None
        return ball

    def _get_training_editor(self) -> TrainingEditorWrapper | None:
        """Get the training editor wrapper.

        Returns:
            The training editor wrapper, or None.
        """
        if self.game_wrapper is None:
            return None
        game_event = self.game_wrapper.GetCurrentGameEvent()
        if game_event.IsNull():
            return None
        # Downcast to TrainingEditorWrapper
        editor = TrainingEditorWrapper(game_event.GetMemoryAddress())
        if editor.IsNull():
            return None
        return editor

    def _get_gfx_training(self) -> GfxDataTrainingWrapper | None:
        """Get the training GFX data wrapper.

        Returns:
            The GFX training data wrapper, or None.
        """
        if self.game_wrapper is None:
            return None
        gfx = self.game_wrapper.GetGfxTrainingData()
        if gfx.IsNull():
            return None
        return gfx

    def _log(self, message: str) -> None:
        """Log a message.

        Args:
            message: The message to log.
        """
        if self.cvar_manager:
            self.cvar_manager.log(f"[TrainingTools] {message}")


# ------------------------------------------------------------------
# Utility functions for training scripts
# ------------------------------------------------------------------

def set_car_color_for_training(
    game_wrapper: GameWrapper,
    main_color: tuple[float, float, float, float],
    secondary_color: tuple[float, float, float, float],
) -> None:
    """Set the local car's colors (only works in freeplay).

    Args:
        game_wrapper: The main SDK entry point.
        main_color: RGBA tuple for the primary color (0-1).
        secondary_color: RGBA tuple for the secondary color (0-1).
    """
    car = game_wrapper.GetLocalCar()
    if car.IsNull():
        return

    main = LinearColor(main_color[0], main_color[1], main_color[2], main_color[3])
    secondary = LinearColor(
        secondary_color[0], secondary_color[1],
        secondary_color[2], secondary_color[3],
    )
    car.SetCarColor(main, secondary)


def teleport_ball_to_target(
    game_wrapper: GameWrapper,
    target_location: Vector,
    target_rotation: Vector,
) -> None:
    """Teleport the ball to a specific location (freeplay).

    Args:
        game_wrapper: The main SDK entry point.
        target_location: Desired ball position.
        target_rotation: Desired ball rotation.
    """
    server = game_wrapper.GetCurrentGameEvent().GetServer()
    ball = server.GetBall()
    if ball.IsNull():
        return

    ball.SetLocation(target_location)
    ball.SetRotation(Rotator(int(target_rotation.X), int(target_rotation.Y), int(target_rotation.Z)))


def apply_freeplay_boost(
    game_wrapper: GameWrapper,
    enable: bool = True,
) -> None:
    """Enable or disable infinite boost for the local car.

    Args:
        game_wrapper: The main SDK entry point.
        enable: True for infinite boost, False to disable.
    """
    car = game_wrapper.GetLocalCar()
    if car.IsNull():
        return
    car.ForceBoost(enable)

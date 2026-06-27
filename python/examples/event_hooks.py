"""
Event Hooks
===========
Demonstrates how to hook game events to respond to in-game actions.

BakkesMod fires events for almost everything that happens:
  - Match lifecycle: round start/end, goal scored, match end
  - Player actions: jump, boost, dodge, demo, touch
  - Ball events: hit, goal, spawn
  - Training: reset, shot added

Common event name patterns:
  "Function GameEvent_Soccar_TA.Active.StartRound"
  "Function GameEvent_Soccar_TA.Active.OnGoalScored"
  "Function TAGame.Ball_TA.EventHitGround"
  "Function TAGame.Car_TA.OnJumpPressed"
  "Function GameEvent_Soccar_TA.Active.MatchEnded"
  "Function TAGame.Car_TA.EventDemolished"
  "Function TAGame.Ball_TA.OnCarTouch"

You can also use HookEventWithCaller (via the C++ API) to get the caller object.
"""

from bakkesmod import (
    GameWrapper,
    CVarManagerWrapper,
    NOTIFIER_PERMISSION,
    ServerWrapper,
    CarWrapper,
    BallWrapper,
)


class EventLogger:
    """Demonstrates hooking multiple game events and logging them."""

    def __init__(self) -> None:
        self.game_wrapper: GameWrapper | None = None
        self.cvar_manager: CVarManagerWrapper | None = None
        self.goal_count: int = 0
        self._hooks: list[str] = []

    def on_load(
        self,
        game_wrapper: GameWrapper,
        cvar_manager: CVarManagerWrapper,
    ) -> None:
        """Register all event hooks.

        Args:
            game_wrapper: The main SDK entry point.
            cvar_manager: Console variable manager.
        """
        self.game_wrapper = game_wrapper
        self.cvar_manager = cvar_manager

        self._register_hook(
            "Function GameEvent_Soccar_TA.Active.StartRound",
            self._on_start_round,
        )
        self._register_hook(
            "Function TAGame.Ball_TA.EventHitGoal",
            self._on_goal_scored,
        )
        self._register_hook(
            "Function TAGame.Car_TA.OnJumpPressed",
            self._on_jump,
        )
        self._register_hook(
            "Function TAGame.Car_TA.EventDemolished",
            self._on_demolish,
        )
        self._register_hook(
            "Function TAGame.Ball_TA.OnCarTouch",
            self._on_ball_touch,
        )
        self._register_hook(
            "Function GameEvent_Soccar_TA.Active.MatchEnded",
            self._on_match_ended,
        )
        self._register_hook(
            "Function TAGame.Car_TA.EventSuperSonicChanged",
            self._on_supersonic,
        )

        # Notifier to toggle event logging
        cvar_manager.registerNotifier(
            "event_logger_status",
            self._on_status_command,
            "Show event logger status and toggle hooks.",
            NOTIFIER_PERMISSION.ALL,
        )

        game_wrapper.LogToChatbox(
            "EventLogger: tracking goals, jumps, demos, touches...",
            "EventLogger",
        )

    def on_unload(self) -> None:
        """Unregister all hooks."""
        if self.game_wrapper:
            for hook in self._hooks:
                self.game_wrapper.UnhookEvent(hook)
        self._hooks.clear()

    def _register_hook(
        self, event_name: str, callback
    ) -> None:
        """Register a hook and track it for cleanup.

        Args:
            event_name: The game event name to hook.
            callback: Function to call when the event fires.
        """
        if self.game_wrapper:
            self.game_wrapper.HookEvent(event_name, callback)
            self._hooks.append(event_name)

    # ------------------------------------------------------------------
    # Event Callbacks
    # ------------------------------------------------------------------

    def _on_start_round(self, event_name: str) -> None:
        """Called when a round starts.

        Args:
            event_name: The event that fired.
        """
        self._log(f"[Event] Round started ({event_name})")
        if self.game_wrapper and self.game_wrapper.IsInGame():
            try:
                server = self.game_wrapper.GetCurrentGameState()
                round_num = server.GetRoundNum()
                self._log(f"[Event] Round number: {round_num}")
            except Exception:
                pass

    def _on_goal_scored(self, event_name: str) -> None:
        """Called when a goal is scored (ball hits goal).

        Args:
            event_name: The event that fired.
        """
        self.goal_count += 1
        self._log(f"[Event] GOAL SCORED! Total: {self.goal_count}")

        if self.game_wrapper:
            try:
                server = self.game_wrapper.GetCurrentGameState()
                ball = server.GetBall()
                pos = ball.GetLocation()
                vel = ball.GetVelocity()
                self._log(
                    f"[Event] Goal ball at ({pos.X:.0f}, {pos.Y:.0f}), "
                    f"speed: {vel.magnitude():.0f} uu/s"
                )
            except Exception:
                pass

    def _on_jump(self, event_name: str) -> None:
        """Called when a car jumps.

        Args:
            event_name: The event that fired.
        """
        self._log(f"[Event] Jump detected")

    def _on_demolish(self, event_name: str) -> None:
        """Called when a car is demolished.

        Args:
            event_name: The event that fired.
        """
        self._log(f"[Event] DEMOLISH!")

    def _on_ball_touch(self, event_name: str) -> None:
        """Called when a car touches the ball.

        Args:
            event_name: The event that fired.
        """
        if self.game_wrapper:
            try:
                server = self.game_wrapper.GetCurrentGameState()
                ball = server.GetBall()
                vel = ball.GetVelocity()
                speed = vel.magnitude()
                self._log(f"[Event] Ball touched (speed: {speed:.0f} uu/s)")
            except Exception:
                self._log(f"[Event] Ball touched")

    def _on_match_ended(self, event_name: str) -> None:
        """Called when the match ends.

        Args:
            event_name: The event that fired.
        """
        self._log(f"[Event] Match ended! Goals scored: {self.goal_count}")
        try:
            server = self.game_wrapper.GetCurrentGameState()
            winner = server.GetMatchWinner()
            mvp = server.GetMVP()
            self._log(f"[Event] Winner team exists: {not winner.IsNull()}")
            self._log(f"[Event] MVP exists: {not mvp.IsNull()}")
        except Exception:
            pass
        self.goal_count = 0

    def _on_supersonic(self, event_name: str) -> None:
        """Called when a car goes supersonic or loses it.

        Args:
            event_name: The event that fired.
        """
        pass  # Uncomment below to log every supersonic event
        # self._log("[Event] Supersonic state changed")

    def _on_status_command(self, args: list[str]) -> None:
        """Handle the status notifier command.

        Args:
            args: Command arguments.
        """
        self._log(
            f"[EventLogger] Goal count this match: {self.goal_count}"
        )
        self._log(f"[EventLogger] Active hooks: {len(self._hooks)}")

    def _log(self, message: str) -> None:
        """Log a message to the console and optionally chat.

        Args:
            message: The message to log.
        """
        if self.cvar_manager:
            self.cvar_manager.log(message)

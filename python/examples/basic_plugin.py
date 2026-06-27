"""
Basic Plugin Structure
======================
Demonstrates the fundamental structure of a BakkesMod plugin using Python bindings.

A BakkesMod plugin follows this lifecycle:
  1. onLoad()    - Called when the plugin is loaded. Register cvars, notifiers, hooks here.
  2. onUnload()  - Called when the plugin is unloaded. Cleanup is automatic for most things.

The plugin interacts with the game through two main objects:
  - gameWrapper:    The main API entry point (GameWrapper)
  - cvarManager:    Console variable/notifier registration (CVarManagerWrapper)
"""

from bakkesmod import (
    GameWrapper,
    CVarManagerWrapper,
    PLUGINTYPE,
    NOTIFIER_PERMISSION,
    Vector,
    ServerWrapper,
)


class BasicPlugin:
    """A minimal BakkesMod plugin example.

    This class mirrors the C++ BakkesModPlugin base class pattern.
    In a real plugin, you would instantiate this and pass it to the
    BAKKESMOD_PLUGIN equivalent registration logic.
    """

    def __init__(self) -> None:
        self.game_wrapper: GameWrapper | None = None
        self.cvar_manager: CVarManagerWrapper | None = None
        self._drawable_registered: bool = False

    def on_load(
        self,
        game_wrapper: GameWrapper,
        cvar_manager: CVarManagerWrapper,
    ) -> None:
        """Called when the plugin is loaded by BakkesMod.

        Args:
            game_wrapper: The main SDK entry point for game state access.
            cvar_manager: Console variable and notifier registration.
        """
        self.game_wrapper = game_wrapper
        self.cvar_manager = cvar_manager

        # --- Register a console notifier (command) ---
        # Users can run this from the F6 console:  my_plugin_command arg1 arg2
        self.cvar_manager.registerNotifier(
            "my_plugin_command",
            self._on_my_command,
            "Example notifier: demonstrates console command handling.",
            NOTIFIER_PERMISSION.ALL,
        )

        # --- Register a console variable (cvar) ---
        # Users can read/write:  my_plugin_setting 42
        self.my_setting = self.cvar_manager.registerCvar(
            "my_plugin_setting",
            "10",
            "An example plugin setting (integer).",
            True,   # searchable
            True,   # has minimum
            0.0,    # min value
            True,   # has maximum
            100.0,  # max value
            True,   # save to config
        )

        # --- Hook a game event ---
        # This callback fires every time the specified game event occurs.
        self.game_wrapper.HookEvent(
            "Function GameEvent_Soccar_TA.Active.StartRound",
            self._on_round_start,
        )

        # Log a message to the in-game chatbox
        self.game_wrapper.LogToChatbox(
            "BasicPlugin loaded successfully! Run 'my_plugin_command' in console.",
            "BasicPlugin",
        )

    def on_unload(self) -> None:
        """Called when the plugin is being unloaded.

        Note: Notifiers, cvars, hooks, and drawables are automatically
        cleaned up by BakkesMod. Only explicit cleanup needed for
        custom resources.
        """
        self.game_wrapper = None
        self.cvar_manager = None

    # ------------------------------------------------------------------
    # Callback: Console notifier
    # ------------------------------------------------------------------
    def _on_my_command(self, args: list[str]) -> None:
        """Handle the 'my_plugin_command' console notifier.

        Args:
            args: List of arguments passed after the command name.
        """
        # args[0] is the command name itself ("my_plugin_command")
        # args[1:] are user-provided arguments
        print(f"[BasicPlugin] Command invoked with args: {args}")

        if len(args) > 1:
            subcommand = args[1].lower()
            if subcommand == "status":
                self._print_status()
            elif subcommand == "hello":
                self.game_wrapper.LogToChatbox("Hello from BasicPlugin!", "BasicPlugin")
            else:
                self.game_wrapper.LogToChatbox(
                    f"Unknown subcommand: {subcommand}", "BasicPlugin"
                )
        else:
            self.game_wrapper.LogToChatbox(
                "Usage: my_plugin_command [status|hello]", "BasicPlugin"
            )

    def _print_status(self) -> None:
        """Print current plugin status to the console."""
        if self.cvar_manager is None:
            return
        val = self.my_setting.getIntValue()
        self.cvar_manager.log(f"[BasicPlugin] Setting value: {val}")
        self.cvar_manager.log(
            f"[BasicPlugin] In game: {self.game_wrapper.IsInGame()}"
        )
        self.cvar_manager.log(
            f"[BasicPlugin] In freeplay: {self.game_wrapper.IsInFreeplay()}"
        )

    # ------------------------------------------------------------------
    # Callback: Game event hook
    # ------------------------------------------------------------------
    def _on_round_start(self, event_name: str) -> None:
        """Called when a new round starts (goal reset, match start, etc.).

        Args:
            event_name: The name of the event that fired.
        """
        if self.game_wrapper is None:
            return

        try:
            server: ServerWrapper = self.game_wrapper.GetCurrentGameState()
            ball = server.GetBall()
            if ball is not None and not ball.IsNull():
                pos: Vector = ball.GetLocation()
                self.cvar_manager.log(
                    f"[BasicPlugin] Round started! Ball at "
                    f"({pos.X:.1f}, {pos.Y:.1f}, {pos.Z:.1f})"
                )
        except Exception as e:
            print(f"[BasicPlugin] Error in round start callback: {e}")

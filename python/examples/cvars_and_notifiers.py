"""
Console Variables and Notifiers
================================
Demonstrates how to register console commands (notifiers) and variables (cvars),
and how to bind console keys.

Notifiers:  Console commands that call a Python function
Cvars:      Variables that can be read/written from the console
Binds:      Key bindings that execute notifiers
Aliases:    Shortcuts for longer commands
"""

from bakkesmod import (
    CVarManagerWrapper,
    CVarWrapper,
    GameWrapper,
    LinearColor,
    NOTIFIER_PERMISSION,
    Vector,
    Vector2F,
)


class CvarDemo:
    """Demonstrates advanced cvar and notifier usage."""

    def __init__(self) -> None:
        self.cvar_manager: CVarManagerWrapper | None = None
        self.game_wrapper: GameWrapper | None = None

        # Cvars registered in on_load
        self.boost_amount: CVarWrapper | None = None
        self.display_color: CVarWrapper | None = None
        self.speed_mult: CVarWrapper | None = None
        self.demo_mode: CVarWrapper | None = None

    def on_load(
        self,
        game_wrapper: GameWrapper,
        cvar_manager: CVarManagerWrapper,
    ) -> None:
        """Register all cvars and notifiers.

        Args:
            game_wrapper: The main SDK entry point.
            cvar_manager: Console variable manager.
        """
        self.game_wrapper = game_wrapper
        self.cvar_manager = cvar_manager

        self._register_cvars()
        self._register_notifiers()
        self._register_bindings()

        game_wrapper.LogToChatbox(
            "CvarDemo loaded. Try: cvar_help, boost 100, set_team_color 1",
            "CvarDemo",
        )

    # ------------------------------------------------------------------
    # Cvar Registration
    # ------------------------------------------------------------------

    def _register_cvars(self) -> None:
        """Register all console variables.

        Cvar signature:
          registerCvar(name, defaultValue, desc, searchable, hasMin, min,
                      hasMax, max, saveToCfg)
        """
        self.boost_amount = self.cvar_manager.registerCvar(
            "cd_boost_amount",
            "100",
            "Boost amount to apply with 'boost' command (0-100).",
            True,   # searchable
            True,   # has minimum
            0.0,    # min
            True,   # has maximum
            100.0,  # max
            True,   # save to config
        )

        self.display_color = self.cvar_manager.registerCvar(
            "cd_display_color",
            "(255, 255, 255, 255)",
            "Display color as (R, G, B, A) with 0-255 range.",
            True, False, 0.0, False, 0.0, True,
        )

        self.speed_mult = self.cvar_manager.registerCvar(
            "cd_speed_multiplier",
            "1.0",
            "Car speed multiplier (0.1 - 5.0).",
            True, True, 0.1, True, 5.0, False,
        )

        self.demo_mode = self.cvar_manager.registerCvar(
            "cd_demo_mode",
            "0",
            "Enable demo mode (0=off, 1=on).",
            True, True, 0.0, True, 1.0, True,
        )

    # ------------------------------------------------------------------
    # Notifier Registration
    # ------------------------------------------------------------------

    def _register_notifiers(self) -> None:
        """Register all console notifier commands."""
        self.cvar_manager.registerNotifier(
            "cvar_help",
            self._on_help,
            "Show all CvarDemo commands.",
            NOTIFIER_PERMISSION.ALL,
        )

        self.cvar_manager.registerNotifier(
            "boost",
            self._on_boost,
            "Set local car boost. Usage: boost <amount 0-100>",
            NOTIFIER_PERMISSION.SOCCAR | NOTIFIER_PERMISSION.FREEPLAY,
        )

        self.cvar_manager.registerNotifier(
            "set_team_color",
            self._on_set_team_color,
            "Set car color. Usage: set_team_color <team 0/1> "
            "[r] [g] [b]",
            NOTIFIER_PERMISSION.FREEPLAY,
        )

        self.cvar_manager.registerNotifier(
            "speed_demo",
            self._on_speed_demo,
            "Demonstrate speed multiplier effect.",
            NOTIFIER_PERMISSION.FREEPLAY,
        )

        self.cvar_manager.registerNotifier(
            "cvar_inspect",
            self._on_inspect,
            "Inspect all registered cvars for this plugin.",
            NOTIFIER_PERMISSION.ALL,
        )

    # ------------------------------------------------------------------
    # Key Bindings
    # ------------------------------------------------------------------

    def _register_bindings(self) -> None:
        """Register example key bindings."""
        # Bind F6 key to show help
        self.cvar_manager.setBind("F6", "cvar_help")
        # Bind F7 to boost
        self.cvar_manager.setBind("F7", "boost 100")
        # Bind F8 to toggle demo mode
        self.cvar_manager.setBind("F8", "toggle cd_demo_mode")

        # Set an alias
        self.cvar_manager.setAlias(
            "bdemo",
            "cd_demo_mode 1; boost 100; speed_demo",
        )

    # ------------------------------------------------------------------
    # Notifier Callbacks
    # ------------------------------------------------------------------

    def _on_help(self, args: list[str]) -> None:
        """Show help for all registered commands.

        Args:
            args: Command arguments.
        """
        self._log("=== CvarDemo Commands ===")
        self._log("  cvar_help          - Show this help")
        self._log("  boost <amount>     - Set boost (0-100)")
        self._log("  set_team_color <team> [r] [g] [b]")
        self._log("                     - Set car color")
        self._log("  speed_demo         - Speed multiplier demo")
        self._log("  cvar_inspect       - Show cvar details")
        self._log("")
        self._log("=== Key Binds ===")
        self._log("  F6 = cvar_help")
        self._log("  F7 = boost 100")
        self._log("  F8 = toggle cd_demo_mode")
        self._log("  Alias 'bdemo' runs boost + speed demo")
        self._log("")
        self._log("=== Registered Cvars ===")
        self._log(f"  cd_boost_amount = {self._get_cvar_str('cd_boost_amount')}")
        self._log(f"  cd_display_color = {self._get_cvar_str('cd_display_color')}")
        self._log(f"  cd_speed_multiplier = {self._get_cvar_str('cd_speed_multiplier')}")
        self._log(f"  cd_demo_mode = {self._get_cvar_str('cd_demo_mode')}")

    def _on_boost(self, args: list[str]) -> None:
        """Apply boost to the local car.

        Args:
            args: [command_name, amount]
        """
        if self.game_wrapper is None or self.boost_amount is None:
            return

        # Get boost amount from args or cvar default
        amount = self.boost_amount.getIntValue()
        if len(args) > 1:
            try:
                amount = max(0, min(100, int(args[1])))
                self.boost_amount.setValue(amount)
            except ValueError:
                self._log("Invalid boost amount. Use 0-100.")

        self._log(f"Setting boost to {amount}%")

        # Apply boost via the local car
        try:
            local_car = self.game_wrapper.GetLocalCar()
            if local_car is not None and not local_car.IsNull():
                local_car.ForceBoost(True)
                local_car.SetBoostCheap(True)
                self._log("Boost applied (cheap mode enabled).")
            else:
                self._log("No local car available (are you in a match?).")
        except Exception as e:
            self._log(f"Error applying boost: {e}")

    def _on_set_team_color(self, args: list[str]) -> None:
        """Set the local car's team color.

        Args:
            args: [command_name, team, r, g, b]
        """
        if self.game_wrapper is None:
            return

        if len(args) < 2:
            self._log("Usage: set_team_color <team 0/1> [r] [g] [b]")
            return

        try:
            team = int(args[1])
            if team not in (0, 1):
                self._log("Team must be 0 (blue) or 1 (orange).")
                return

            r = float(args[2]) / 255.0 if len(args) > 2 else 0.0
            g = float(args[3]) / 255.0 if len(args) > 3 else 1.0
            b = float(args[4]) / 255.0 if len(args) > 4 else 0.0

            local_car = self.game_wrapper.GetLocalCar()
            if local_car is not None and not local_car.IsNull():
                main_color = LinearColor(r, g, b, 1.0)
                secondary_color = LinearColor(0.1, 0.1, 0.1, 1.0)
                local_car.SetCarColor(main_color, secondary_color)
                self._log(
                    f"Set team {team} color to "
                    f"({r:.2f}, {g:.2f}, {b:.2f})"
                )
            else:
                self._log("No local car available.")
        except (ValueError, IndexError) as e:
            self._log(f"Error: {e}. Usage: set_team_color <team> [r] [g] [b]")

    def _on_speed_demo(self, args: list[str]) -> None:
        """Demonstrate speed multiplier.

        Args:
            args: Command arguments.
        """
        if self.game_wrapper is None or self.speed_mult is None:
            return

        mult = self.speed_mult.getFloatValue()
        self._log(f"Speed multiplier is currently {mult:.1f}x")

        try:
            server = self.game_wrapper.GetCurrentGameState()
            server.SetGameSpeed(mult)
            self._log(f"Game speed set to {mult:.1f}x")
        except Exception as e:
            self._log(f"Error setting game speed: {e}")

    def _on_inspect(self, args: list[str]) -> None:
        """Inspect all cvars registered by this plugin.

        Args:
            args: Command arguments.
        """
        self._log("=== Cvar Inspection ===")

        for cvar_name in [
            "cd_boost_amount",
            "cd_display_color",
            "cd_speed_multiplier",
            "cd_demo_mode",
        ]:
            cvar = self.cvar_manager.getCvar(cvar_name)
            if cvar.IsNull():
                self._log(f"  {cvar_name}: NOT FOUND")
                continue

            self._log(f"  --- {cvar.getCVarName()} ---")
            self._log(f"    Value:    {cvar.getStringValue()}")
            self._log(f"    Default:  {cvar.GetDefaultValue()}")
            self._log(f"    Desc:     {cvar.getDescription()}")
            self._log(f"    Int:      {cvar.getIntValue()}")
            self._log(f"    Float:    {cvar.getFloatValue()}")
            self._log(f"    Bool:     {cvar.getBoolValue()}")
            self._log(f"    Hidden:   {cvar.IsHidden()}")
            self._log(f"    SaveCfg:  {cvar.ShouldSaveToCfg()}")
            self._log(f"    HasMin:   {cvar.HasMinimum()}")
            if cvar.HasMinimum():
                self._log(f"    Min:      {cvar.GetMinimum()}")
            self._log(f"    HasMax:   {cvar.HasMaximum()}")
            if cvar.HasMaximum():
                self._log(f"    Max:      {cvar.GetMaximum()}")

    # ------------------------------------------------------------------
    # Utilities
    # ------------------------------------------------------------------

    def _get_cvar_str(self, name: str) -> str:
        """Get the string value of a cvar by name.

        Args:
            name: The cvar name.

        Returns:
            str: The cvar's string value, or 'ERROR' if not found.
        """
        if self.cvar_manager is None:
            return "N/A"

        cvar = self.cvar_manager.getCvar(name)
        if cvar.IsNull():
            return "NOT FOUND"
        return cvar.getStringValue()

    def _log(self, message: str) -> None:
        """Log a message to the BakkesMod console.

        Args:
            message: The message to log.
        """
        if self.cvar_manager:
            self.cvar_manager.log(message)

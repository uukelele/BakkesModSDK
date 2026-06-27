"""
Canvas Rendering
================
Demonstrates 2D rendering on the game screen using CanvasWrapper.

Use GameWrapper.RegisterDrawable() to register a function that gets
called every frame. The callback receives a CanvasWrapper that provides
all drawing primitives.

Drawing coordinate system:
  - Origin is top-left of the screen
  - X increases to the right, Y increases downward
  - Units are pixels (use Vector2F for float precision)
"""

from bakkesmod import (
    GameWrapper,
    CanvasWrapper,
    CVarManagerWrapper,
    Vector,
    Vector2F,
    Vector2,
    LinearColor,
    Rotator,
    NOTIFIER_PERMISSION,
)


class RenderPlugin:
    """Demonstrates 2D rendering on the game screen."""

    def __init__(self) -> None:
        self.game_wrapper: GameWrapper | None = None
        self.cvar_manager: CVarManagerWrapper | None = None
        self._registered: bool = False

    def on_load(
        self,
        game_wrapper: GameWrapper,
        cvar_manager: CVarManagerWrapper,
    ) -> None:
        """Register drawable and notifiers.

        Args:
            game_wrapper: The main SDK entry point.
            cvar_manager: Console variable manager.
        """
        self.game_wrapper = game_wrapper
        self.cvar_manager = cvar_manager

        # Register the drawable callback
        game_wrapper.RegisterDrawable(self._on_render)
        self._registered = True

        cvar_manager.registerNotifier(
            "render_toggle",
            self._on_toggle,
            "Toggle rendering on/off.",
            NOTIFIER_PERMISSION.ALL,
        )

        game_wrapper.LogToChatbox(
            "RenderPlugin: drawing HUD info. Use 'render_toggle' to toggle.",
            "RenderPlugin",
        )

    def on_unload(self) -> None:
        """Unregister drawables."""
        if self.game_wrapper and self._registered:
            self.game_wrapper.UnregisterDrawables()
            self._registered = False

    # ------------------------------------------------------------------
    # Render Callback (called every frame)
    # ------------------------------------------------------------------

    def _on_render(self, canvas: CanvasWrapper) -> None:
        """Draw various UI elements on the screen.

        Args:
            canvas: The drawing canvas for this frame.
        """
        if self.game_wrapper is None:
            return

        screen: Vector2 = self.game_wrapper.GetScreenSize()

        # --- Draw a filled background panel ---
        canvas.SetPosition(Vector2F(10.0, 10.0))
        canvas.SetColor(LinearColor(0.1, 0.1, 0.2, 0.7))
        canvas.FillBox(Vector2F(260.0, 100.0))

        # --- Draw text with drop shadow ---
        canvas.SetPosition(Vector2F(20.0, 20.0))
        canvas.SetColor(LinearColor(0.0, 1.0, 1.0, 1.0))
        canvas.DrawString("BakkesMod Python Demo", 1.0, 1.0, True)

        canvas.SetPosition(Vector2F(20.0, 40.0))
        canvas.SetColor(LinearColor(1.0, 1.0, 1.0, 1.0))

        # Show game state info
        in_game = self.game_wrapper.IsInGame()
        in_freeplay = self.game_wrapper.IsInFreeplay()
        in_online = self.game_wrapper.IsInOnlineGame()

        mode_str = "Menu"
        if in_freeplay:
            mode_str = "Freeplay"
        elif in_online:
            mode_str = "Online"
        elif self.game_wrapper.IsInReplay():
            mode_str = "Replay"
        elif self.game_wrapper.IsInCustomTraining():
            mode_str = "Custom Training"

        canvas.DrawString(f"Mode: {mode_str}", 1.0, 1.0, True)

        if in_game:
            try:
                server = self.game_wrapper.GetCurrentGameState()
                ball = server.GetBall()
                ball_pos = ball.GetLocation()
                ball_vel = ball.GetVelocity()

                local_car = self.game_wrapper.GetLocalCar()

                # Draw ball info
                canvas.SetPosition(Vector2F(20.0, 60.0))
                canvas.SetColor(LinearColor(0.3, 1.0, 0.3, 1.0))
                canvas.DrawString(
                    f"Ball: ({ball_pos.X:.0f}, {ball_pos.Y:.0f}, "
                    f"{ball_pos.Z:.0f})  Speed: {ball_vel.magnitude():.0f}",
                    1.0, 1.0, True,
                )

                # Draw local car info
                if local_car is not None and not local_car.IsNull():
                    car_pos = local_car.GetLocation()
                    car_vel = local_car.GetVelocity()
                    on_ground = local_car.GetbOnGround()
                    super_sonic = local_car.GetbSuperSonic()

                    canvas.SetPosition(Vector2F(20.0, 80.0))
                    canvas.SetColor(LinearColor(1.0, 0.8, 0.3, 1.0))
                    canvas.DrawString(
                        f"Car: ({car_pos.X:.0f}, {car_pos.Y:.0f}, "
                        f"{car_pos.Z:.0f})  Speed: {car_vel.magnitude():.0f}",
                        1.0, 1.0, True,
                    )

                    # Status indicators
                    status_parts = []
                    if on_ground:
                        status_parts.append("GROUND")
                    else:
                        status_parts.append("AIR")
                    if super_sonic:
                        status_parts.append("SUPERSONIC")
                    if local_car.HasFlip():
                        status_parts.append("HAS FLIP")
                    if local_car.IsDodging():
                        status_parts.append("DODGING")

                    canvas.SetPosition(Vector2F(20.0, 100.0))
                    canvas.SetColor(LinearColor(0.8, 0.8, 1.0, 1.0))
                    canvas.DrawString(
                        f"Status: {' | '.join(status_parts)}",
                        1.0, 1.0, True,
                    )

            except Exception:
                pass

        # --- Draw a semi-transparent FPS/style overlay (top-right) ---
        canvas.SetColor(LinearColor(1.0, 1.0, 1.0, 0.3))
        str_size = canvas.GetStringSize("BakkesMod Python", 1.0, 1.0)
        canvas.SetPosition(Vector2F(
            float(screen.X) - str_size.X - 20.0, 20.0
        ))
        canvas.DrawString("BakkesMod Python", 1.0, 1.0, True)

        # --- Draw crosshair-style lines at screen center ---
        cx = float(screen.X) // 2
        cy = float(screen.Y) // 2
        crosshair_size = 15.0

        canvas.SetColor(LinearColor(1.0, 1.0, 1.0, 0.4))
        canvas.DrawLine(
            Vector2F(cx - crosshair_size, cy),
            Vector2F(cx + crosshair_size, cy),
        )
        canvas.DrawLine(
            Vector2F(cx, cy - crosshair_size),
            Vector2F(cx, cy + crosshair_size),
        )

        # --- Project a 3D point to screen ---
        if in_game:
            try:
                server = self.game_wrapper.GetCurrentGameState()
                ball = server.GetBall()
                ball_pos = ball.GetLocation()

                # Project ball location to screen coordinates
                screen_pos: Vector2F = canvas.ProjectF(ball_pos)
                canvas.SetColor(LinearColor(1.0, 0.0, 0.0, 0.8))

                # Draw a circle-like indicator (using lines)
                for angle in range(0, 360, 30):
                    rad = angle * 3.14159 / 180.0
                    x1 = screen_pos.X + 8.0 * (rad % 1.0)  # simplified
                    _ = x1  # placeholder - real circle drawing needs iteration

                # Simple dot indicator
                canvas.SetPosition(Vector2F(screen_pos.X - 4, screen_pos.Y - 4))
                canvas.FillBox(Vector2F(8.0, 8.0))

            except Exception:
                pass

    def _on_toggle(self, args: list[str]) -> None:
        """Toggle rendering on/off via notifier.

        Args:
            args: Command arguments.
        """
        if self.game_wrapper is None:
            return

        if self._registered:
            self.game_wrapper.UnregisterDrawables()
            self._registered = False
            self.game_wrapper.LogToChatbox(
                "Rendering disabled.", "RenderPlugin"
            )
        else:
            self.game_wrapper.RegisterDrawable(self._on_render)
            self._registered = True
            self.game_wrapper.LogToChatbox(
                "Rendering enabled.", "RenderPlugin"
            )

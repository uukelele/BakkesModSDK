#include <pybind11/pybind11.h>

namespace py = pybind11;

// Forward declarations of init functions from all binding modules
void init_core_structs(py::module& m);
void init_enums(py::module& m);
void init_engine_wrappers(py::module& m);
void init_game_object_wrappers(py::module& m);
void init_game_event_wrappers(py::module& m);
void init_game_wrapper(py::module& m);
void init_wrappers(py::module& m);
void init_items_wrappers(py::module& m);
void init_misc_wrappers(py::module& m);

PYBIND11_MODULE(_bindings, m) {
    m.doc() = R"doc(
BakkesModSDK CPython bindings
==============================

Comprehensive Python bindings for the BakkesMod Rocket League plugin SDK.
Provides access to game state, rendering, console variables, events,
items, and all wrapper types exposed by BakkesMod.

Typical usage::

    from bakkesmod import GameWrapper, CVarManagerWrapper, ServerWrapper
    from bakkesmod import Vector, Rotator, LinearColor

    # GameWrapper is the main entry point
    game_wrapper: GameWrapper = ...

    # Access game state
    server = game_wrapper.GetCurrentGameState()
    ball = server.GetBall()
    local_car = game_wrapper.GetLocalCar()

    # Get the ball's position
    pos = ball.GetLocation()
    print(f"Ball at {pos.X}, {pos.Y}, {pos.Z}")

    # Predicted trajectory
    prediction = ball.PredictPosition(1.0)
    print(f"Ball will be at {prediction.Location} in 1 second")

    # Register drawable for 2D rendering
    game_wrapper.RegisterDrawable(lambda canvas: canvas.DrawString("Hello!"))

    # Register a console notifier
    cvar_manager.registerNotifier("my_command", callback, "Description", 0)

See the BakkesMod SDK documentation for full API details.
)doc";

    // Initialize all binding modules in order
    init_core_structs(m);
    init_enums(m);
    init_engine_wrappers(m);
    init_game_object_wrappers(m);
    init_game_event_wrappers(m);
    init_game_wrapper(m);
    init_wrappers(m);
    init_items_wrappers(m);
    init_misc_wrappers(m);
}

"""
HTTP Requests
=============
Demonstrates how to make HTTP requests using HttpWrapper.

BakkesMod supports:
  - GET/POST requests with string responses
  - Binary data responses
  - File downloads
  - JSON requests with auto-set headers

IMPORTANT: HTTP callbacks run on background threads. You MUST wrap any
SDK calls inside GameWrapper.Execute() to run them on the game thread.
"""

from bakkesmod import (
    GameWrapper,
    CVarManagerWrapper,
    HttpWrapper,
    CurlRequest,
    NOTIFIER_PERMISSION,
)


class HttpDemo:
    """Demonstrates HTTP request functionality."""

    def __init__(self) -> None:
        self.game_wrapper: GameWrapper | None = None
        self.cvar_manager: CVarManagerWrapper | None = None

    def on_load(
        self,
        game_wrapper: GameWrapper,
        cvar_manager: CVarManagerWrapper,
    ) -> None:
        """Register HTTP demo notifiers.

        Args:
            game_wrapper: The main SDK entry point.
            cvar_manager: Console variable manager.
        """
        self.game_wrapper = game_wrapper
        self.cvar_manager = cvar_manager

        cvar_manager.registerNotifier(
            "http_get",
            self._on_http_get,
            "Perform a GET request. Usage: http_get <url>",
            NOTIFIER_PERMISSION.ALL,
        )
        cvar_manager.registerNotifier(
            "http_json",
            self._on_http_json,
            "Perform a JSON GET request. Usage: http_json <url>",
            NOTIFIER_PERMISSION.ALL,
        )
        cvar_manager.registerNotifier(
            "http_version",
            self._on_http_version,
            "Check BakkesMod plugin API version via HTTP.",
            NOTIFIER_PERMISSION.ALL,
        )

        game_wrapper.LogToChatbox(
            "HttpDemo loaded. Try: http_get https://httpbin.org/get",
            "HttpDemo",
        )

    def _on_http_get(self, args: list[str]) -> None:
        """Perform a simple GET request.

        Args:
            args: [command_name, url]
        """
        if len(args) < 2:
            self._log("Usage: http_get <url>")
            return

        url = args[1]
        self._log(f"GET: {url}")

        # Build the request
        request = CurlRequest()
        request.url = url
        request.method = "GET"
        request.timeout = 10.0
        request.maxRedirects = 5

        # Send with string callback
        # NOTE: The callback runs on a BACKGROUND THREAD!
        # Use game_wrapper.Execute() to run game-thread code.
        gw = self.game_wrapper

        def on_complete(http_code: int, response: str) -> None:
            """Handle HTTP response on background thread.

            Args:
                http_code: HTTP status code.
                response: Response body string.
            """
            # Capture gw in closure to access game wrapper on game thread
            gw.Execute(lambda _: self._on_response(http_code, response))

        HttpWrapper.SendCurlRequest(request, on_complete)

    def _on_http_json(self, args: list[str]) -> None:
        """Perform a JSON GET request with auto-set Accept/Content-Type headers.

        Args:
            args: [command_name, url]
        """
        if len(args) < 2:
            self._log("Usage: http_json <url>")
            return

        url = args[1]
        self._log(f"JSON GET: {url}")

        request = CurlRequest()
        request.url = url
        request.method = "GET"
        request.timeout = 10.0

        gw = self.game_wrapper

        def on_complete(http_code: int, response: str) -> None:
            gw.Execute(lambda _: self._on_response(http_code, response))

        HttpWrapper.SendCurlJsonRequest(request, on_complete)

    def _on_http_version(self, args: list[str]) -> None:
        """Check the BakkesMod version via the version API.

        Args:
            args: Command arguments.
        """
        self._log("Fetching BakkesMod version from API...")

        request = CurlRequest()
        request.url = "https://api.bakkesmod.com/version"
        request.method = "GET"
        request.timeout = 5.0

        gw = self.game_wrapper

        def on_complete(http_code: int, response: str) -> None:
            def callback(gw_self) -> None:
                if http_code == 200:
                    self._log(f"API Response ({http_code}): {response[:200]}")
                else:
                    self._log(f"HTTP Error: {http_code}")
                # Also show local version
                version = gw.GetBakkesModVersion()
                self._log(f"Local BakkesMod version: {version}")

            gw.Execute(callback)

        HttpWrapper.SendCurlRequest(request, on_complete)

    def _on_response(self, http_code: int, response: str) -> None:
        """Handle HTTP response (called on game thread via Execute).

        Args:
            http_code: HTTP status code.
            response: The response body.
        """
        if http_code == 418:
            self._log(f"Request failed (HTTP 418 - I'm a teapot / error)")
            return

        self._log(f"HTTP {http_code}: received {len(response)} bytes")
        self._log(f"Response (first 300 chars): {response[:300]}")

        # Show a toast notification
        if self.game_wrapper:
            self.game_wrapper.Toast(
                "HTTP Response",
                f"Status: {http_code}, {len(response)} bytes",
                "default",
                3.0,
                0,  # Info
            )

    def _log(self, message: str) -> None:
        """Log a message.

        Args:
            message: The message to log.
        """
        if self.cvar_manager:
            self.cvar_manager.log(f"[HttpDemo] {message}")


# ------------------------------------------------------------------
# Example: POST JSON data
# ------------------------------------------------------------------

def send_post_example(game_wrapper: GameWrapper) -> None:
    """Example of sending a JSON POST request.

    Args:
        game_wrapper: The main SDK entry point.
    """
    request = CurlRequest()
    request.url = "https://httpbin.org/post"
    request.method = "POST"
    request.body = '{"key": "value", "plugin": "python-bindings"}'
    request.headers = ["Content-Type: application/json"]
    request.timeout = 10.0

    def on_complete(http_code: int, response: str) -> None:
        game_wrapper.Execute(lambda _: print(
            f"POST response ({http_code}): {response[:200]}"
        ))

    HttpWrapper.SendCurlJsonRequest(request, on_complete)

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
        cvar_manager.registerNotifier(
            "http_post",
            self._on_http_post,
            "Perform a JSON POST request. Usage: http_post <url> <json_body>",
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

        request = CurlRequest()
        request.url = url
        request.verb = "GET"

        gw = self.game_wrapper

        def on_complete(http_code: int, response: str) -> None:
            gw.Execute(lambda _: self._on_response(http_code, response))

        HttpWrapper.SendCurlRequest(request, on_complete)

    def _on_http_json(self, args: list[str]) -> None:
        """Perform a JSON GET request.

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
        request.verb = "GET"

        gw = self.game_wrapper

        def on_complete(http_code: int, response: str) -> None:
            gw.Execute(lambda _: self._on_response(http_code, response))

        HttpWrapper.SendCurlJsonRequest(request, on_complete)

    def _on_http_post(self, args: list[str]) -> None:
        """Perform a JSON POST request.

        Args:
            args: [command_name, url, json_body]
        """
        if len(args) < 2:
            self._log("Usage: http_post <url> [json_body]")
            return

        url = args[1]
        body = args[2] if len(args) > 2 else '{"key": "value"}'

        self._log(f"POST: {url}")

        request = CurlRequest()
        request.url = url
        request.verb = "POST"
        request.body = body
        request.headers = {"Content-Type": "application/json"}

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
        request.verb = "GET"

        gw = self.game_wrapper

        def on_complete(http_code: int, response: str) -> None:
            def callback(gw_self) -> None:
                if http_code == 200:
                    self._log(f"API Response ({http_code}): {response[:200]}")
                else:
                    self._log(f"HTTP Error: {http_code}")
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
        self._log(f"HTTP {http_code}: received {len(response)} bytes")
        self._log(f"Response (first 300 chars): {response[:300]}")

        if self.game_wrapper:
            self.game_wrapper.Toast(
                "HTTP Response",
                f"Status: {http_code}, {len(response)} bytes",
                "default",
                3.0,
                0,
            )

    def _log(self, message: str) -> None:
        """Log a message.

        Args:
            message: The message to log.
        """
        if self.cvar_manager:
            self.cvar_manager.log(f"[HttpDemo] {message}")

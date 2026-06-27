"""
MMR and Rank Lookup
===================
Demonstrates how to read skill ratings, MMR, and competitive ranks.

Key APIs:
  - MMRWrapper:      Get MMR, skill rating, and rank data
  - UniqueIDWrapper: Cross-platform player identification
  - SkillRating:     Mu/Sigma TrueSkill values
  - SkillRank:       Tier, Division, MatchesPlayed

The MMR system uses TrueSkill where:
  - Mu (mean):    The player's estimated skill
  - Sigma (std):  Uncertainty in the estimate (lower = more confident)
  - MMR = Mu - 3*Sigma (standard formula)
  - Tier/Division maps to the visual rank shown in-game

Playlist IDs (common):
  0  = Unranked Duel
  1  = Unranked Doubles
  2  = Unranked Standard
  3  = Competitive Duel
  4  = Competitive Doubles (2v2)
  6  = Competitive Standard (3v3)
  8  = Hoops
  9  = Rumble
  10 = Dropshot
  11 = Snow Day
  12 = Tournament
  13 = Casual
  22 = Heatseeker
"""

from bakkesmod import (
    GameWrapper,
    MMRWrapper,
    UniqueIDWrapper,
    SkillRating,
    SkillRank,
    CVarManagerWrapper,
    NOTIFIER_PERMISSION,
    OnlinePlatform,
)


# Rank tier names (map tier number to display name)
RANK_TIERS: dict[int, str] = {
    0:  "Unranked",
    1:  "Bronze I",
    2:  "Bronze II",
    3:  "Bronze III",
    4:  "Silver I",
    5:  "Silver II",
    6:  "Silver III",
    7:  "Gold I",
    8:  "Gold II",
    9:  "Gold III",
    10: "Platinum I",
    11: "Platinum II",
    12: "Platinum III",
    13: "Diamond I",
    14: "Diamond II",
    15: "Diamond III",
    16: "Champion I",
    17: "Champion II",
    18: "Champion III",
    19: "Grand Champion I",
    20: "Grand Champion II",
    21: "Grand Champion III",
    22: "Supersonic Legend",
}

PLAYLIST_NAMES: dict[int, str] = {
    0:  "Duel",
    1:  "Doubles",
    2:  "Standard",
    3:  "Competitive Duel",
    4:  "Competitive Doubles",
    6:  "Competitive Standard",
    8:  "Hoops",
    9:  "Rumble",
    10: "Dropshot",
    11: "Snow Day",
    12: "Tournament",
    13: "Casual",
    22: "Heatseeker",
}


class MMRDemo:
    """Demonstrates MMR and rank lookup."""

    def __init__(self) -> None:
        self.game_wrapper: GameWrapper | None = None
        self.cvar_manager: CVarManagerWrapper | None = None

    def on_load(
        self,
        game_wrapper: GameWrapper,
        cvar_manager: CVarManagerWrapper,
    ) -> None:
        """Register MMR demo notifiers.

        Args:
            game_wrapper: The main SDK entry point.
            cvar_manager: Console variable manager.
        """
        self.game_wrapper = game_wrapper
        self.cvar_manager = cvar_manager

        cvar_manager.registerNotifier(
            "mmr_me",
            self._on_mmr_me,
            "Show your current MMR and rank for all playlists.",
            NOTIFIER_PERMISSION.ALL,
        )
        cvar_manager.registerNotifier(
            "mmr_player",
            self._on_mmr_player,
            "Show MMR for a player. Usage: mmr_player <epic_id> [playlist]",
            NOTIFIER_PERMISSION.ALL,
        )
        cvar_manager.registerNotifier(
            "mmr_current",
            self._on_mmr_current,
            "Show MMR for the current playlist.",
            NOTIFIER_PERMISSION.ONLINE,
        )

        game_wrapper.LogToChatbox(
            "MMRDemo loaded. Try: mmr_me, mmr_current",
            "MMRDemo",
        )

    def _on_mmr_me(self, args: list[str]) -> None:
        """Display MMR and rank for the local player.

        Args:
            args: Command arguments.
        """
        if self.game_wrapper is None:
            return

        try:
            mmr: MMRWrapper = self.game_wrapper.GetMMRWrapper()
            uid: UniqueIDWrapper = self.game_wrapper.GetUniqueID()
            platform = uid.GetPlatform()
            epic_id = uid.GetEpicAccountID()

            self._log(f"=== MMR for {self.game_wrapper.GetPlayerName().ToString()} ===")
            self._log(f"Platform: {platform}")
            self._log(f"Epic ID: {epic_id}")
            self._log(f"UID: {uid.GetUID()}")
            self._log("")

            # Check sync status
            self._log(f"Syncing: {mmr.IsSyncing(uid)}")

            # Query interesting playlists
            playlists_to_check = [0, 1, 2, 3, 4, 6, 8, 9, 10, 11, 13, 22]

            for playlist_id in playlists_to_check:
                synced = mmr.IsSynced(uid, playlist_id)
                ranked = mmr.IsRanked(playlist_id)
                playlist_name = PLAYLIST_NAMES.get(playlist_id, f"Playlist {playlist_id}")

                if not synced:
                    self._log(f"  {playlist_name}: Not synced")
                    continue

                rank: SkillRank = mmr.GetPlayerRank(uid, playlist_id)
                skill: SkillRating = mmr.GetPlayerSkillRating(uid, playlist_id)
                mmr_val: float = mmr.GetPlayerMMR(uid, playlist_id)

                tier_name = RANK_TIERS.get(rank.Tier, f"Tier {rank.Tier}")
                self._log(
                    f"  {playlist_name}: {tier_name} (Div {rank.Division})  "
                    f"MMR={mmr_val:.0f}  "
                    f"Mu={skill.Mu:.1f} Sigma={skill.Sigma:.1f}  "
                    f"Matches={rank.MatchesPlayed}"
                )

            # Current playlist
            current = mmr.GetCurrentPlaylist()
            self._log(f"\nCurrent playlist ID: {current}")

        except Exception as e:
            self._log(f"Error getting MMR: {e}")

    def _on_mmr_player(self, args: list[str]) -> None:
        """Display MMR for a specific player by Epic ID.

        Args:
            args: [command_name, epic_id, playlist_id]
        """
        if self.game_wrapper is None:
            return

        if len(args) < 2:
            self._log("Usage: mmr_player <epic_id> [playlist_id]")
            return

        epic_id = args[1]
        playlist_id = int(args[2]) if len(args) > 2 else 4  # default: 2v2

        try:
            mmr: MMRWrapper = self.game_wrapper.GetMMRWrapper()

            # Create a UniqueIDWrapper from the Epic ID
            # Note: UID and splitscreen default to 0
            uid = UniqueIDWrapper.FromEpicAccountID(
                epic_id,
                0,               # uid (unknown)
                OnlinePlatform.Epic,
                0,               # splitscreen ID
            )

            playlist_name = PLAYLIST_NAMES.get(playlist_id, f"Playlist {playlist_id}")

            if not mmr.IsSynced(uid, playlist_id):
                self._log(f"{epic_id}: Not synced for {playlist_name}")
                return

            rank: SkillRank = mmr.GetPlayerRank(uid, playlist_id)
            mmr_val: float = mmr.GetPlayerMMR(uid, playlist_id)
            tier_name = RANK_TIERS.get(rank.Tier, f"Tier {rank.Tier}")

            self._log(
                f"{epic_id} in {playlist_name}: "
                f"{tier_name} Div {rank.Division} | "
                f"MMR={mmr_val:.0f} | "
                f"Matches={rank.MatchesPlayed}"
            )

        except Exception as e:
            self._log(f"Error: {e}")

    def _on_mmr_current(self, args: list[str]) -> None:
        """Display MMR for the current playlist.

        Args:
            args: Command arguments.
        """
        if self.game_wrapper is None:
            return

        try:
            mmr: MMRWrapper = self.game_wrapper.GetMMRWrapper()
            uid: UniqueIDWrapper = self.game_wrapper.GetUniqueID()

            current_playlist = mmr.GetCurrentPlaylist()
            playlist_name = PLAYLIST_NAMES.get(
                current_playlist, f"Playlist {current_playlist}"
            )

            if current_playlist < 0:
                self._log("Not in a ranked playlist.")
                return

            if not mmr.IsSynced(uid, current_playlist):
                self._log(f"{playlist_name}: Not synced yet")
                return

            rank: SkillRank = mmr.GetPlayerRank(uid, current_playlist)
            skill: SkillRating = mmr.GetPlayerSkillRating(uid, current_playlist)
            mmr_val: float = mmr.GetPlayerMMR(uid, current_playlist)
            tier_name = RANK_TIERS.get(rank.Tier, f"Tier {rank.Tier}")

            self._log(f"=== Current Playlist: {playlist_name} ===")
            self._log(f"Rank: {tier_name} (Division {rank.Division})")
            self._log(f"MMR: {mmr_val:.0f}")
            self._log(f"TrueSkill: Mu={skill.Mu:.1f}, Sigma={skill.Sigma:.3f}")
            self._log(f"Matches Played: {rank.MatchesPlayed}")

        except Exception as e:
            self._log(f"Error: {e}")

    def _log(self, message: str) -> None:
        """Log a message.

        Args:
            message: The message to log.
        """
        if self.cvar_manager:
            self.cvar_manager.log(f"[MMRDemo] {message}")

"""
Items and Loadouts
==================
Demonstrates how to query the item/inventory system, read loadouts,
and inspect products and their attributes.

Key APIs:
  - ItemsWrapper:    Main entry for all item/inventory operations
  - ProductWrapper:  A game product definition
  - OnlineProductWrapper: An owned product instance
  - ProductSlotWrapper: Equipment slot types
  - LoadoutWrapper:  A player's equipped loadout
  - LoadoutSaveWrapper: Saved loadout data
"""

from bakkesmod import (
    GameWrapper,
    ItemsWrapper,
    ProductWrapper,
    ProductSlotWrapper,
    OnlineProductWrapper,
    ProductInstanceID,
    LoadoutWrapper,
    CVarManagerWrapper,
    NOTIFIER_PERMISSION,
    UnrealStringWrapper,
    Equipslot,
)


# Human-readable names for equipment slots
SLOT_NAMES: dict[Equipslot, str] = {
    Equipslot.BODY: "Car Body",
    Equipslot.DECAL: "Decal",
    Equipslot.WHEELS: "Wheels",
    Equipslot.ROCKETBOOST: "Rocket Boost",
    Equipslot.ANTENNA: "Antenna",
    Equipslot.TOPPER: "Topper",
    Equipslot.BUMPER: "Bumper",
    Equipslot.PAINTFINISH: "Paint Finish",
    Equipslot.GOALEXPLOSION: "Goal Explosion",
    Equipslot.TRAIL: "Trail",
    Equipslot.ENGINEAUDIO: "Engine Audio",
    Equipslot.PLAYERBANNER: "Player Banner",
    Equipslot.AVATARBORDER: "Avatar Border",
    Equipslot.PLAYERTITLE: "Player Title",
}


class ItemsDemo:
    """Demonstrates item system usage."""

    def __init__(self) -> None:
        self.game_wrapper: GameWrapper | None = None
        self.cvar_manager: CVarManagerWrapper | None = None

    def on_load(
        self,
        game_wrapper: GameWrapper,
        cvar_manager: CVarManagerWrapper,
    ) -> None:
        """Register items demo notifiers.

        Args:
            game_wrapper: The main SDK entry point.
            cvar_manager: Console variable manager.
        """
        self.game_wrapper = game_wrapper
        self.cvar_manager = cvar_manager

        cvar_manager.registerNotifier(
            "items_loadout",
            self._on_show_loadout,
            "Show your current loadout for each team.",
            NOTIFIER_PERMISSION.ALL,
        )
        cvar_manager.registerNotifier(
            "items_slots",
            self._on_show_slots,
            "Show all equipment slots and their products.",
            NOTIFIER_PERMISSION.ALL,
        )
        cvar_manager.registerNotifier(
            "items_product",
            self._on_show_product,
            "Show product info. Usage: items_product <product_id>",
            NOTIFIER_PERMISSION.ALL,
        )
        cvar_manager.registerNotifier(
            "items_owned",
            self._on_show_owned,
            "Count owned products.",
            NOTIFIER_PERMISSION.ALL,
        )

        game_wrapper.LogToChatbox(
            "ItemsDemo loaded. Try: items_loadout, items_slots",
            "ItemsDemo",
        )

    def _on_show_loadout(self, args: list[str]) -> None:
        """Display the current loadout for both teams.

        Args:
            args: Command arguments.
        """
        if self.game_wrapper is None:
            return

        try:
            items: ItemsWrapper = self.game_wrapper.GetItemsWrapper()
            if items.IsNull():
                self._log("ItemsWrapper is null (not in a match?).")
                return

            loadout_name: UnrealStringWrapper = items.GetCurrentLoadoutName()
            self._log(f"Current Loadout: {loadout_name.ToString()}")

            # Show loadout for team 0 (blue) and team 1 (orange)
            for team_index in range(2):
                try:
                    loadout: LoadoutWrapper = items.GetCurrentLoadout(team_index)
                    self._log(f"  --- Team {team_index} Loadout ---")

                    # Iterate known equipment slots
                    for slot_enum, slot_name in SLOT_NAMES.items():
                        slot_id = int(slot_enum)
                        # Note: LoadoutWrapper provides per-slot access
                        # via the ItemsWrapper product lookup
                        self._log(f"    {slot_name}: slot {slot_id}")

                except Exception as e:
                    self._log(f"  Team {team_index}: {e}")

        except Exception as e:
            self._log(f"Error showing loadout: {e}")

    def _on_show_slots(self, args: list[str]) -> None:
        """Display all product slots and some example products.

        Args:
            args: Command arguments.
        """
        if self.game_wrapper is None:
            return

        try:
            items: ItemsWrapper = self.game_wrapper.GetItemsWrapper()
            if items.IsNull():
                self._log("ItemsWrapper is null.")
                return

            slots = items.GetAllProductSlots()
            self._log(f"Total product slots: {slots.Count()}")

            for i in range(slots.Count()):
                slot: ProductSlotWrapper = slots.Get(i)
                if slot.IsNull():
                    continue
                self._log(f"  Slot {i}: exists")

            # Count all products
            all_products = items.GetAllProducts()
            self._log(f"Total products in database: {all_products.Count()}")

        except Exception as e:
            self._log(f"Error: {e}")

    def _on_show_product(self, args: list[str]) -> None:
        """Display information about a specific product.

        Args:
            args: [command_name, product_id]
        """
        if self.game_wrapper is None:
            return

        if len(args) < 2:
            self._log("Usage: items_product <product_id>")
            return

        try:
            product_id = int(args[1])
            items: ItemsWrapper = self.game_wrapper.GetItemsWrapper()
            if items.IsNull():
                self._log("ItemsWrapper is null.")
                return

            product: ProductWrapper = items.GetProduct(product_id)
            if product.IsNull():
                self._log(f"Product {product_id}: Not found")
                return

            self._log(f"Product {product_id}: exists")

        except ValueError:
            self._log("Product ID must be an integer.")
        except Exception as e:
            self._log(f"Error: {e}")

    def _on_show_owned(self, args: list[str]) -> None:
        """Count owned (unlocked) products.

        Args:
            args: Command arguments.
        """
        if self.game_wrapper is None:
            return

        try:
            items: ItemsWrapper = self.game_wrapper.GetItemsWrapper()
            if items.IsNull():
                self._log("ItemsWrapper is null.")
                return

            owned = items.GetOwnedProducts()
            cached = items.GetCachedUnlockedProducts()

            self._log(f"Owned products (live): {owned.Count()}")
            self._log(f"Cached unlocked products: {cached.Count()}")

        except Exception as e:
            self._log(f"Error: {e}")

    def _log(self, message: str) -> None:
        """Log a message.

        Args:
            message: The message to log.
        """
        if self.cvar_manager:
            self.cvar_manager.log(f"[ItemsDemo] {message}")

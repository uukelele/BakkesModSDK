#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <bakkesmod/wrappers/items/ItemsWrapper.h>
#include <bakkesmod/wrappers/items/ProductWrapper.h>
#include <bakkesmod/wrappers/items/ProductSlotWrapper.h>
#include <bakkesmod/wrappers/items/OnlineProductWrapper.h>
#include <bakkesmod/wrappers/items/ProductPaintWrapper.h>
#include <bakkesmod/wrappers/items/ProductTemplateWrapper.h>
#include <bakkesmod/wrappers/items/ProductTradeInWrapper.h>
#include <bakkesmod/wrappers/items/TradeWrapper.h>
#include <bakkesmod/wrappers/items/LoadoutWrapper.h>
#include <bakkesmod/wrappers/items/LoadoutSaveWrapper.h>
#include <bakkesmod/wrappers/items/LoadoutSetWrapper.h>
#include <bakkesmod/wrappers/items/LoadingProductWrapper.h>
#include <bakkesmod/wrappers/items/GfxProductsWrapper.h>
#include <bakkesmod/wrappers/items/attributes/ProductAttributeWrapper.h>
#include <bakkesmod/wrappers/items/attributes/ProductAttribute_AnimatedSkinLabelWrapper.h>
#include <bakkesmod/wrappers/items/attributes/ProductAttribute_BlueprintCostWrapper.h>
#include <bakkesmod/wrappers/items/attributes/ProductAttribute_BlueprintWrapper.h>
#include <bakkesmod/wrappers/items/attributes/ProductAttribute_BodyCompatibilityWrapper.h>
#include <bakkesmod/wrappers/items/attributes/ProductAttribute_CertifiedWrapper.h>
#include <bakkesmod/wrappers/items/attributes/ProductAttribute_CurrencyWrapper.h>
#include <bakkesmod/wrappers/items/attributes/ProductAttribute_PaintedWrapper.h>
#include <bakkesmod/wrappers/items/attributes/ProductAttribute_QualityWrapper.h>
#include <bakkesmod/wrappers/items/attributes/ProductAttribute_SpecialEditionWrapper.h>
#include <bakkesmod/wrappers/items/attributes/ProductAttribute_SpecialEditionSettingsWrapper.h>
#include <bakkesmod/wrappers/items/attributes/ProductAttribute_TeamEditionUploadWrapper.h>
#include <bakkesmod/wrappers/items/attributes/ProductAttribute_TeamEditionWrapper.h>
#include <bakkesmod/wrappers/items/attributes/ProductAttribute_UnlockMethodWrapper.h>
#include <bakkesmod/wrappers/items/dbs/CertifiedStatDatabaseWrapper.h>
#include <bakkesmod/wrappers/items/dbs/DataAssetDatabaseWrapper.h>
#include <bakkesmod/wrappers/items/dbs/DataAssetDatabase_ESportsTeamWrapper.h>
#include <bakkesmod/wrappers/items/dbs/PaintDatabaseWrapper.h>
#include <bakkesmod/wrappers/items/dbs/SpecialEditionDatabaseWrapper.h>
#include <bakkesmod/wrappers/arraywrapper.h>
#include <bakkesmod/core/loadout_structs.h>

namespace py = pybind11;

void init_items_wrappers(py::module& m) {
    // ============================================================
    // ItemsWrapper
    // ============================================================
    py::class_<ItemsWrapper, ObjectWrapper>(m, "ItemsWrapper",
        R"doc(Wrapper for the game's item/inventory system.

        Provides access to products, online items, loadouts, paints,
        certifications, and trading functionality.
        )doc")
        .def(py::init<std::uintptr_t, std::uintptr_t>(), py::arg("game_data"), py::arg("save_data"))
        .def("IsNull", &ItemsWrapper::IsNull)
        .def("__bool__", &ItemsWrapper::operator bool)
        .def("GetAllProducts", &ItemsWrapper::GetAllProducts,
            "Get all available products.")
        .def("GetAllProductSlots", &ItemsWrapper::GetAllProductSlots,
            "Get all product equipment slots.")
        .def("GetProduct", &ItemsWrapper::GetProduct, py::arg("productId"),
            "Get a product by its ID.")
        .def("GetOnlineProduct", py::overload_cast<const ProductInstanceID&>(&ItemsWrapper::GetOnlineProduct, py::const_),
            py::arg("product_instance_id"),
            "Get an online product by its instance ID.")
        .def_static("IsOnlineID", &ItemsWrapper::IsOnlineID, py::arg("product_instance_id"),
            "Check if a product instance ID is for an online item.")
        .def_static("GetProductIdFromInstanceId", &ItemsWrapper::GetProductIdFromInstanceId,
            py::arg("product_instance_id"),
            "Extract the product ID from a product instance ID.")
        .def("GetCachedUnlockedProducts", &ItemsWrapper::GetCachedUnlockedProducts,
            "Get cached unlocked products.")
        .def("GetOwnedProducts", &ItemsWrapper::GetOwnedProducts,
            "Get the player's owned products.")
        .def("GetCertifiedStatDB", &ItemsWrapper::GetCertifiedStatDB)
        .def("GetEsportTeamDB", &ItemsWrapper::GetEsportTeamDB)
        .def("GetPaintDB", &ItemsWrapper::GetPaintDB)
        .def("GetSpecialEditionDB", &ItemsWrapper::GetSpecialEditionDB)
        .def("GetCurrentLoadoutName", &ItemsWrapper::GetCurrentLoadoutName)
        .def("GetCurrentLoadout", &ItemsWrapper::GetCurrentLoadout, py::arg("teamIndex"))
        .def("GetTradeWrapper", &ItemsWrapper::GetTradeWrapper)
        .def("GetProductTradeInWrapper", &ItemsWrapper::GetProductTradeInWrapper)
        .def("GetGfxProductsWrapper", &ItemsWrapper::GetGfxProductsWrapper);

    // ============================================================
    // Item Product Wrappers
    // ============================================================
    py::class_<ProductWrapper, ObjectWrapper>(m, "ProductWrapper",
        R"doc(Wrapper for a game product/item definition.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<ProductSlotWrapper, ObjectWrapper>(m, "ProductSlotWrapper",
        R"doc(Wrapper for a product equipment slot.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<OnlineProductWrapper, ObjectWrapper>(m, "OnlineProductWrapper",
        R"doc(Wrapper for an online (owned) product instance.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<ProductPaintWrapper, ObjectWrapper>(m, "ProductPaintWrapper",
        R"doc(Wrapper for a product paint variant.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<ProductTemplateWrapper, ObjectWrapper>(m, "ProductTemplateWrapper",
        R"doc(Wrapper for a product template.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<ProductTradeInWrapper, ObjectWrapper>(m, "ProductTradeInWrapper",
        R"doc(Wrapper for product trade-in functionality.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<TradeWrapper, ObjectWrapper>(m, "TradeWrapper",
        R"doc(Wrapper for trading functionality.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<LoadoutWrapper, ObjectWrapper>(m, "LoadoutWrapper",
        R"doc(Wrapper for a player's loadout.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<LoadoutSaveWrapper, ObjectWrapper>(m, "LoadoutSaveWrapper",
        R"doc(Wrapper for loadout save data.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<LoadoutSetWrapper, ObjectWrapper>(m, "LoadoutSetWrapper",
        R"doc(Wrapper for a loadout set.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<LoadingProductWrapper, ObjectWrapper>(m, "LoadingProductWrapper",
        R"doc(Wrapper for a loading product.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<GfxProductsWrapper, ObjectWrapper>(m, "GfxProductsWrapper",
        R"doc(Wrapper for GFX product display data.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    // ============================================================
    // Product Attribute Wrappers
    // ============================================================
    py::class_<ProductAttributeWrapper, ObjectWrapper>(m, "ProductAttributeWrapper",
        R"doc(Base wrapper for product attributes.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<ProductAttribute_AnimatedSkinLabelWrapper, ProductAttributeWrapper>(
        m, "ProductAttribute_AnimatedSkinLabelWrapper",
        R"doc(Wrapper for animated skin label attributes.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<ProductAttribute_BlueprintCostWrapper, ProductAttributeWrapper>(
        m, "ProductAttribute_BlueprintCostWrapper",
        R"doc(Wrapper for blueprint cost attributes.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<ProductAttribute_BlueprintWrapper, ProductAttributeWrapper>(
        m, "ProductAttribute_BlueprintWrapper",
        R"doc(Wrapper for blueprint attributes.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<ProductAttribute_BodyCompatibilityWrapper, ProductAttributeWrapper>(
        m, "ProductAttribute_BodyCompatibilityWrapper",
        R"doc(Wrapper for body compatibility attributes.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<ProductAttribute_CertifiedWrapper, ProductAttributeWrapper>(
        m, "ProductAttribute_CertifiedWrapper",
        R"doc(Wrapper for certified item attributes.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<ProductAttribute_CurrencyWrapper, ProductAttributeWrapper>(
        m, "ProductAttribute_CurrencyWrapper",
        R"doc(Wrapper for currency attributes.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<ProductAttribute_PaintedWrapper, ProductAttributeWrapper>(
        m, "ProductAttribute_PaintedWrapper",
        R"doc(Wrapper for painted item attributes.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<ProductAttribute_QualityWrapper, ProductAttributeWrapper>(
        m, "ProductAttribute_QualityWrapper",
        R"doc(Wrapper for quality attributes.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<ProductAttribute_SpecialEditionWrapper, ProductAttributeWrapper>(
        m, "ProductAttribute_SpecialEditionWrapper",
        R"doc(Wrapper for special edition attributes.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<ProductAttribute_SpecialEditionSettingsWrapper, ProductAttributeWrapper>(
        m, "ProductAttribute_SpecialEditionSettingsWrapper",
        R"doc(Wrapper for special edition settings attributes.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<ProductAttribute_TeamEditionUploadWrapper, ProductAttributeWrapper>(
        m, "ProductAttribute_TeamEditionUploadWrapper",
        R"doc(Wrapper for team edition upload attributes.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<ProductAttribute_TeamEditionWrapper, ProductAttributeWrapper>(
        m, "ProductAttribute_TeamEditionWrapper",
        R"doc(Wrapper for team edition attributes.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<ProductAttribute_UnlockMethodWrapper, ProductAttributeWrapper>(
        m, "ProductAttribute_UnlockMethodWrapper",
        R"doc(Wrapper for unlock method attributes.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    // ============================================================
    // Database Wrappers
    // ============================================================
    py::class_<CertifiedStatDatabaseWrapper, ObjectWrapper>(m, "CertifiedStatDatabaseWrapper",
        R"doc(Wrapper for the certified stat database.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<DataAssetDatabaseWrapper, ObjectWrapper>(m, "DataAssetDatabaseWrapper",
        R"doc(Wrapper for the data asset database.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<DataAssetDatabase_ESportsTeamWrapper, DataAssetDatabaseWrapper>(
            m, "DataAssetDatabase_ESportsTeamWrapper",
        R"doc(Wrapper for the esports team database.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<PaintDatabaseWrapper, ObjectWrapper>(m, "PaintDatabaseWrapper",
        R"doc(Wrapper for the paint database.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    py::class_<SpecialEditionDatabaseWrapper, ObjectWrapper>(m, "SpecialEditionDatabaseWrapper",
        R"doc(Wrapper for the special edition database.)doc")
        .def(py::init<std::uintptr_t>(), py::arg("memory_address"));

    // ============================================================
    // Loadout structs from pluginsdk
    // ============================================================
    py::class_<pluginsdk::TeamPaint>(m, "TeamPaint",
        R"doc(Team paint configuration for car colors.)doc")
        .def(py::init<>())
        .def_readwrite("team", &pluginsdk::TeamPaint::team)
        .def_readwrite("team_color_id", &pluginsdk::TeamPaint::team_color_id)
        .def_readwrite("custom_color_id", &pluginsdk::TeamPaint::custom_color_id);

    py::class_<pluginsdk::CarColors>(m, "CarColors",
        R"doc(Car color configuration including team paint and optional color overrides.)doc")
        .def(py::init<>())
        .def_readwrite("team_paint", &pluginsdk::CarColors::team_paint)
        .def_readwrite("team_color_override", &pluginsdk::CarColors::team_color_override)
        .def_readwrite("custom_color_override", &pluginsdk::CarColors::custom_color_override);

    py::class_<pluginsdk::ColorPosition>(m, "ColorPosition",
        R"doc(A position in a color grid (row, column).)doc")
        .def(py::init<>())
        .def(py::init<int, int>(), py::arg("row"), py::arg("column"))
        .def_readwrite("row", &pluginsdk::ColorPosition::row)
        .def_readwrite("column", &pluginsdk::ColorPosition::column);

    py::class_<pluginsdk::ItemAttribute>(m, "ItemAttribute",
        R"doc(An attribute applied to an item (paint, special edition, etc.).)doc")
        .def(py::init<>())
        .def_readwrite("type", &pluginsdk::ItemAttribute::type)
        .def_readwrite("value", &pluginsdk::ItemAttribute::value)
        .def_readwrite("color", &pluginsdk::ItemAttribute::color);

    py::enum_<pluginsdk::ItemAttribute::AttributeType>(m, "AttributeType",
        R"doc(Item attribute type enumeration.)doc")
        .value("UNKNOWN", pluginsdk::ItemAttribute::AttributeType::UNKNOWN)
        .value("PAINT", pluginsdk::ItemAttribute::AttributeType::PAINT)
        .value("ESPORT", pluginsdk::ItemAttribute::AttributeType::ESPORT)
        .value("SPECIALEDITION", pluginsdk::ItemAttribute::AttributeType::SPECIALEDITION)
        .value("USERCOLOR", pluginsdk::ItemAttribute::AttributeType::USERCOLOR)
        .export_values();

    py::class_<pluginsdk::ItemData>(m, "ItemData",
        R"doc(Data for a single equipped item in a loadout slot.)doc")
        .def(py::init<>())
        .def_readwrite("slot", &pluginsdk::ItemData::slot)
        .def_readwrite("product_id", &pluginsdk::ItemData::product_id)
        .def_readwrite("attributes", &pluginsdk::ItemData::attributes);

    py::class_<pluginsdk::Loadout>(m, "Loadout",
        R"doc(A complete player loadout with items and paint finish.)doc")
        .def(py::init<>())
        .def_readwrite("items", &pluginsdk::Loadout::items)
        .def_readwrite("paint_finish", &pluginsdk::Loadout::paint_finish);
}

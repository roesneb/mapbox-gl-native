#include <mbgl/tile/geojson_tile.hpp>
#include <mbgl/tile/geojson_tile_data.hpp>
#include <mbgl/renderer/query.hpp>
#include <mbgl/renderer/tile_parameters.hpp>
#include <mbgl/style/filter_evaluator.hpp>

namespace mbgl {

GeoJSONTile::GeoJSONTile(const OverscaledTileID& overscaledTileID,
                         std::string sourceID_,
                         const TileParameters& parameters,
                         mapbox::geometry::feature_collection<int16_t> features)
    : GeometryTile(overscaledTileID, sourceID_, parameters) {
    updateData(std::move(features));
}

void GeoJSONTile::updateData(mapbox::geometry::feature_collection<int16_t> features) {
    setData(std::make_unique<GeoJSONTileData>(std::move(features)));
}

void GeoJSONTile::setNecessity(Necessity) {}
    
void GeoJSONTile::querySourceFeatures(
    std::vector<Feature>& result,
    const SourceQueryOptions& options) {
    
    // Ignore the sourceLayer, there is only one
    auto layer = getData()->getLayer({});
    
    if (layer) {
        auto featureCount = layer->featureCount();
        for (std::size_t i = 0; i < featureCount; i++) {
            auto feature = layer->getFeature(i);
            
            // Apply filter, if any
            if (options.filter && !(*options.filter)(*feature)) {
                continue;
            }
            
            result.push_back(convertFeature(*feature, id.canonical));
        }
    }
}

} // namespace mbgl

PALETTE palette_grid "gfx/grid/palette_zona0_bg.png"

TILESET tileset_grid "gfx/grid/tileset_zona0_grid_v2.tsx" BEST ALL

MAP map_zona_demo "gfx/grid/tilemap_zona0_zone_demo10x10.tmx" layer_grid BEST 0
OBJECTS grid_markers_zona_demo "gfx/grid/tilemap_zona0_zone_demo10x10.tmx" layer_objects "name:string;x:s16;y:s16;grid_x:s16;grid_y:s16;conn_down:object;conn_right:object" "struct GridBoundaryMarker" GridBoundaryMarker
OBJECTS movables_markers_zona_demo "gfx/grid/tilemap_zona0_zone_demo10x10.tmx" layer_objects "name:string;x:s16;y:s16;grid_x:s16;grid_y:s16;direction:u16" "struct MovableInitMarker" MovableInitMarker
OBJECTS map_info_zona_demo "gfx/grid/tilemap_zona0_zone_demo10x10.tmx" layer_objects "name:string;width:s16;height:s16" "struct MapInfo" MapInfo

MAP map_zona_14 "gfx/grid/tilemap_zona0_zone14_v2.tmx" layer_grid BEST 0
OBJECTS grid_markers_zona_14 "gfx/grid/tilemap_zona0_zone14_v2.tmx" layer_objects "name:string;x:s16;y:s16;grid_x:s16;grid_y:s16;conn_down:object;conn_right:object" "struct GridBoundaryMarker" GridBoundaryMarker
OBJECTS movables_markers_zona_14 "gfx/grid/tilemap_zona0_zone14_v2.tmx" layer_objects "name:string;x:s16;y:s16;grid_x:s16;grid_y:s16;direction:u16" "struct MovableInitMarker" MovableInitMarker
OBJECTS map_info_zona_14 "gfx/grid/tilemap_zona0_zone14_v2.tmx" layer_objects "name:string;width:s16;height:s16" "struct MapInfo" MapInfo

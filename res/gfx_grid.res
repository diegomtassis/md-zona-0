PALETTE palette_grid "gfx/grid/palette_zona0_bg.png"

TILESET tileset_grid "gfx/grid/tileset_zona0_grid.tsx" BEST ALL

MAP map_zona0_zona14 "gfx/grid/tilemap_zona0_zone14.tmx" layer_grid BEST 0

OBJECTS grid_markers "gfx/grid/tilemap_zona0_zone14.tmx" layer_objects "name:string;x:s16;y:s16;grid_x:s16;grid_y:s16;conn_down:object;conn_right:object" "struct GridBoundaryMarker" GridBoundaryMarker

OBJECTS movables_markers "gfx/grid/tilemap_zona0_zone14.tmx" layer_objects "name:string;x:s16;y:s16;grid_x:s16;grid_y:s16;direction:u16" "struct MovableInitMarker" MovableInitMarker

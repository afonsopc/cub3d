FROM afonsopc/web-server AS runtime

COPY cub3d.js cub3d.wasm cub3d.data /web/
COPY cub3d.html /web/index.html
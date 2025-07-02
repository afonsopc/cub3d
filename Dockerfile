FROM alpine:latest AS runtime

COPY cub3d.js cub3d.wasm cub3d.data /web/
COPY cub3d.html /web/index.html
COPY server.py /
RUN apk add --no-cache python3
WORKDIR /web

CMD ["python3", "/server.py"]
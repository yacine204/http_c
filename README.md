# http_c

Tiny C HTTP server that accepts a single connection at a time on port 8080, parses the incoming HTTP request, and returns a fixed plain-text response.

## Build and Run

```
make
./server
```

- Listens on `0.0.0.0:8080`.
- Sends `HTTP/1.1 200 OK` with a short text body, (sends body rigth back at u if u included one).

## Project Layout

- server.c — socket setup, accept loop, response send.
- handlers/req_handler.c — naive HTTP request parsing into structs.
- include/request.h — request data structures shared across files.
- MakeFile — simple build and clean targets.

## Quick Test

```
curl -v http://localhost:8080/ -d "hello"
```

# http_c
A lightweight HTTP server written in C from scratch. Listens on port 8080, parses incoming HTTP requests, and routes them to the appropriate handler.

## Build and Run
make
./server

## Features
- Listens on 0.0.0.0:8080
- Routes GET, POST, PUT, DELETE requests
- GET — serves static files from assets/, routes / to index.html
- POST — creates or appends to a file in assets/
- PUT — overwrites a file in assets/
- DELETE — clears a file in assets/
- Returns proper HTTP status codes: 200, 201, 403, 404, 405, 500

## Project Layout
- server.c — socket setup, accept loop, request parsing, response serialization
- handlers/req_handler.c — HTTP request parsing into structs
- handlers/res_handler.c — request routing and response generation
- handlers/routing.c — static file routing with content-type detection
- handlers/header_handler.c — HTTP header generation
- handlers/response_status_line_handler.c — status line generation
- include/ — shared data structures (request, response, header, body, status line)
- assets/ — static files served by the server
- Makefile — build and clean targets

## Quick Test
```bash
# GET root
curl http://localhost:8080/

# POST a file
curl -X POST http://localhost:8080/test.txt -d "hello!"

# GET the file back
curl http://localhost:8080/test.txt

# PUT (overwrite) a file
curl -X PUT http://localhost:8080/test.txt -d "overwritten!"

# DELETE (clear) a file
curl -X DELETE http://localhost:8080/test.txt
```

## TODO
- [ ] handle concurrent requests using threads or fork()
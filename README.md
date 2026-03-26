# http_c
A lightweight HTTP server written in C from scratch. Listens on port 8080, parses incoming HTTP requests, and routes them to the appropriate handler.

## Build and Run
make
./server

## Features
- Listens on 0.0.0.0:8080
- Routes GET, POST, PUT, DELETE requests
- GET — reads and returns a file from the files/ directory
- POST — creates/appends to a file in files/ and returns its contents, (notice: the path u give in the command is the path where the file is going to be created in).
- PUT and DELETE — returns 501 Not Implemented (coming soon)
- Returns proper HTTP status codes: 200, 201, 403, 404, 405, 500

## Project Layout
- server.c — socket setup, accept loop, request parsing, response serialization
- handlers/req_handler.c — HTTP request parsing into structs
- handlers/res_handler.c — request routing and response generation
- handlers/header_handler.c — HTTP header generation
- handlers/response_status_line_handler.c — status line generation
- include/ — shared data structures (request, response, header, body, status line)
- files/ — directory where server reads and writes files
- Makefile — build and clean targets

## Quick Test
```bash
# GET root
curl http://localhost:8080/

# POST a file
curl -X POST http://localhost:8080/test.txt -d "hello!"

# GET the file back
curl http://localhost:8080/test.txt
```

## TODO
- [ ] make it handle many requests using threads or fork().
- [ ] add static file routing.

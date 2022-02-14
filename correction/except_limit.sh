#!/bin/bash

curl -X DELETE http://localhost:8080/nodelete/index.html
curl -X GET http://localhost:8080/noget/
curl -X POST http://localhost:8080/nopost/
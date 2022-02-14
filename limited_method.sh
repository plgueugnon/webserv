#!/bin/bash


curl -X DELETE http://localhost:8080/nodelete/index.html
curl -X POST http://localhost:8080/nopost/index.html
curl -X GET http://localhost:8080/noget/index.html
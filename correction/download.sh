#!/bin/bash


curl -v -X GET http://localhost:8080/pikachu/index.html -o index.html
curl -v -X GET http://localhost:8080/pokemon/carapuce -o carapuce
curl -v -X GET http://localhost:8080/pokemon/carapuce.png -o carapuce.png
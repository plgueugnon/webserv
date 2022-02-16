#!/bin/bash


echo "Deleting 3 pokemon";
echo "-------------------------------------------------------";
curl -v -X DELETE http://localhost:8080/pokedex_to_delete/bulbizare
echo "-------------------------------------------------------";
curl -v -X DELETE http://localhost:8080/pokedex_to_delete/mewtow
echo "-------------------------------------------------------";
curl -v -X DELETE http://localhost:8080/pokedex_to_delete/salameche
echo "-------------------------------------------------------";

echo "deleting file whith method not allowed";
echo "-------------------------------------------------------";
curl -v -X DELETE http://localhost:8080/nodelete/index.html
echo "-------------------------------------------------------";
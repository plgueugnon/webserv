#!/bin/bash

mkdir -p www/pokedex_to_delete

echo "kikoo je suis un bulbizare" > ../www/pokedex_to_delete/bulbizare
echo "kikoo je suis un salameche" > ../www/pokedex_to_delete/salameche
echo "kikoo je suis un mewtow" > ../www/pokedex_to_delete/mewtow

# curl -X DELETE http://localhost:8080/pokedex_to_delete/bulbizare
# curl -X DELETE http://localhost:8080/pokedex_to_delete/mewtow
# curl -X DELETE http://localhost:8080/pokedex_to_delete/salameche
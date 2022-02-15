#!/bin/bash

curl -v --resolve server2:8082:127.0.0.1 http://server2:8082
curl -v --resolve server1:8081:127.0.0.1 http://server1:8081
curl -v --resolve localhost:8080:127.0.0.1 http://localhost:8080
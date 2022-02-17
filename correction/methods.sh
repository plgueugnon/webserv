#!/bin/bash

# wrong http version
curl --http1.0  -X GET localhost:8080   

#unknown method
curl -X gets localhost:8080  
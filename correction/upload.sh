#!/bin/bash
index.html

curl -v -X POST -F "fileupload=@../upload.txt" -H 'Expect: ' http://localhost:8080/upload/upload_script.php

curl -v -X POST -F "fileupload=@../fuego.txt" -H 'Expect: ' http://localhost:8080/upload/upload_script.php
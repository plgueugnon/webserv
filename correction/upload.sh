#!/bin/bash

curl -v -X POST -F "fileupload=@../files_to_upload/upload.txt" -H 'Expect: ' http://localhost:8080/upload/upload_script.php

curl -v -X POST -F "fileupload=@../files_to_upload/fuego.txt" -H 'Expect: ' http://localhost:8080/upload/upload_script.php

curl -v -X POST -F "fileupload=@../files_to_upload/pokedex.txt" -H 'Expect: ' http://localhost:8080/upload/upload_script.php

curl -v -X POST -F "fileupload=@../files_to_upload/fuego.txt" -H 'Expect: ' http://localhost:8082/upload/upload_script.php

curl -v -X POST -F "fileupload=@../files_to_upload/fuego.txt" -H 'Expect: ' http://localhost:8082/upload/upload_script.php

curl -v -X POST -F "fileupload=@../files_to_upload/fatman" -H 'Expect: ' http://localhost:8082/upload/upload_script.php
<?php

echo "IN UPLOAD SCRIPT\n";

// Sauvegarde de tous les fichiers passés en input
foreach ($_FILES as $file) {
    $uploadfile = getenv("uploaddir") . "/" . basename($file['name']);
    echo "uploading => " . $file['name'] . "\n";
    if (move_uploaded_file($file['tmp_name'], $uploadfile)){
        echo "Le fichier " . $file['name'] . " a bien été envoyé\n";
    } else {
        echo "Erreur dans l'envoi du fichier " . $file['name'] . "\n";
    }
}

?>
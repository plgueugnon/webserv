<?php
// Sauvegarde de tous les fichiers passés en input
foreach ($_FILES as $file) {
    // var_dump($file);
    $errors= array();
    $path = $file['name'];
    $ext = pathinfo($path, PATHINFO_EXTENSION);
    $extensions= array("html", "txt", "php", "hpp");
    // var_dump($ext);
    if(in_array($ext,$extensions)=== false)
    {
        $errors[]="extension not allowed";
     }

    $uploadfile = $_ENV['TMP_DIR'] . basename($file['name']);
    if (empty($errors)==true)
    {
        echo "<!DOCTYPE html> <html> <body><h1><h1>";
        // echo "uploading => " . $file['name'] . "\n";
        (move_uploaded_file($file['tmp_name'], $uploadfile));
        echo "File '" . $file['name'] . "' was successfuly uploaded\n";
        echo " </h1> </body> </html>";
    } 
    else 
    {
        echo "<!DOCTYPE html> <html> <body><h1><h1 style=\"color:red;\">";
        echo "Error : Extension is not allowed !";
        echo " </h1> </body> </html>";
        // echo "Erreur dans l'envoi du fichier " . $file['name'] . "\n";
    }
}

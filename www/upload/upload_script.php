<?php
// Sauvegarde de tous les fichiers passés en input
foreach ($_FILES as $file) {
    var_dump($file);
    $errors= array();
    $path = $file['name'];
    $ext = pathinfo($path, PATHINFO_EXTENSION);
    $extensions= array("html", "txt", "php", "cpp", "hpp");
    var_dump($ext);
    if(in_array($ext,$extensions)=== false)
    {
        $errors[]="extension not allowed";
     }

    $uploadfile = $_ENV['TMP_DIR'] . basename($file['name']);
    if (empty($errors)==true)
    {
        echo "uploading => " . $file['name'] . "\n";
        (move_uploaded_file($file['tmp_name'], $uploadfile));
        echo "Le fichier " . $file['name'] . " a bien ete envoye\n";
    } 
    else 
    {
        print_r($errors);
        echo "Erreur dans l'envoi du fichier " . $file['name'] . "\n";
    }
}

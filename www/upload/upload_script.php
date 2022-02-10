<?php
// script melkior
// $file_uploads = On;
// $upload_tmp_dir = /Users/ygeslin/webserv/www/upload/;
echo "salut";
if(isset($_FILES['fichier'])){
   $errors= array();
   $file_name = $_FILES['fichier']['name'];
   $file_size =$_FILES['fichier']['size'];
   $file_tmp =$_FILES['fichier']['tmp_name'];
   $file_type=$_FILES['fichier']['type'];
   // $file_ext=strtolower(end(explode('.',$_FILES['fichier']['name'])));
   $file_ext = strtolower(pathinfo($file_name,PATHINFO_EXTENSION));
   
   $extensions= array("html", "txt", "php");
   
   if(in_array($file_ext,$extensions)=== false){
      $errors[]="extension not allowed";
   }
   
   if(empty($errors)==true){
      move_uploaded_file($file_tmp,"/Users/ygeslin/webserv/tmp/".$file_name);
      echo "Success";
   }else{
      print_r($errors);
   }
}

// Script Alienard
// echo "IN UPLOAD SCRIPT\n";
// // Sauvegarde de tous les fichiers passés en input
// foreach ($_FILES as $file) {
//     $uploadfile = "tmp" . "/" . basename($file['name']);
//     echo "uploading => " . $file['name'] . "\n";
//     if (move_uploaded_file($file['tmp_name'], $uploadfile)){
//         echo "Le fichier " . $file['name'] . " a bien été envoyé\n";
//     } else {
//         echo "Erreur dans l'envoi du fichier " . $file['name'] . "\n";
//     }
// }

?>
<?php
    echo "TEST ECHO FORM";
if(isset($_POST['last_name'])) {
    echo $_POST['last_name'];

    echo "LET'S CHECK ARG RECEIVED";
    var_dump($argv);

    echo "LET'S CHECK ALL INFO";
    phpinfo();
}
?>
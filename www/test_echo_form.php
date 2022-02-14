<?php
    echo "TEST ECHO FORM";
if(isset($_POST['last_name'])) {
    echo $_POST['last_name'];
}

    echo "\nLET'S CHECK ARG RECEIVED\n";
    var_dump($_POST);
    // var_dump($RAW_POST_DATA);
    // var_dump($argv);
    // echo $argv[0];
    // echo "\n";
    // echo $argv[1];
    // echo "\n";
    // echo $argv[2];
    // echo "\n";

    // echo "LET'S CHECK ALL INFO\n";
    // // phpinfo();
    // $indicesServer = array
    // ('PHP_SELF',
    // 'argv',
    // 'argc',
    // 'GATEWAY_INTERFACE',
    // 'CONTENT_LENGTH',
    // 'SERVER_ADDR',
    // 'SERVER_NAME',
    // 'SERVER_PROTOCOL',
    // 'REQUEST_METHOD',
    // 'QUERY_STRING',
    // 'DOCUMENT_ROOT',
    // 'SCRIPT_FILENAME',
    // 'SERVER_ADMIN',
    // 'SERVER_PORT',
    // 'SERVER_SIGNATURE',
    // 'PATH_TRANSLATED',
    // 'SCRIPT_NAME',
    // 'QUERY_STRING',
    // 'REQUEST_URI',
    // 'CONTENT_TYPE',
    // 'GATEWAY_CGI',
    // 'PATH_INFO',) ;
    
    // foreach ($indicesServer as $arg) {
    //     if (isset($_SERVER[$arg])) {
    //         echo $arg. ' : ' . $_SERVER[$arg] . "\n"  ;
    //     }
    //     else {
    //         echo $arg. " : \n" ;
    //     }
    // }



?>
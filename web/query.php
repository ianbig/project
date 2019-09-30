<?php 
    $cmd = $grep = "";
    if($_SERVER["REQUEST_METHOD"] == "POST")
    {

            // do file process
        $count = "grep -h -c " . $_POST["search_sentence"] . " test.txt";
        $cgrep = shell_exec($count);
        $cmd = "grep -h " . $_POST["search_sentence"] . " test.txt";
        $grep = shell_exec($cmd);
        
    }

    else {
       die("UNKNOWN ERROR: not using post method");
    }

    include("show.html");
?>


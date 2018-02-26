<?php
echo "POST: ".$_POST["name"]."\n";
echo "GET: ".$_GET["name"]."\n";
echo "REQUEST: ".$_REQUEST["name"]."\n";
echo "SERVER HTTP_HOST: ".$_SERVER["HTTP_HOST"]."\n";
echo "SERVER REQUEST_URI: ".$_SERVER["REQUEST_URI"]."\n";
echo "SERVER HTTP_X_FORWARDED_HOST: ".$_SERVER["HTTP_X_FORWARDED_HOST"]."\n";
echo "SERVER HTTPS: ".$_SERVER["HTTPS"]."\n";
echo "SERVER SERVER_PORT: ".$_SERVER["SERVER_PORT"]."\n";
echo "SERVER SERVER_PROTOCOL: ".$_SERVER["SERVER_PROTOCOL"]."\n";
echo "SERVER SERVER_NAME: ".$_SERVER["SERVER_NAME"]."\n";
foreach ($_SERVER as &$x){
	echo $x."\n"; 
}
?>

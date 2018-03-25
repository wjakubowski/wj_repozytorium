<?php

function __autoload($class_name) {
	include $class_name . '.php' ;
}

try {
	$contr = $_GET['sub'];
	$action = $_GET['action'];

	if (empty ($_GET['sub']) or empty ($_GET['action'])){
		echo "Nie wlasciwe zapytanie.";
	} 
	else{
		//print $contr. ' ' . $action ;
		$controller = new $contr ;
		//echo "<br>echo<br>";
		//echo $controller."<br>";
		echo $controller->$action() ;
	}
}
catch (Exception $e) {
  echo 'Blad: [' . $e->getCode() . '] ' . $e->getMessage() ;
}
$standalone = isset($_GET['standalone']) ?  $_GET['standalone'] :true ;
if($standalone)
	echo "<p><a href='index.php'>Powrot do programu glownego</a></p>";

?>
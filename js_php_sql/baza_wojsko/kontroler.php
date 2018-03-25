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
		$controller = new $contr ;
		echo $controller->$action() ;
	}
}
catch (Exception $e) {
  echo 'Blad: [' . $e->getCode() . '] ' . $e->getMessage() ;
}
$standalone = isset($_GET['standalone']) ?  $_GET['standalone'] :false ;
if($standalone)
	echo "<p><a href='.'>Powrot do programu glownego</a></p>";

?>
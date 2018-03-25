<?php
 
function __autoload($class_name) {
    include $class_name . '.php' ;
}
 
$user = new uzytkownicy ;
 
$result = $user->wszyscy_uzytkownicy();
$powodzenie = $result[0];
if($powodzenie){
   $table = $result[1];
   echo "<table><tr><th>Nazwisko</th><th>Imie</th><th>E-mail</th></tr>" ;
   foreach ( $table as $key => $record ) {
     echo "<tr><td>".$record['imie']."</td><td>".$record['nazwisko']."</td><td>".$record['email']."</td><td>".$record['id']."</td><tr>";
   }
   echo "</table>" ;
}
else{
	echo $result[1];
}
echo "<p><a href='index.php'>Powrot do programu glownego</a></p>";
?>

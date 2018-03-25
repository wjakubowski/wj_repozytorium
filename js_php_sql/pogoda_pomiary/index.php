<?php
 
function __autoload($class_name) {
    include $class_name . '.php' ;
}

$user = new uzytkownicy;
$device = new urzadzenia;

if ( $user->is_logged() )
{  
	echo "<p>Jestes zalogowany jako uzytkownik</p>" ;
     echo "<p><a href='kontroler.php?sub=uzytkownicy&action=uzytkownik'>Dane uzytkownika</a></p>" ;
     echo "<p><a href='listuj_uzytkownikow.html'>Zarejestrowani uzytkownicy</a></p>" ;
	 echo "<p><a href='kontroler.php?sub=uzytkownicy&action=usun_konto'>Usuń konto</a></p>" ;
     echo "<p><a href='kontroler.php?sub=uzytkownicy&action=logout'>Wylogowanie z serwisu</a></p>" ;
	 echo "<br>";
	 echo "<p><a href='dodaj_urzadzenie.html'>Dodaj nowe urządzenie</a></p>" ;
	 echo "<p><a href='listuj_urzadzenia.html'>Listuje urządzenia</a></p>" ;


}elseif( $device->is_logged() ){
	echo "<p>Jestes zalogowany jako urzadzenie</p>" ;
	echo "<p><a href='dodaj_pomiar.html'>Dodaj pomiar</a></p>" ;
	echo "<p><a href='kontroler.php?sub=urzadzenia&action=logout'>Wylogowanie urzadzenia z serwisu</a></p>" ;
}
else{
	echo "<p>Nie jesteś zalogowany</p>" ;
	echo "<p><a href='rejestracja.html'>Rejestracja w serwisie</a></p>";
    echo "<p><a href='logowanie.html'>Logowanie do serwisu</a></p>";
	echo "<p><a href='zaloguj_jako_urzadzenie.html'>Zaloguj sie jako urzadzenie</a></p>" ;
}

echo "<p><a href='instrukcja.html'>INSTRUKCJA UZYTKOWNIKA</a></p>" ;

?>
<?php

/*
README

Klasa odpowiada za wszystkie czynnoœci które mo¿e wykonywaæ urzytkownik w serwisie, tzn:
-rejestracja w serwisie,
-zalogowanie siê poprzez swój nr email i has³o,
-dodawanie/usuwanie urz¹dzeñ zbieraj¹cych pomiary,
-przegl¹danie urz¹dzeñ i zgromadzonych przez nie danych(przegl¹daj¹c swoje urz¹dzenia mo¿na zapamiêtaæ
ID jednego z urz¹dzeñ, zalogowaæ siê jako urz¹dzenie i dodawaæ pomiary)
-wylogowanie z serwisu.

*/

class uzytkownicy
{
	protected $db;
    
	function __construct()
	{
		$this->db = new baza ;
		session_start();		
	}
    
    /*
     *  Zapis nowego uzytkownika w bazie
     */
    
    function zapisz()
    {	
		$rekord = array();

		$rekord['imie'] = $_POST['imie'];
		$rekord['nazwisko'] = $_POST['nazwisko'];
		$rekord['email'] = $_POST['email'];
		$rekord['haslo']  = $_POST['haslo'];
				
		echo "<p>";
		echo "Wprowadzone dane<br/>";
		echo "Imie: " . $rekord['imie'] . " <br/>";
		echo "Nazwisko: " . $rekord['nazwisko'] . " <br/>";
		echo "E-mail: " . $rekord['email'] . " <br/>";
		echo "Haslo: " . $rekord['haslo'] . " <br/>";
		echo "</p>";
		
		$result = $this->db->zapisz('uzytkownicy', $rekord);
		$powodzenie = $result[0];
		if($powodzenie){
			echo "Zapisano konto.";
		}
		else{
			echo "Nie udalo sie zapisac konta: ".$result[1];
		}
    }
	
    /*
     * Logowanie uzytkownika do serwisu 
     */

    function login(){

		$email = $_POST['email'];
		$haslo = $_POST['haslo'];
		
		$result = $this->db->wybierz('uzytkownicy', array('email'=>$email,'haslo'=>$haslo),1);
		$powodzenie = $result[0];
		if($powodzenie){
			$istnieje = sizeof($result[1]) > 0 ;
			//echo '<br>RESULT ID: '.$result[1][0]['id'];
			if ($istnieje) {
				$_SESSION['auth'] = true;
				$_SESSION['type'] = 'uzytkownik';
				$_SESSION['uzytkownik_id']   = $result[1][0]['id'];
				//echo "ID urzytkownika: ".$_SESSION['uzytkownik_id'];
				echo "Uzytkownik zalogowany.";
			}
			else{
				echo "Uzytkownik nie zalogowany.";
			}
		}
		else{
			echo "Baza danych nie dziala poprawnie: ".$result[1];
		}        
    }
    
    /*
     * Sprawdzenie czy uzytkownik jest zalogowany 
     */
    
    function is_logged()
    {
		$ret;
        if (isset($_SESSION['auth']) and $_SESSION['type'] == 'uzytkownik') {
            $ret = $_SESSION['auth'] ;
        } else {
            $ret = false;
        }
        return $ret;
    }
	
    /*
     * Wylogowanie uzytkownika z serwisu 
     */
    
    function logout()
    {
		if($this->is_logged()){
			unset($_SESSION);
			session_destroy();
			echo 'Uzytkownik wylogowany ';
		}
		else{
			echo "Uzytkownik nie byl zalogowany.";
		}
    }
	
	/*
	 * Usuwanie konta uzytkownika ze wszystkimi powi¹zanymi danymi 
	 */
	 
	function usun_konto(){
		if($this->is_logged()){
			$result = $this->db->usun('uzytkownicy', $_SESSION['uzytkownik_id']);
			$powodzenie = $result[0];
			if($powodzenie){
				$this->logout();
				echo "Usunieto konto.";
			}
			else{
				echo "Nie udalo sie usunac konta: ".$result[1];
			}
		}
		else{
			echo "Nie mozesz usuwac konta, nie jestes zalogowany.";
		}
	}
    
    /*
     * Pobranie danych o uzytkowniku z bazy 
     */
    
    function uzytkownik()
    {
		if(! $this->is_logged()){
			echo "Nie jesteœ zalogowany.";
		}
		else{
			$result = $this->db->wybierz('uzytkownicy', array('id'=>$_SESSION['uzytkownik_id']),1);
			$powodzenie = $result[0];	
			if($powodzenie){
				$uzytkownik = $result[1][0];
				echo "<p>";
				echo "Dane uzytkownika:<br/>";
				echo "ID: " . $uzytkownik['id'] . " <br/>";
				echo "Imie: " . $uzytkownik['imie'] . " <br/>";
				echo "Nazwisko: " . $uzytkownik['nazwisko'] . " <br/>";
				echo "E-mail: " . $uzytkownik['email'] . " <br/>";
				echo "Haslo: " . $uzytkownik['haslo'] . " <br/>";
				echo "</p>";
			}
			else{
				echo "Baza danych nie dziala poprawnie: ".$result[1];
			}
		}
    }
    
    /*
     * Pobranie danych wszystkich uzytkownikow z bazy 
     */
    
    function listuj_uzytkownikow()
    {
		$powodzenie;
		$info;
		$wynik;
		if(! $this->is_logged()){
			$info = "Nie jestes zalogowany.";
			$powodzenie = false;
		}
		else{
			$result = $this->db->wybierz('uzytkownicy');
			$powodzenie = $result[0];	
			if($powodzenie){
				$wynik = $result[1];
				$powodzenie = true;
			}
			else{
				$info = "Baza danych nie dziala poprawnie: ".$result[1];
				$powodzenie = false;
			}
		}
		if($powodzenie){
			echo json_encode(array($powodzenie, $wynik));
		}
		else{
			echo json_encode(array($powodzenie, $info));
		}
    }
    
	/*
	 * Dodawanie nowego urz¹dzenia
	 */
	
    function dodaj_urzadzenie(){
		$rekord = array();
		$rekord['opis'] = $_POST['opis'];
		$rekord['szer_geo'] = $_POST['szer_geo'];
		$rekord['dl_geo'] = $_POST['dl_geo'];

		echo "<p>";
		echo "Wprowadzone dane<br/>";
		echo "opis: " . $rekord['opis'] . " <br/>";
		echo "szer. geo.: " . $rekord['szer_geo'] . " <br/>";
		echo "dl. geo.: " . $rekord['dl_geo'] . " <br/>";
		echo "</p>";
		
		if(! $this->is_logged()){
			echo "Nie jestes zalogowany.";
		}
		else{
			$uzytkownik_id = $_SESSION['uzytkownik_id'];
			$rekord['uzytkownik_id'] = $uzytkownik_id;
			
			$result = $this->db->zapisz('urzadzenia',$rekord);
			$powodzenie = $result[0];	
			if($powodzenie){
				echo "Dodano urzadzenie.";
			}
			else{
				echo "Baza danych nie dziala poprawnie: ".$result[1];
			}
		}
    }
	/*
	 * Usuwanie urz¹dzenia i powi¹zanych danych
	 */
	
	function usun_urzadzenie()
	{
		$urzadzenie_id = $_POST['urzadzenie_id'];
		
		if(! $this->is_logged()){
			echo "Nie jestes zalogowany.";
		}
		else{
			
			$result = $this->db->usun('urzadzenia',$urzadzenie_id);
			$powodzenie = $result[0];	
			if($powodzenie){
				echo "Usunieto urzadzenie.";
			}
			else{
				echo "Baza danych nie dziala poprawnie: ".$result[1];
			}
		}
    }

	
	/*
	 * Listowanie urz¹dzeñ
	 */
	
	function listuj_urzadzenia(){
		$powodzenie;
		$info;
		$wynik;
		if(! $this->is_logged()){
			$info = "Nie jestes zalogowany.";
			$powodzenie = false;
		}
		else{
			$result = $this->db->wybierz('urzadzenia', array('uzytkownik_id'=>$_SESSION['uzytkownik_id']));
			$powodzenie = $result[0];	
			if($powodzenie){
				$wynik = $result[1];
				$powodzenie = true;
			}
			else{
				$info = "Baza danych nie dziala poprawnie: ".$result[1];
				$powodzenie = false;
			}
		}
		if($powodzenie){
			echo json_encode(array($powodzenie, $wynik));
		}
		else{
			echo json_encode(array($powodzenie, $info));
		}		
	}
	
	/*
	 * Listowanie pomiarów z danego urz¹dzenia
	 */
	
	function listuj_pomiary(){
		$urzadzenie_id = $_POST['urzadzenie_id'];
		$powodzenie;
		$info;
		$wynik;
		if(! $this->is_logged()){
			$info = "Nie jestes zalogowany.";
			$powodzenie = false;
		}
		else{
			$result = $this->db->wybierz('pomiary', array('urzadzenie_id'=>$urzadzenie_id));
			$powodzenie = $result[0];	
			if($powodzenie){
				$wynik = $result[1];
				$powodzenie = true;
			}
			else{
				$info = "Baza danych nie dziala poprawnie: ".$result[1];
				$powodzenie = false;
			}
		}
		if($powodzenie){
			echo json_encode(array($powodzenie, $wynik));
		}
		else{
			echo json_encode(array($powodzenie, $info));
		}		
	}
	
    
    
}

?>
<?php

/*
README

Klasa odpowiada za wszystkie czynnoœci które mo¿e wykonywaæ urz¹dzenie w serwisie, tzn:
-zalogowanie siê poprzez swój nr ID w bazie,
-dodanie nowego pomiaru do bazy,
-wylogowanie z serwisu.

*/

class urzadzenia
{
    
	protected $db;
    
	function __construct()
	{
		$this->db = new baza ;
		session_start();		
	}

    /*
     * Logowanie urz¹dzenia w serwisie
     */
    
    function login(){
		$urzadzenie_id = $_POST['urzadzenie_id'];
		$result = $this->db->wybierz('urzadzenia', array('id'=>$urzadzenie_id),1);
		$powodzenie = $result[0];
		if($powodzenie){
			$istnieje = sizeof($result[1]) > 0 ;
			//echo '<br>RESULT ID: '.$result[1][0]['id'];
			if ($istnieje) {
				$_SESSION['auth'] = true;
				$_SESSION['type'] = 'urzadzenie';
				$_SESSION['urzadzenie_id']   = $result[1][0]['id'];
				//echo "ID urzadzenia: ".$_SESSION['urzadzenie_id'];
				echo "Urzadzenie zalogowane.";
			}
			else{
				echo "Urzadzenie nie zalogowane.";
			}
		}
		else{
			echo "Baza danych nie dziala poprawnie: ".$result[1];
		}        
    }
    
    /*
     * Sprawdzenie czy urzadzenie jest rozpoznawane 
     */
    
    function is_logged()
    {
		$ret;
        if (isset($_SESSION['auth']) and isset($_SESSION['type']) and $_SESSION['type'] == 'urzadzenie') {
            $ret = $_SESSION['auth'] ;
        } else {
            $ret = false;
        }
        return $ret;
    }
    /*
     * Przerwanie sesji urz¹dzenia 
     */
    
    function logout()
    {
		if($this->is_logged()){
			unset($_SESSION);
			session_destroy();
			echo 'Urzadzenie wylogowane. ';
		}
		else{
			echo "Urzadzenie nie bylo zalogowane.";
		}
    }
	
	/*
	 * Rejestracja pomiaru przez urz¹dzenie
	 */
	
	function dodaj_pomiar()
	{
		
		$rekord = array();
		$rekord['czas'] = $_POST['czas'];
		$rekord['temperatura'] = $_POST['temperatura'];
		$rekord['cisnienie'] = $_POST['cisnienie'];
		$rekord['sila_wiatru'] = $_POST['sila_wiatru'];
		$rekord['kierunek_wiatru'] = $_POST['kierunek_wiatru'];

		$info;
		$wynik;
		$powodzenie;
		if(! $this->is_logged()){
			$info = "Urzadzenie nie jest zalogowane.";
			$powodzenie = false;
		}
		else{
			$urzadzenie_id = $_SESSION['urzadzenie_id'];
			$rekord['urzadzenie_id'] = $urzadzenie_id;
			$result = $this->db->zapisz('pomiary',$rekord);
			$powodzenie = $result[0];	
			if($powodzenie){
				$info = "Dodano pomiar.";
			}
			else{
				$info = "Baza danych nie dziala poprawnie: ".$result[1];
			}
		}
		echo json_encode(array($powodzenie, $info));
    }
}

?>


<?php

/*
README

Klasa odpowiada za połączenie serwisu z bazą danych SQLite. Stanowi uniwersalny interfejs do komunikacji z bazą danych, umożliwiający 
modyfikację wszystkich relacji w bazie danych (skrypt "baza.sql", relacje: uzytkownicy, urzadzenia, pomiary). 
Metody klasy pobierają dane w postaci tablic asocjacyjnych. Zwracana wartość to tablica, której pierwszy element to wertość logiczna
mówiąca o powodzeniu operacji, a drugi to dane/komunikaty o wykonaniu operacji/ lub ewentualnie komunikaty o błędach.

*/

class baza{
	
	static $dsn = 'sqlite:sql/baza.db' ;
	protected static $db;
	
	function __construct()
	{
      self::$db = new PDO ( self::$dsn ) ;
      self::$db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION) ;
	}
	
	/*
	realizacja polecenia INSERT
	*/
	
	function zapisz($nazwa_tabeli, $properties)
    {
		$info;
		$powodzenie;
		try {
			$sql = 'INSERT INTO '.$nazwa_tabeli.' ( ';
			foreach ( $properties as $key => $property ) {
				if($property != null){
					$sql .= ' "'.$key.'" , ';
				}
			}
			$sql = substr($sql, 0, -2);
			$sql .=' ) VALUES ( ';
			foreach ( $properties as $key => $property ) {
				if($property != null){
					$sql .= ' :'.$key.' , ';
				}
			}
			$sql = substr($sql, 0, -2);
			$sql .=' )';
			$statement = self::$db->prepare($sql);
			foreach ( $properties as $key => $property ) {
				if($property != null){
					$statement->bindValue(':'.$key, $property);
				}
			}
			$powodzenie = $statement->execute();
			if($powodzenie){
				$info = "Zapisano rekord.";
			}
			else{
				$info = "Nie udalo sie zapisac rekordu.";
			}
		} catch (Exception $e) {
			$info = 'Wystapil wyjatek: ' . $e->getMessage();
			$powodzenie = false;
		}
		return array($powodzenie,$info);
    }
	
	/*
	realizacja polecenia DELETE
	*/
	
	function usun($nazwa_tabeli, $id){
		$info;
		$powodzenie;
		try {
			$sql = 'DELETE FROM '.$nazwa_tabeli.' WHERE id = :id ';
			$statement = self::$db->prepare($sql);
			$statement->bindValue(':id', $id);
			$powodzenie = $statement->execute();
			if($powodzenie){
				$info = "Usunieto rekord.";
			}
			else{
				$info = "Nie udalo sie usuna rekordu.";
			}
		} catch (Exception $e) {
			$info = 'Wystapil wyjatek: ' . $e->getMessage();
			$powodzenie = false;
		}
		return array($powodzenie,$info);
	}
	
	/*
	realizacja polecenia SELECT
	*/
	
	function wybierz($nazwa_tabeli, $properties,$limit)
    {	
		$info;
		$wyniki;
		$powodzenie;
		try {
			$sql = 'SELECT * FROM '.$nazwa_tabeli;
			if($properties != null or sizeof($properties)){
				$sql .= ' WHERE';
				foreach ( $properties as $key => $property ) {
					if($property != null){
						$sql .= (' '.$key.' = \''.$property.'\' and ');
					}
				}
				$sql = substr($sql, 0, -5);
			}
			if($limit != null){
				$sql .= (' LIMIT '.$limit);
			}
			//echo '<br>'.$sql.'<br>';
			$results = self::$db->query($sql);
			$powodzenie = $results->execute();
			if($powodzenie){
				$wyniki = $results->fetchAll();
			}
			else{
				$info = "Nie udalo sie pobrac danych.";
			}
		} catch (Exception $e) {
			$info = 'Wystapil wyjatek: ' . $e->getMessage();
			$powodzenie = false;
		}
		if($powodzenie){
			return array($powodzenie,$wyniki);
		}
		else{
			return array($powodzenie,$info);
		}
		
    }
	
	
}


?>
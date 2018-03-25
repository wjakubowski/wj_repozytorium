<?php
class baza{
	
	/*static $dbname = 'u3jakubowski' ;
	static $host = 'pascal.fis.agh.edu.pl' ;
	static $dbuser = 'u3jakubowski' ;
	static $dbpass = 'abcd' ;*/
	//psql "sslmode=require host=sl-eu-fra-2-portal.4.dblayer.com port=16088 dbname=compose user=admin"
	static $dbname = 'compose' ;
	static $host = 'sl-eu-fra-2-portal.4.dblayer.com' ;
	static $dbuser = 'admin' ;
	static $dbpass = 'Kokos_117' ;
	
	protected static $db;
	
	function __construct()
	{
		//self::$db = new PDO ( "pgsql:host=pascal.fis.agh.edu.pl;port=5432;dbname=u3jakubowski;user=u3jakubowski;password=abcd") ;
		//self::$db = new PDO ( "pgsql:require host=sl-eu-fra-2-portal.4.dblayer.com port=16088 dbname=compose user=admin password=Kokos_117") ;
		self::$db = new PDO ( "pgsql:sslmode=require host=sl-eu-fra-2-portal.4.dblayer.com port=16088 dbname=compose user=admin password=Kokos_117") ;
		self::$db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION) ;
	}
	
	/*
	realizacja polecenia INSERT
	*/
	
	function insert($nazwa_tabeli, $properties)
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
			
			//echo '<br> SQL: '.$sql.'<br>';
			$statement = self::$db->prepare($sql);
			if ($statement) {
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
			}
			else{
				$powodzenie = false;
				$info = "PDO::errorInfo(): ".json_encode(self::$db->errorInfo());
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
	
	function delete($nazwa_tabeli, $conditions){
		$info;
		$powodzenie;
		try {
			$sql = 'DELETE FROM '.$nazwa_tabeli;
			
			if($conditions != null or sizeof($conditions)){
				$sql .= ' WHERE ';
				foreach ( $conditions as $key => $property ) {
					if($property != null){
						$sql .= ' "'.$key.'" = :'.$key.' and ';
					}
				}
				$sql = substr($sql, 0, -5);
			}
			else{
				$powodzenie = false;
				$info = "Mosisz podac warunki na usuwane obiekty.";
				return array($powodzenie,$info);
			}
			
			
			//echo '<br> SQL: '.$sql.'<br>';
			$statement = self::$db->prepare($sql);			
			
			if ($statement) {
				foreach ( $conditions as $key => $property ) {
					if($property != null){
						$statement->bindValue(':'.$key, $property);
					}
				}
				$powodzenie = $statement->execute();
				if($powodzenie){
					$info = "Usunieto rekord.";
				}
				else{
					$info = "Nie udalo sie usunac rekordu.";
				}
			}
			else{
				$powodzenie = false;
				$info = "PDO::errorInfo(): ".json_encode(self::$db->errorInfo());
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
	
	function select($nazwa_tabeli, $properties,$limit)
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
						$sql .= (' "'.$key.'" = :'.$key.' and ');
					}
				}
				$sql = substr($sql, 0, -5);
			}
			if($limit != null){
				$sql .= (' LIMIT '.$limit);
			}

			//echo '<br> SQL: '.$sql.'<br>';
			$statement = self::$db->prepare($sql);
			if ($statement) {
				foreach ( $properties as $key => $property ) {
					if($property != null){
						$statement->bindValue(':'.$key, $property);
					}
				}
				
				$powodzenie = $statement->execute();
				if($powodzenie){
					$wyniki = $statement->fetchAll();
				}
				else{
					$info = "Nie udalo sie pobrac danych.";
				}
			}
			else{
				$powodzenie = false;
				$info = "PDO::errorInfo(): ".json_encode(self::$db->errorInfo());
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
	
	/*
	realizacja polecenia UPDATE
	*/
	
	
	function update($nazwa_tabeli, $properties, $conditions)
    {	
		$info;
		$wyniki;
		$powodzenie;
		try {
			$sql = 'UPDATE '.$nazwa_tabeli." SET ";
	
			foreach ( $properties as $key => $property ) {
				if($property != null){
					$sql .= ' "'.$key.'" = :'.$key.' , ';
				}
			}
			$sql = substr($sql, 0, -2);
			
						
			if($conditions != null or sizeof($conditions)){
				$sql .= ' WHERE ';
				foreach ( $conditions as $key => $property ) {
					if($property != null){
						$sql .= ' '.$key.' = :c'.$key.' and ';
					}
				}
				$sql = substr($sql, 0, -5);
			}
			
			//echo '<br> SQL: '.$sql.'<br>';
			
			$statement = self::$db->prepare($sql);
			if ($statement) {
				foreach ( $properties as $key => $property ) {
					if($property != null){
						$statement->bindValue(':'.$key, $property);
					}
				}
				foreach ( $conditions as $key => $property ) {
					if($property != null){
						$statement->bindValue(':c'.$key, $property);
					}
				}
				
				$powodzenie = $statement->execute();
				if($powodzenie){
					$info = "Zmodyfikowano dane.";
				}
				else{
					$info = "Nie udalo sie zmodyfikowac danych.";
				}
			}
			else{
				$powodzenie = false;
				$info = "PDO::errorInfo(): ".json_encode(self::$db->errorInfo());
			}
		} catch (Exception $e) {
			$info = 'Wystapil wyjatek: ' . $e->getMessage();
			$powodzenie = false;
		}
		return array($powodzenie,$info);
    }
	
	//http://localhost:9090/~3jakubowsk/bazy_proj/kontroler.php?sub=baza&action=table_names
	function table_names(){
		try{
			$sql = 
				"SELECT table_name
				FROM information_schema.tables
				WHERE table_schema='wojsko'
				AND table_type='BASE TABLE';";
			
			$statement = self::$db->query($sql);
			$powodzenie = $statement->execute();
			
			if($powodzenie){
				$wyniki = $statement->fetchAll();
				echo json_encode(array($powodzenie,$wyniki));
			}
			else{
				$info = "Nie udalo sie pobrac danych.";
				echo json_encode(array($powodzenie,$info));
			}
		} catch (Exception $e) {
			$info = 'Wystapil wyjatek: ' . $e->getMessage();
			$powodzenie = false;
			echo json_encode(array($powodzenie,$info));
		}
	}
	
	//http://localhost:9090/~3jakubowsk/bazy_proj/kontroler.php?sub=baza&action=table_columns&relacja=czlowiek
	function table_columns(){
		try{
			$relacja = $_GET['relacja'];
			$sql = 
				"SELECT attname            AS col
					 , atttypid::regtype  AS datatype
					   -- more attributes?
				FROM   pg_attribute
				WHERE  attrelid = 'wojsko.".$relacja."'::regclass  -- table name, optionally schema-qualified
				AND    attnum > 0
				AND    NOT attisdropped
				ORDER  BY attnum;";
			
			$statement = self::$db->query($sql);
			$powodzenie = $statement->execute();
			
			if($powodzenie){
				$wyniki = $statement->fetchAll();
				echo json_encode(array($powodzenie,$wyniki));
			}
			else{
				$info = "Nie udalo sie pobrac danych.";
				echo json_encode(array($powodzenie,$info));
			}
		} catch (Exception $e) {
			$info = 'Wystapil wyjatek: ' . $e->getMessage();
			$powodzenie = false;
			echo json_encode(array($powodzenie,$info));
		}
		
	}
	
	
	function dane_zolnierza(){
		$id_zolnierza = $_GET['id_zolnierza'];
		
		try{
			$dane = array();
			$sql = 
				"SELECT * from wojsko.czlowiek where id =".$id_zolnierza;
			
			$statement = self::$db->query($sql);
			$powodzenie = $statement->execute();
			$czlowiek = $statement->fetchAll();
			$dane['imie'] = $czlowiek[0]['imie'];
			$dane['nazwisko'] = $czlowiek[0]['nazwisko'];
			
			$sql = 
				"SELECT * from wojsko.oddzial where id =".$czlowiek[0]['oddzial_id'];
			
			$statement = self::$db->query($sql);
			$powodzenie = $powodzenie and $statement->execute();
			$oddzial = $statement->fetchAll();
			$dane['oddzial'] = $oddzial[0]['opis'];
			
			$sql = 
				"SELECT * from wojsko.oddzial where id =".$oddzial[0]['oddzial_nadrzedny_id'];
			
			$statement = self::$db->query($sql);
			$powodzenie = $powodzenie and $statement->execute();
			$oddzial_nadrzedny = $statement->fetchAll();
			$dane['oddzial_nadrzedny'] = $oddzial_nadrzedny[0]['opis'];
			
			$sql = 
				"SELECT * from wojsko.stopnie where id =".$czlowiek[0]['stopnie_id'];
			
			$statement = self::$db->query($sql);
			$powodzenie = $powodzenie and $statement->execute();
			$stopien = $statement->fetchAll();
			$dane['stopien'] = $stopien[0]['opis'];
			
			
			$sql = 
				"SELECT r.opis from wojsko.rozkaz_wykonawca as r_w join 
					wojsko.rozkazy as r on r_w.rozkazy_id = r.id where r_w.czlowiek_id =".$id_zolnierza;
			
			$statement = self::$db->query($sql);
			$powodzenie = $powodzenie and $statement->execute();
			$rozkazy = $statement->fetchAll();
			$dane['rozkazy'] = $rozkazy;
			
			$sql = 
				"SELECT r.opis from wojsko.rozkazy as r where r.nadawca_id =".$id_zolnierza;
			
			$statement = self::$db->query($sql);
			$powodzenie = $powodzenie and $statement->execute();
			$rozkazy_wydane = $statement->fetchAll();
			$dane['rozkazy_wydane'] = $rozkazy_wydane;
			
			$sql = 
				"SELECT (select opis from wojsko.uprawnienia where id = uprawnienia_id) from wojsko.czlowiek_uprawnienia_view where czlowiek_id =".$id_zolnierza;
			
			$statement = self::$db->query($sql);
			$powodzenie = $powodzenie and $statement->execute();
			$uprawnienia = $statement->fetchAll();
			$dane['uprawnienia'] = $uprawnienia;
			
			$sql = 
				"SELECT (select opis from wojsko.szkolenia where id = szkolenia_id) from wojsko.czlowiek_szkolenia where czlowiek_id =".$id_zolnierza;
			
			$statement = self::$db->query($sql);
			$powodzenie = $powodzenie and $statement->execute();
			$szkolenia = $statement->fetchAll();
			$dane['szkolenia'] = $szkolenia;

			
			if($powodzenie){
				echo json_encode(array($powodzenie,$dane));
			}
			else{
				$info = "Nie udalo sie pobrac danych.";
				echo json_encode(array($powodzenie,$info));
			}
		} catch (Exception $e) {
			$info = 'Wystapil wyjatek: ' . $e->getMessage();
			$powodzenie = false;
			echo json_encode(array($powodzenie,$info));
		}
	}
	
	
	
}


?>
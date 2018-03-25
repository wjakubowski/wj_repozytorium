<?php

class baza_obsluga{
	
	protected static $db;
	
	function __construct()
	{
		self::$db = new baza;
	}
	
	/*
	realizacja polecenia INSERT
	*/
	
	function insert()
    {
		$nazwa_tabeli = $_GET['relacja'];
		$properties = $_POST["properties_set"];
		echo json_encode(self::$db->insert('wojsko.'.$nazwa_tabeli,$properties));
    }
	
	/*
	realizacja polecenia DELETE
	*/
	
	function delete(){
		$nazwa_tabeli = $_GET['relacja'];
		$conditions = $_POST["conditions_set"];
		echo json_encode(self::$db->delete('wojsko.'.$nazwa_tabeli,$conditions));
	}
	
	/*
	realizacja polecenia SELECT
	*/
	
	function select()
    {	
		$nazwa_tabeli = $_GET['relacja'];
		$conditions = $_POST["conditions_set"];
		echo json_encode(self::$db->select('wojsko.'.$nazwa_tabeli,$conditions));
    }
	
	/*
	realizacja polecenia UPDATE
	*/
	
	
	function update()
    {	
		$nazwa_tabeli = $_GET['relacja'];
		$properties = $_POST["properties_set"];
		$conditions = $_POST["conditions_set"];
		echo json_encode(self::$db->update('wojsko.'.$nazwa_tabeli,$properties,$conditions));
    }
}


?>
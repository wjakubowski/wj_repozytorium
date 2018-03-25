DROP TABLE if exists pomiary;

DROP TABLE if exists urzadzenia;

DROP TABLE if exists uzytkownicy;

CREATE TABLE uzytkownicy (
  id INTEGER  NOT NULL  ,
  imie VARCHAR  NOT NULL  ,
  nazwisko VARCHAR  NOT NULL  ,
  haslo VARCHAR    ,
  email VARCHAR  NOT NULL  UNIQUE   ,
PRIMARY KEY(id));



CREATE TABLE urzadzenia (
  id INTEGER  NOT NULL  ,
  uzytkownik_id INTEGER  NOT NULL  ,
  opis VARCHAR    ,
  szer_geo FLOAT  NOT NULL  ,
  dl_geo FLOAT  NOT NULL    ,
PRIMARY KEY(id),
  FOREIGN KEY(uzytkownik_id)
    REFERENCES uzytkownicy(id)
      ON DELETE CASCADE
      ON UPDATE NO ACTION);



CREATE TABLE pomiary (
  urzadzenie_id INTEGER  NOT NULL  ,
  czas INTEGER  NOT NULL  ,
  temperatura FLOAT    ,
  cisnienie FLOAT    ,
  sila_wiatru FLOAT    ,
  kierunek_wiatru FLOAT      ,
PRIMARY KEY(urzadzenie_id, czas),
  FOREIGN KEY(urzadzenie_id) 
    REFERENCES urzadzenia(id)
      ON DELETE CASCADE
      ON UPDATE NO ACTION);








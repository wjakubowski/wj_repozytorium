drop schema wojsko cascade;
create schema wojsko;
set search_path to wojsko;

CREATE TABLE sprzet (
  id SERIAL  NOT NULL ,
  opis VARCHAR   NOT NULL   ,
PRIMARY KEY(id));



CREATE TABLE typ_oddzialu (
  id SERIAL  NOT NULL ,
  miara INTEGER    ,
  opis VARCHAR      ,
PRIMARY KEY(id));



CREATE TABLE szkolenia (
  id SERIAL  NOT NULL ,
  opis VARCHAR   NOT NULL   ,
PRIMARY KEY(id));



CREATE TABLE stopnie (
  id SERIAL  NOT NULL ,
  miara INTEGER    ,
  opis VARCHAR      ,
PRIMARY KEY(id));



CREATE TABLE uprawnienia (
  id SERIAL  NOT NULL ,
  opis VARCHAR   NOT NULL   ,
PRIMARY KEY(id));



CREATE TABLE oddzial (
  id SERIAL  NOT NULL ,
  dowaodca_id INTEGER    ,
  oddzial_nadrzedny_id INTEGER    ,
  opis VARCHAR   NOT NULL ,
  typ_id INTEGER   NOT NULL   ,
PRIMARY KEY(id),
  FOREIGN KEY(oddzial_nadrzedny_id)
    REFERENCES oddzial(id),
  FOREIGN KEY(typ_id)
    REFERENCES typ_oddzialu(id));



CREATE TABLE uprawnienia_szkolenia (
  uprawnienia_id INTEGER   NOT NULL ,
  szkolenia_id INTEGER   NOT NULL   ,
PRIMARY KEY(uprawnienia_id, szkolenia_id),
  FOREIGN KEY(uprawnienia_id)
    REFERENCES uprawnienia(id),
  FOREIGN KEY(szkolenia_id)
    REFERENCES szkolenia(id));



CREATE TABLE czlowiek (
  id SERIAL  NOT NULL ,
  stopnie_id INTEGER   NOT NULL ,
  oddzial_id INTEGER   NOT NULL ,
  pesel INTEGER   NOT NULL ,
  imie VARCHAR   NOT NULL ,
  nazwisko VARCHAR   NOT NULL ,
  m_zamieszkania VARCHAR   NOT NULL   ,
PRIMARY KEY(id),
  FOREIGN KEY(oddzial_id)
    REFERENCES oddzial(id),
  FOREIGN KEY(stopnie_id)
    REFERENCES stopnie(id));



CREATE TABLE sprzet_uzytkowanie (
  sprzet_id INTEGER   NOT NULL ,
  oddzial_id INTEGER   NOT NULL   ,
PRIMARY KEY(sprzet_id, oddzial_id),
  FOREIGN KEY(sprzet_id)
    REFERENCES sprzet(id),
  FOREIGN KEY(oddzial_id)
    REFERENCES oddzial(id));



CREATE TABLE sprzet_uprawnienia (
  uprawnienia_id INTEGER   NOT NULL ,
  sprzet_id INTEGER   NOT NULL   ,
PRIMARY KEY(uprawnienia_id, sprzet_id),
  FOREIGN KEY(uprawnienia_id)
    REFERENCES uprawnienia(id),
  FOREIGN KEY(sprzet_id)
    REFERENCES sprzet(id));



CREATE TABLE rozkazy (
  id SERIAL  NOT NULL ,
  nadawca_id INTEGER   NOT NULL ,
  opis VARCHAR      ,
UNIQUE (nadawca_id, opis),
PRIMARY KEY(id),
  FOREIGN KEY(nadawca_id)
    REFERENCES czlowiek(id));



CREATE TABLE czlowiek_szkolenia (
  czlowiek_id INTEGER   NOT NULL ,
  szkolenia_id INTEGER   NOT NULL   ,
PRIMARY KEY(czlowiek_id, szkolenia_id),
  FOREIGN KEY(czlowiek_id)
    REFERENCES czlowiek(id),
  FOREIGN KEY(szkolenia_id)
    REFERENCES szkolenia(id));



CREATE TABLE rozkaz_wykonawca (
  czlowiek_id INTEGER   NOT NULL ,
  rozkazy_id INTEGER   NOT NULL   ,
PRIMARY KEY(czlowiek_id, rozkazy_id),
  FOREIGN KEY(czlowiek_id)
    REFERENCES czlowiek(id),
  FOREIGN KEY(rozkazy_id)
    REFERENCES rozkazy(id));



create view czlowiek_uprawnienia_view as
	select c.id as "czlowiek_id", u.id as "uprawnienia_id" from czlowiek as c 
		join czlowiek_szkolenia as c_s on c.id = c_s.czlowiek_id
		join szkolenia as s on c_s.szkolenia_id = s.id
		join uprawnienia_szkolenia as u_s on s.id = u_s.szkolenia_id
		join uprawnienia as u on u.id = u_s.uprawnienia_id;

-- create view sprzet_uprawnienia_view as 
	-- select s.id as "sprzet_id", u.id as "uprawnienia_id" from sprzet as s
		-- join sprzet_uprawnienia as s_u on s_u.sprzet_id = s.id
		-- join uprawnienia as u on u.id = s_u.uprawnienia_id;
		
create view rozkaz_wykonawcy_view as 
	select r.id as "rozkaz_id", w.id as "wykonawca_id" from rozkazy as r 
		join rozkaz_wykonawca as r_w on r.id = r_w.rozkazy_id
		join czlowiek as w on w.id = r_w.czlowiek_id;
		
--trigger sprawdza czy:
--oddzial nadzedny modyfikowanego oddzialu jest wyzej w hierarchi oddzialow niz oddzial modyfikowany
		
create function spr_poprawnosc_hierarchi_oddzialow() returns trigger as $spr_poprawnosc_hierarchi_oddzialow$
	declare 
		nowy_oddzial_hierarchia integer;
		oddzial_nadrzedny_hieratchia integer;
	begin 
		set search_path to wojsko;
		nowy_oddzial_hierarchia := (select t.miara from typ_oddzialu as t where t.id = new.typ_id);
		oddzial_nadrzedny_hieratchia := (select t.miara from (select * from oddzial where id = new.oddzial_nadrzedny_id) as n 
										join typ_oddzialu as t on t.id = n.typ_id);
		if 	oddzial_nadrzedny_hieratchia <= nowy_oddzial_hierarchia then
			raise exception 'Oddzial nadrzedny powinien byc wyzej w hierarchi wojskowej niz oddzial podlegly.\n 
			Odzial nadrzedny hierarchia: "%" , oddzial podlagly hierarchia: "%"',
			(select opis from typ_oddzialu where miara = oddzial_nadrzedny_hieratchia),
			(select opis from typ_oddzialu where miara = nowy_oddzial_hierarchia);
		end if;			
		return new;
	end;
	$spr_poprawnosc_hierarchi_oddzialow$ language 'plpgsql';
	
create trigger spr_poprawnosc_hierarchi_oddzialow before insert or update on oddzial
	for each row execute procedure spr_poprawnosc_hierarchi_oddzialow();
	

--trigger sprawdza czy:
--dowodca nalezy do oddzialu którym ma kierowac
--dowodca posiada wystarczajaco wysoki stopien wojskowy do kierowania oddzialem

create function spr_kompetencje_dowodcy() returns trigger as $spr_kompetencje_dowodcy$
	declare 
		kompetencje_dowodcy integer;
		oddzial_hierarchia integer;
		oddzial_dowodcy integer;
	begin 
		set search_path to wojsko;
		oddzial_hierarchia := (select t.miara from typ_oddzialu as t where t.id = new.typ_id);
		kompetencje_dowodcy := (select s.miara from (select * from czlowiek where id = new.dowaodca_id) as d 
										join stopnie as s on s.id = d.stopnie_id);
		if 	oddzial_hierarchia > kompetencje_dowodcy then
			raise exception 'Dowodca powinien posiadac wustarczjaco wysoki stopien wojskowy do kierowania oddzialem.
				Stopien dowodcy: "%" - miara: "%", hierarchia oddzialu: "%" - miara "%".',
				(select opis from stopnie where miara = kompetencje_dowodcy),
				kompetencje_dowodcy,
				(select opis from typ_oddzialu where miara = oddzial_hierarchia),
				oddzial_hierarchia;
		end if;		

		oddzial_dowodcy := (select o.id from (select * from czlowiek where id = new.dowaodca_id) as d 
										join oddzial as o on o.id = d.oddzial_id);
		if oddzial_dowodcy <> new.id then
			raise exception 'Dowodca musi nalezec do dowodzonego oddzialu. Oddzial dowodcy: "%" \n
				ten oddzial "%"',
				(select opis from oddzial where id = oddzial_dowodcy),
				(select opis from oddzial where id = new.id);
		end if;
		return new;
	end;
	$spr_kompetencje_dowodcy$ language 'plpgsql';
	
create trigger spr_kompetencje_dowodcy before insert or update on oddzial
	for each row execute procedure spr_kompetencje_dowodcy();
	

create function spr_czy_czlowiek_jest_dowodca_oddzialu(oddzial_id integer, czlowiek_id integer) returns boolean as 
	$spr_czy_czlowiek_jest_dowodca_oddzialu$
	declare
		dowaodca_id integer;
	begin
		set search_path to wojsko;
		dowaodca_id := (select o.dowaodca_id from oddzial as o where o.id = oddzial_id);
		return dowaodca_id is not null and dowaodca_id = czlowiek_id;
	end;
	$spr_czy_czlowiek_jest_dowodca_oddzialu$ language 'plpgsql';
	

create function spr_prawo_do_wydania_rozkazu() returns trigger as $spr_prawo_do_wydania_rozkazu$
	declare 
		oddzial_wykonawcy integer;
		id_nadawcy integer;
		dowodca_oddzialu_nadzednego integer;
	begin 
		set search_path to wojsko;
		oddzial_wykonawcy := (select oddzial_id from czlowiek as c where c.id = new.czlowiek_id);
		id_nadawcy := (select r.nadawca_id from (select * from rozkazy where id = new.rozkazy_id) as r);
								--join czlowiek as n on n.id = );
		-- RAISE INFO 'Oddzial wykonawcy %, id nadawcy %',oddzial_wykonawcy,id_nadawcy;
		-- RAISE INFO 'Wykonawca jest dowodca %',spr_czy_czlowiek_jest_dowodca_oddzialu(oddzial_wykonawcy, new.czlowiek_id);
		-- RAISE INFO 'Wykonawca podwladnym nadawcy %',spr_czy_czlowiek_jest_dowodca_oddzialu(oddzial_wykonawcy, id_nadawcy);
		
		
		
		--jezeli wykonawca jest dowodca oddzialu to prawo do wydawania mu rozkazow ma dowodca oddzialu
		--nadzednego
		if 	spr_czy_czlowiek_jest_dowodca_oddzialu(oddzial_wykonawcy, new.czlowiek_id) then
			dowodca_oddzialu_nadzednego := (select dowaodca_id from oddzial where id = 
												(select oddzial_nadrzedny_id from oddzial where id = oddzial_wykonawcy));
			--RAISE INFO 'dowodca oddzialu nadzednego %',dowodca_oddzialu_nadzednego;
			if dowodca_oddzialu_nadzednego is null or id_nadawcy <> dowodca_oddzialu_nadzednego then
				raise exception '"%" "%" nie ma praw do wydawania rozkazow dla "%" "%".',
					(select imie from czlowiek where id = id_nadawcy),
					(select nazwisko from czlowiek where id = id_nadawcy),
					(select imie from czlowiek where id = new.czlowiek_id),
					(select nazwisko from czlowiek where id = new.czlowiek_id);
			end if;
		--jezeli wykonawca nie jest dowodca zadnego oddzialu, to rozkazy moze wydawac mu tylko dowodca jego oddzilu				
		else				
			if not spr_czy_czlowiek_jest_dowodca_oddzialu(oddzial_wykonawcy, id_nadawcy) then
				raise exception '"%" "%" nie ma praw do wydawania rozkazow dla "%" "%".',
					(select imie from czlowiek where id = id_nadawcy),
					(select nazwisko from czlowiek where id = id_nadawcy),
					(select imie from czlowiek where id = new.czlowiek_id),
					(select nazwisko from czlowiek where id = new.czlowiek_id);
			end if;
		end if;
		return new;
	end;
	$spr_prawo_do_wydania_rozkazu$ language 'plpgsql';
	
create trigger spr_prawo_do_wydania_rozkazu before insert or update on rozkaz_wykonawca
	for each row execute procedure spr_prawo_do_wydania_rozkazu();



--trigger sprawdza czy:
-- w oddziale do kturego dadawany jest sprzet jest conajmniej 
-- jedna osoba majaca uprawnienia do obslugi tego sprzetu


create function spr_uprawnienia_oddzialu_do_uzytkowania_sprzetu() returns trigger as $spr_uprawnienia_oddzialu_do_uzytkowania_sprzetu$
	
	declare 
		--uprawnienia ktorych wymaga dany sprzet
		sprzet_uprawnienia_cursor cursor(sprzet_id_ integer) 
			for select s_u.uprawnienia_id from wojsko.sprzet_uprawnienia as s_u where s_u.sprzet_id = sprzet_id_;
			
		--uprawnienia czlonkow oddczialu
		oddzial_uprawnienia_cursor cursor(oddzial_id_ integer) 
			for select c_u.uprawnienia_id from (select * from wojsko.czlowiek as c where c.oddzial_id = oddzial_id_) as czlonek
				join czlowiek_uprawnienia_view as c_u on czlonek.id = c_u.czlowiek_id;
		
		flaga_znaleziono_uprawnionego boolean;
		s_uprawnianie integer;
		c_uprawnienie integer;
	begin 
		set search_path to wojsko;
		open sprzet_uprawnienia_cursor(new.sprzet_id);
		loop
			fetch from sprzet_uprawnienia_cursor into s_uprawnianie;
			exit when not found;
			flaga_znaleziono_uprawnionego := false;
			open oddzial_uprawnienia_cursor(new.oddzial_id);
			loop 
				fetch from oddzial_uprawnienia_cursor into c_uprawnienie;
				exit when not found;
				if c_uprawnienie = s_uprawnianie then
					flaga_znaleziono_uprawnionego := true;
				end if;
			end loop;
			close oddzial_uprawnienia_cursor;
				
			if not flaga_znaleziono_uprawnionego then
				raise exception '\n 
				W oddziale "%" nie znaleziono kompetentnej osoby do obslugi sprzetu: "%",\n
				nie można przydzielic sprzetu temu oddzialowi.\n
				Trzeba najpierw przeszkolic conajmniej jednego zolnierza.',
				(select opis from oddzial where id = new.oddzial_id),
				(select opis from sprzet where id = new.sprzet_id);
			end if;
			
		end loop;
		close sprzet_uprawnienia_cursor;

		return new;
	end;
	$spr_uprawnienia_oddzialu_do_uzytkowania_sprzetu$ language 'plpgsql';
	
create trigger spr_uprawnienia_oddzialu_do_uzytkowania_sprzetu before insert or update on sprzet_uzytkowanie
	for each row execute procedure spr_uprawnienia_oddzialu_do_uzytkowania_sprzetu();
	
	

insert into typ_oddzialu (id ,miara, opis) values 
	(1, 2, 'pododdzial'), 
	(2, 3, 'oddzial'),
	(3, 5, 'zwiazek taktyczny'), 
	(4, 7, 'zwiazek operacyjny');
	
insert into stopnie (id, miara, opis) values 
	(1, 1, 'szeregowy'), 
	(2, 2, 'podoficer mlodszy'),
	(3, 3, 'podoficer'), 
	(4, 4, 'podoficer starszy'),
	(5, 5, 'oficer mlodszy'), 
	(6, 6, 'oficer starszy'),
	(7, 7, 'general');

 insert into oddzial (id, typ_id, opis) values 
	(1, 3, 'brygada zmechnizowna'); 

 insert into oddzial (id, oddzial_nadrzedny_id, typ_id, opis) values 
	(2,1,2 , 'wzmocniony batalion pancerny 1'),
	(3,2,1 , 'zaloga czolgu 1'); 

	
	
 insert into czlowiek (id, oddzial_id, stopnie_id, imie, nazwisko, pesel, m_zamieszkania) values 
 	(1,3,2 , 'Roman', 'Klusek' , 1, 'Wroclaw'), 
	(2,2,3 , 'Adam', 'Kopyto', 2, 'Wroclaw'), 
	(3,1,5 , 'Andrzej', 'Pila', 3, 'Wroclaw'),
	(4,3,1 , 'Jerzy', 'Kania', 4, 'Wroclaw');
	
update oddzial set dowaodca_id = 3 where id = 1;
update oddzial set dowaodca_id = 2 where id = 2;
update oddzial set dowaodca_id = 1 where id = 3;

	
insert into rozkazy (id, nadawca_id, opis ) values (1,1, 'Wykonac manewr zawracania');
insert into rozkaz_wykonawca (czlowiek_id, rozkazy_id) values (4,1);
	
insert into sprzet (id, opis) values (1, 'T34');
insert into uprawnienia (id, opis) values (1, 'kierowanie czolgiem');
insert into sprzet_uprawnienia (uprawnienia_id, sprzet_id) values (1,1);

insert into szkolenia (id, opis) values (1, 'Nauka jazdy czolgiem');
insert into uprawnienia_szkolenia (uprawnienia_id, szkolenia_id) values (1,1);

insert into czlowiek_szkolenia (czlowiek_id, szkolenia_id) values (1,1),(4,1);

insert into sprzet_uzytkowanie (sprzet_id, oddzial_id) values (1,3);






--TRIGGER TEST	
update oddzial set oddzial_nadrzedny_id = 3 where id = 1;

--TRIGGER TEST
update oddzial set dowaodca_id = 1 where id = 1;

insert into czlowiek (id, oddzial_id, stopnie_id, imie, nazwisko, pesel, m_zamieszkania) values 
 	(5,3,7 , 'Bogdan', 'Japa' , 5, 'Wroclaw'); 
	
update oddzial set dowaodca_id = 5 where id = 1;



--TRIGGER TEST
insert into rozkazy (id, nadawca_id, opis ) values (2,2, 'Odmaszerowac');
insert into rozkaz_wykonawca (czlowiek_id, rozkazy_id) values (3,2);


--TRIGGER TEST
insert into sprzet_uzytkowanie (sprzet_id, oddzial_id) values (1,2);
insert into czlowiek_szkolenia (szkolenia_id, czlowiek_id) values (1,2);
insert into sprzet_uzytkowanie (sprzet_id, oddzial_id) values (1,2);

var xmlHttp;

function getRequestObject() {
	if (window.ActiveXObject) {
		return (new ActiveXObject("Microsoft.XMLHTTP"));
	} else if (window.XMLHttpRequest) {
		return (new XMLHttpRequest());
	} else {
		return (null);
	}
}

function postAndRedirect(url, postData)
{
    var postFormStr = "<form method='POST' action='" + url + "'>\n";

    for (var key in postData)
    {
        if (postData.hasOwnProperty(key))
        {
            postFormStr += "<input type='hidden' name='" + key + "' value='" + postData[key] + "'></input>";
        }
    }

    postFormStr += "</form>";

    var formElement = $(postFormStr);

    $('body').append(formElement);
    $(formElement).submit();
}

function listujUrzadzenia() {
	xmlHttp = getRequestObject();
	if (xmlHttp) {
		try {
			console.log("wywolanie listuj urzadzenia");
			let url = "kontroler.php?sub=uzytkownicy&action=listuj_urzadzenia&standalone=0";
			xmlHttp.onload = handleResponse;
			xmlHttp.open("POST", url, true);
			xmlHttp.setRequestHeader("Content-Type", "text/plane");
			xmlHttp.send("");
		} catch (e) {
			alert("Nie mozna polaczyc sie z serwerem: " + e.toString());
		}

	} else {
		alert("Blad, AJAX nie dziala!");
	}
}

function handleResponse() {

	console.log("funkcja handle response");
	console.log(xmlHttp.responseText);
	console.log(JSON.parse(xmlHttp.responseText));
	let pobrane_dane = JSON.parse(xmlHttp.responseText);
	let dane_urzadzen;
	if (pobrane_dane[0]) {
		dane_urzadzen = pobrane_dane[1];

		var heading = new Array();
		heading[0] = "ID urzadzenie.";
		heading[1] = "ID uzytkownika";
		heading[2] = "opis urzadzenia";
		heading[3] = "szer. geo.";
		heading[4] = "dl. geo.";
		heading[5] = "listuj rekordy";
		heading[6] = "przegladaj wykresy";
		heading[7] = "usun urzadzenie";

		addTable('lista_urzadzen', heading, dane_urzadzen);

	} else {
		document.getElementById('lista_urzadzen').innerHTML = "Nie udalo sie poprawnie pobrac danych o urzadzeniach: "+pobrane_dane[1];
	}
}

function dodajTabele(node_name,heading, stock) {
	var myNode = document.getElementById(node_name);
	myNode.innerHTML = '';
	var table = document.createElement('TABLE');
	myNode.appendChild(table);
	var tableBody = document.createElement('TBODY');

	table.border = '1';
	table.appendChild(tableBody);

	//TABLE COLUMNS
	var tr = document.createElement('TR');
	tableBody.appendChild(tr);
	for (i = 0; i < heading.length; i++) {
		var th = document.createElement('TH');
		th.width = '75';
		th.appendChild(document.createTextNode(heading[i]));
		tr.appendChild(th);

	}

	//TABLE ROWS
	for (i = 0; i < Object.keys(stock).length; i++) {
		var tr = document.createElement('TR');
		//console.log("TR");
		for (let key in stock[i]) {
			var td = document.createElement('TD');
			td.appendChild(document.createTextNode(stock[i][key]));
			tr.appendChild(td);
			//console.log("TD: " + key + "   " + stock[i][key]);
		}
		tableBody.appendChild(tr);
	}
}

function addTable(nazwa_wezla, heading, stock) {
	console.log("funkcja add table");
	var myNode = document.getElementById(nazwa_wezla);
	var table = document.createElement('TABLE');
	myNode.appendChild(table);
	var tableBody = document.createElement('TBODY');

	table.border = '1';
	table.appendChild(tableBody);

	//TABLE COLUMNS
	var tr = document.createElement('TR');
	tableBody.appendChild(tr);
	for (i = 0; i < heading.length; i++) {
		var th = document.createElement('TH');
		th.width = '75';
		th.appendChild(document.createTextNode(heading[i]));
		tr.appendChild(th);

	}

	//TABLE ROWS
	for (i = 0; i < Object.keys(stock).length; i++) {
		var tr = document.createElement('TR');
		for (j = 0; j < Object.keys(stock[i]).length / 2; j++) {
			var td = document.createElement('TD');
			td.appendChild(document.createTextNode(stock[i][j]));
			tr.appendChild(td);
		}
		var td = document.createElement('TD');
		var input = document.createElement('input');
		input.setAttribute('onclick',"listuj_pomiary("+stock[i][0]+")");
		input.setAttribute('type',"BUTTON");
		input.setAttribute('value',"LISTUJ");
		input.setAttribute('style',"width:100%; height:100%" );
		td.appendChild(input);
		tr.appendChild(td);
		
		var td = document.createElement('TD');
		var input = document.createElement('input');
		input.setAttribute('onclick',"rysuj_wykresy("+stock[i][0]+")");
		input.setAttribute('type',"BUTTON");
		input.setAttribute('value',"RYSUJ");
		input.setAttribute('style',"width:100%; height:100%" );
		td.appendChild(input);
		tr.appendChild(td);
		
		var td = document.createElement('TD');
		var input = document.createElement('input');
		input.setAttribute('onclick',"usun_urzadzenie("+stock[i][0]+")");
		input.setAttribute('type',"BUTTON");
		input.setAttribute('value',"USUN");
		input.setAttribute('style',"width:100%; height:100%" );
		td.appendChild(input);
		tr.appendChild(td);
		
	
		tableBody.appendChild(tr);
	}
}

function listuj_pomiary(urzadzenie_id){
	//postAndRedirect("listuj_pomiary.php", {'urzadzenie_id': urzadzenie_id});
	
	var jqxhr = $.post("kontroler.php?sub=uzytkownicy&action=listuj_pomiary&standalone=0", {'urzadzenie_id': urzadzenie_id})
		.done(
			function(data) {
				console.log("DATA: "+data);
				data = JSON.parse(data);
				console.log(data[1] + " " + data[0]);
				if (data[0]) {
					
					let dane = [];
					for(let wiersz in data[1]){
						dane[wiersz] = {};
						dane[wiersz]["czas"]=new Date(data[1][wiersz]["czas"]/1000);
						dane[wiersz]["temperatura"]=data[1][wiersz]["temperatura"];
						dane[wiersz]["cisnienie"]=data[1][wiersz]["cisnienie"];
						dane[wiersz]["sila_wiatru"]=data[1][wiersz]["sila_wiatru"];
						dane[wiersz]["kierunek_wiatru"]=data[1][wiersz]["kierunek_wiatru"];
						//console.log(dane[wiersz]);
					}
					
					
					
					var node_name = 'wykres';
					var heading = new Array();
					heading[0] = "czas";
					heading[1] = "temperatura";
					heading[2] = "cisnienie";
					heading[3] = "sila_wiatru";
					heading[4] = "kierunek_wiatru";
					dodajTabele(node_name, heading, dane);					
				} else {
					alert("Nie jestes zalogowany, lub urzadzenie nie istnieje.");
				}
			}
		)
		.fail(
			function() {
				console.log('cos poszlo nie tak.');
			}
		);
	
	
	
}

function usun_urzadzenie(urzadzenie_id){
	postAndRedirect("kontroler.php?sub=uzytkownicy&action=usun_urzadzenie", {'urzadzenie_id': urzadzenie_id});
}

function rysuj_wykresy(urzadzenie_id) {
	let typ = document.getElementById('typ_wykresu').value;
	var jqxhr = $.post("kontroler.php?sub=uzytkownicy&action=listuj_pomiary&standalone=0", {'urzadzenie_id': urzadzenie_id})
		.done(
			function(data) {
				console.log("DATA: "+data);
				data = JSON.parse(data);
				console.log(data[1] + " " + data[0]);
				if (data[0]) {
					rysuj_wykres(data[1],typ);
				} else {
					alert("Nie jestes zalogowany, lub urzadzenie nie istnieje.");
				}
			}
		)
		.fail(
			function() {
				console.log('cos poszlo nie tak.');
			}
		);

}

function rysuj_wykres(data,typ){
	let punkty = [];
	for (let key in data){
		if(data[key][typ] != null){
			console.log(data[key]['czas'] + "    " + data[key][typ]);
			punkty.push({'x': new Date(parseInt(data[key]['czas'])), 'y': parseFloat(data[key][typ])});
		}
		
		
	}
	console.log(punkty);
	
	let jednostki = {
		"temperatura":"[C]",
		"cisnienie":"[hPa]",
		"sila_wiatru":"[m/s]",
		"kierunek_wiatru":"[stopnie]"
	};
	
	
	var chart = new CanvasJS.Chart("wykres", {
		title: {
			text: typ
		},
		 axisX:{
		  title : "czas"
		 },
		 axisY:{
		  title : typ+""+jednostki[typ]
		 },
				data: [{
				type: "line",
				dataPoints: punkty
			}
		]
	});
	chart.render();
}

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

function zaladujUzytkownikow(){
	var jqxhr = $.get("kontroler.php?sub=uzytkownicy&action=listuj_uzytkownikow&standalone=0")
		.done(
			function(data) {
				//console.log("DATA: "+data);
				data = JSON.parse(data);
				//console.log(data[1] + " " + data[0]);
				let dane = [];
				for(let wiersz in data[1]){
					dane[wiersz] = {};
					dane[wiersz]["imie"]=data[1][wiersz]["imie"];
					dane[wiersz]["nazwisko"]=data[1][wiersz]["nazwisko"];
					dane[wiersz]["email"]=data[1][wiersz]["email"];
					//console.log(dane[wiersz]);
				}
				
				if (data[0]) {
					var node_name = 'lista_uzytkownikow';
					var heading = new Array();
					heading[0] = "Imie";
					heading[1] = "Nazwisko";
					heading[2] = "email";
					dodajTabele(node_name, heading, dane);
				} else {
					alert("cos poszlo nie tak.");
				}
			}
		)
		.fail(
			function() {
				console.log('cos poszlo nie tak.');
			}
		);

}

function handleResponse(){
	document.getElementById('lista_uzytkownikow').innerHTML = xmlHttp.responseText;
	console.log(xmlHttp.responseText);
}
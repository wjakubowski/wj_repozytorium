var urzadzenie_id;


window.indexedDB = window.indexedDB || window.mozIndexedDB || window.webkitIndexedDB || window.msIndexedDB;

//prefixes of window.IDB objects
window.IDBTransaction = window.IDBTransaction || window.webkitIDBTransaction || window.msIDBTransaction;
window.IDBKeyRange = window.IDBKeyRange || window.webkitIDBKeyRange || window.msIDBKeyRange

if (!window.indexedDB) {
	window.alert("Your browser doesn't support a stable version of IndexedDB.")
}

var db;
var request = window.indexedDB.open("pomiary_db", 1);

window.onload = function(){
	request.onerror = function(event) {
		console.log("error: ");
	};

	request.onsuccess = function(event) {
		db = request.result;
		console.log("Otwarto polaczenie z lokalna baza danych: " + db);
		uaktualnijTabliceBufora();
	};

	request.onupgradeneeded = function(event) {
		var db = event.target.result;
		var objectStore = db.createObjectStore("pomiary", {
			keyPath: "czas"
		});
	}
}


function remove(id) {
	var request = db.transaction(["pomiary"], "readwrite")
		.objectStore("pomiary")
		.delete(id);

	request.onsuccess = function(event) {
		console.log("usunieto rekord w buforze, ID: " + event);
	};
}

function add(rekord){
	let objectStore = db.transaction(["pomiary"], "readwrite").objectStore("pomiary");
	objectStore.add(rekord);

	request.onsuccess = function(event) {
		console.log("Dodano rekord do lokalnej bazy, ID: "+rekord['czas']);
	};

	request.onerror = function(event) {
		console.log("Nie udalo sie dodac rekordu do lokalnej bazy.");
	}
}


function sczytajBuforDlaFunkcji(callback) {
	var dane = [];
	var objectStore = db.transaction("pomiary").objectStore("pomiary");

	objectStore.openCursor().onsuccess = function(event) {
		var cursor = event.target.result;

		if (cursor) {
			let rekord = {
				'czas': cursor.key,
				'temperatura': cursor.value.temperatura,
				'cisnienie': cursor.value.cisnienie,
				'sila_wiatru': cursor.value.sila_wiatru,
				'kierunek_wiatru': cursor.value.kierunek_wiatru
			};
			dane.push(rekord);
			//console.log(dane);
			cursor.continue();
		} else {
			//console.log("dane" + dane);
			callback(dane);
		}

	}

}

function uaktualnijTabliceBufora(){
	
	sczytajBuforDlaFunkcji(przygotujDaneDlaTabeli);
	
}

function przygotujDaneDlaTabeli(dane){
	var node_name = 'bufor';
	var heading = new Array();
	heading[0] = "czas";
	heading[1] = "temperatura";
	heading[2] = "cisnienie";
	heading[3] = "sila_wiatru";
	heading[4] = "kierunek_wiatru";
	dodajTabele(node_name, heading, dane);
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



function zapiszRekordDoBufora() {
	var d = new Date();
	let czas = d.getTime();
	let temperatura = document.getElementById("temperatura").value;
	let cisnienie = document.getElementById("cisnienie").value;
	let sila_wiatru = document.getElementById("sila_wiatru").value;
	let kierunek_wiatru = document.getElementById("kierunek_wiatru").value;
	let pomiar = {
		'czas': czas,
		'temperatura': temperatura,
		'cisnienie': cisnienie,
		'sila_wiatru': sila_wiatru,
		'kierunek_wiatru': kierunek_wiatru
	};
	add(pomiar);
	

	uaktualnijTabliceBufora();
}

function wyczyscBufor() {
	var objectStore = db.transaction("pomiary").objectStore("pomiary");

	objectStore.openCursor().onsuccess = function(event) {
		var cursor = event.target.result;

		if (cursor) {
			console.log("KEY: " + cursor.key);
			remove(cursor.key);
			cursor.continue();
		} else {
			uaktualnijTabliceBufora();
			console.log("No more entries!");
		}
	};
	
}

function synchronizujDane() {
	if (!navigator.onLine) {
		alert("Nie jesteœ online, dane nie mog¹ byæ zsynchronizowane.");
	} else {
		console.log("Synchronizuje dane.");
		var objectStore = db.transaction("pomiary").objectStore("pomiary");

		objectStore.openCursor().onsuccess = function(event) {
			var cursor = event.target.result;

			if (cursor) {
				console.log("pomiar:");
				for (let key in cursor.value)
					console.log(key+"  "+cursor.value[key]);
				var jqxhr = $.post("kontroler.php?sub=urzadzenia&action=dodaj_pomiar&standalone=0",cursor.value)
					.done(
						function(data) {
							data = JSON.parse(data);
							console.log(data[1]+" "+data[0]);
							if(data[0])
							{
								remove(cursor.key);
							}
							else{
								alert("Nie jestes zalogowany, lub dane sa nie poprawne.");
							}
						}
					)
					.fail(
						function() {
							console.log('nie zsynchronizowano pomiaru rekordu');
							break;
						}
					);
				cursor.continue();
			} else {
				wyczyscBufor();
				uaktualnijTabliceBufora();
				console.log("No more entries!");
			}
		};

	}
}
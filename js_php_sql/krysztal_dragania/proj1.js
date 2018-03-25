//PARAMETRY DOMYSLNE
var at_odl;
var at_r,at_r2;
var xy = [];
var vxy = [];
var obrazWysokosc;
var obrazSzerokosc;
var nx;
var ny;
var dt;
var klatka_co_ile;
var play = false;
let metoda_numeryczna;
let ust_poz_X, ust_poz_Y;

var worker;

function aktualizujParametryWznowSymulacje(){
	stopWorker()
	aktualizujParametry()
	inicjalizujSiec();
}

function aktualizujParametry(){
	//at_odl = document.getElementById("at_odl").value;
	at_r = document.getElementById("at_r").value;
	at_r2 = at_r * 1.3;
	/*x0 = at_odl/2;
	y0 = at_odl/2;
	obrazWysokosc;
	obrazSzerokosc;
	nx;
	ny;*/
	nx= document.getElementById("nxy").value;
	ny= document.getElementById("nxy").value;
	dt = document.getElementById("dt").value;
	metoda_numeryczna = document.getElementById("metoda_numeryczna").value;
	klatka_co_ile = document.getElementById("klatka_co_ile").value;
	var canvas = document.getElementById("obraz");
	obrazWysokosc = canvas.height;
	obrazSzerokosc = canvas.width;
	at_odl = obrazSzerokosc / nx;
	console.log("Parametry: " + at_odl +" "+ at_r +" "+ dt)
}

function aktualizujParametryKontynujSymulcaje(){

	//inicjalizujSiec();
}

function wlaczWylaczSymulacje(){
	play = !play;
	if(play){
		startWorker();
	}
	else{
		stopWorker();
	}
}

function sleep(milliseconds) {
  var start = new Date().getTime();
  for (var i = 0; i < 1e7; i++) {
    if ((new Date().getTime() - start) > milliseconds){
      break;
    }
  }
}

function inicjalizujSiec(){
	var canvas = document.getElementById("obraz");
	stopWorker();
	aktualizujParametry();
	//canvas.setAttribute("width", screen.width / 3);
	//canvas.setAttribute("height", screen.height / 3 );

	var ctx = canvas.getContext("2d");
	var x0 = at_odl/2;
	var y0 = at_odl/2;
	//nx = obrazSzerokosc / at_odl -1;
	//ny = obrazWysokosc / at_odl - 1;
	
	console.log("obraz rozmiary:" + obrazSzerokosc + " ; " + obrazWysokosc + " ; " + nx + " ; " + ny);
	xy = [];
	vxy = [];
	for (var ix = 0; ix <= nx; ++ix) {
		xy[ix] = [];
		vxy[ix] = [];
		for (var iy = 0; iy <= ny; ++iy) {
			xy[ix][iy] = [x0 + ix * at_odl, y0 + iy * at_odl];
			vxy[ix][iy] = [0, 0];
		}
	}
	
	xy[1][1][0] = xy[1][1][0] - 23; 
	//xy[4][3][1] = xy[4][3][1] - 30; 
	
	rysujSiec();
	ustawiajAtomyPozycja();
}

function rysujSiec(){
	var canvas = document.getElementById("obraz");
	var ctx = canvas.getContext("2d");
	ctx.beginPath();
	ctx.clearRect(0, 0, canvas.width, canvas.height);
	 
	for (var ix = 0; ix <= nx; ++ix) {
		for (var iy = 0; iy <= ny; ++iy) {

			ctx.fillStyle = "black";
			ctx.moveTo(xy[ix][iy][0], xy[ix][iy][1]);
			ctx.lineWidth = 10;
			ctx.arc(xy[ix][iy][0], xy[ix][iy][1], at_r2, 0, 2*Math.PI);		 
			ctx.fill();
			
			ctx.fillStyle = "red";
			ctx.moveTo(xy[ix][iy][0], xy[ix][iy][1]);
			ctx.lineWidth = 10;
			ctx.arc(xy[ix][iy][0], xy[ix][iy][1], at_r, 0, 2*Math.PI);
			ctx.fill();
			//ctx.fillStyle = "red";
			//ctx.moveTo(xy[ix][iy][0], xy[ix][iy][1]);
			//ctx.arc(xy[ix][iy][0], xy[ix][iy][1], at_r, 0, 2*Math.PI);		 
			//ctx.fill();
		}
	}
	console.log("poz: "+xy[1][1][0]+"   "+ xy[1][1][1])
}

function startWorker() {
	if(typeof(Worker) != "undefined") {
		if(typeof(worker) == "undefined") {
			worker = new Worker("symulacje.js");
			worker.postMessage({'xy' : xy,
									'vxy' : vxy,
									'at_odl' : at_odl,
									'nx' : nx,
									'ny' : ny,
									'dt' : dt,
									'klatka_co_ile' : klatka_co_ile,
									'metoda_numeryczna' : metoda_numeryczna});
		}
		worker.onmessage = function(event) {
			xy = event.data["xy"];
			vxy = event.data["vxy"];
			rysujSiec();
		};
	} else {
		//document.getElementById("result").innerHTML = "Sorry, your browser does not support Web Workers...";
		alert("Sorry, your browser does not support Web Workers...");
	}
}

function stopWorker() { 
	if(typeof(worker) != "undefined"){
		worker.terminate();
		worker = undefined;
	}
}


function ustawiajAtomyPozycja(){
	ust_poz_X = document.getElementById("ust_poz_X").value;
	ust_poz_Y = document.getElementById("ust_poz_Y").value;
	document.getElementById("nowe_pol_X").value = xy[ust_poz_X][ust_poz_Y][0];
	document.getElementById("nowe_pol_Y").value = xy[ust_poz_X][ust_poz_Y][1];
	let tekst = "["+xy[ust_poz_X][ust_poz_Y][0] + "," + xy[ust_poz_X][ust_poz_Y][1] + "]";
	document.getElementById("ust_polozenie").value = tekst;
	console.log(ust_poz_X + "  " + ust_poz_Y);
	rysujSiec();
	
	var canvas = document.getElementById("obraz");
	var ctx = canvas.getContext("2d");
	ctx.beginPath();
	ctx.fillStyle = "green";
	ctx.moveTo(xy[ust_poz_X][ust_poz_Y][0], xy[ust_poz_X][ust_poz_Y][1]);
	ctx.lineWidth = 10;
	ctx.arc(xy[ust_poz_X][ust_poz_Y][0], xy[ust_poz_X][ust_poz_Y][1], at_r, 0, 2*Math.PI);
	ctx.fill();
}

function ustawiajAtomyPolozenie(){
	xy[ust_poz_X][ust_poz_Y][0] = Number(document.getElementById("nowe_pol_X").value);
	xy[ust_poz_X][ust_poz_Y][1] = Number(document.getElementById("nowe_pol_Y").value);
	//console.log("now poz: "+xy[ust_poz_X][ust_poz_Y][0]+"   "+ xy[ust_poz_X][ust_poz_Y][1]+"   "+ (Number(xy[ust_poz_X][ust_poz_Y][0])+ Number(xy[ust_poz_X][ust_poz_Y][1])))
	
	rysujSiec();
	
	var canvas = document.getElementById("obraz");
	var ctx = canvas.getContext("2d");
	ctx.beginPath();
	ctx.fillStyle = "green";
	ctx.moveTo(xy[ust_poz_X][ust_poz_Y][0], xy[ust_poz_X][ust_poz_Y][1]);
	ctx.lineWidth = 10;
	ctx.arc(xy[ust_poz_X][ust_poz_Y][0], xy[ust_poz_X][ust_poz_Y][1], at_r, 0, 2*Math.PI);
	ctx.fill();
}

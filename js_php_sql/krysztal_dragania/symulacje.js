var s = [[1,0],[0,1],[-1,0],[0,-1]];

//PARAMETRY DOMYSLNE
var at_odl;
var xy;
var vxy;
var nx;
var ny;
var dt;
var play;
var ix,iy,is;
let del_x, del_y;
let c;
let i = 0;
let Fm,F,Fp;
let scianki = 1;

let metoda_numeryczna;



function deep_copy(obj){
	return JSON.parse(JSON.stringify(obj));
}

function liczPolozeniaAtomowEuler(){
	console.log("Worker dziala Euler: " + i);
	if (i==0){
		F=deep_copy(xy);
	}
	for (ix = scianki; ix <= nx-1-scianki; ++ix) {
		for (iy = scianki; iy <= ny-1-scianki; ++iy) {
			F[ix][iy][0] = 0.0;
			F[ix][iy][1] = 0.0;
			for (is = 0; is < s.length; ++is) {
				del_x = xy[ix][iy][0] - xy[ix+s[is][0]][iy+s[is][1]][0];
				del_y = xy[ix][iy][1] - xy[ix+s[is][0]][iy+s[is][1]][1];
				c = (at_odl / Math.sqrt(del_x*del_x + del_y*del_y) - 1.0);
				F[ix][iy][0] = del_x * c;
				F[ix][iy][1] = del_y * c;

			xy[ix][iy][0] += vxy[ix][iy][0] * dt;
			xy[ix][iy][1] += vxy[ix][iy][1] * dt;
			vxy[ix][iy][0] += F[ix][iy][0] * dt;
			vxy[ix][iy][1] += F[ix][iy][1] * dt;
			}
		}
	}
}



function liczPolozeniaAtomowVarlet(){
	console.log("Worker dziala Varlet: " + i);
	if (i==0){
		F=deep_copy(xy);
		Fp=deep_copy(xy);
	}
	for (ix = scianki; ix <= nx-1-scianki; ++ix) {
		for (iy = scianki; iy <= ny-1-scianki; ++iy) {
			//F == a(n)
			//obl sily dzialajace na atom od wszystkich 4 sasiadow "s"
			//del_x,del_y - odległosci miedzu sosiadujacymi atomami
			//Fx=del_x*(p0/del - 1); del = sqrt(del_x^2 + del_y^2);
			if(i==0){
				F[ix][iy][0] = 0.0;
				F[ix][iy][1] = 0.0;
				for (is = 0; is < s.length; ++is) {
					del_x = xy[ix][iy][0] - xy[ix+s[is][0]][iy+s[is][1]][0];
					del_y = xy[ix][iy][1] - xy[ix+s[is][0]][iy+s[is][1]][1];
					c = (at_odl / Math.sqrt(del_x*del_x + del_y*del_y) - 1.0);
					F[ix][iy][0] += del_x * c;
					F[ix][iy][1] += del_y * c;
				}
			}
			else{
				F = deep_copy(Fp);
			}
			//x(n+1) = x(n) + v(n)*h + 0.5*a(n)*h^2
			xy[ix][iy][0] += vxy[ix][iy][0] * dt + 0.5 * F[ix][iy][0] *dt*dt;
			xy[ix][iy][1] += vxy[ix][iy][1] * dt + 0.5 * F[ix][iy][1] *dt*dt;
			//Fp == a(n+1)
			Fp[ix][iy][0] = 0.0;
			Fp[ix][iy][1] = 0.0;
			for (is = 0; is < s.length; ++is) {
				del_x = xy[ix][iy][0] - xy[ix+s[is][0]][iy+s[is][1]][0];
				del_y = xy[ix][iy][1] - xy[ix+s[is][0]][iy+s[is][1]][1];
				c = (at_odl / Math.sqrt(del_x*del_x + del_y*del_y) - 1.0);
				Fp[ix][iy][0] += del_x * c;
				Fp[ix][iy][1] += del_y * c;
			}
			//v(n+1) = v(n) + 0.5*(a(n)+a(n+1))*h;
			vxy[ix][iy][0] += 0.5*(F[ix][iy][0] + Fp[ix][iy][0]) * dt;
			vxy[ix][iy][1] += 0.5*(F[ix][iy][1] + Fp[ix][iy][1]) * dt;
		}
	}
}

function liczPolozeniaAtomowBeemanSchofield(){
	if(i==0){
		liczPolozeniaAtomowVarlet();
		return;
	}
	console.log("Worker dziala BeemanSchofield: " + i);
	//Fm == a(n-1); F == a(n);
	Fm = deep_copy(F);
	F = deep_copy(Fp);
	for (ix = scianki; ix <= nx-1-scianki; ++ix) {
		for (iy = scianki; iy <= ny-1-scianki; ++iy) {
			//x(n+1) = x(n) + v(n)*h + h^2 / 6 *(4*a(n)-a(n-1))
			xy[ix][iy][0] += vxy[ix][iy][0] * dt + (4.0*F[ix][iy][0]-Fm[ix][iy][0]) *dt*dt/6.0;
			xy[ix][iy][1] += vxy[ix][iy][1] * dt + (4.0*F[ix][iy][1]-Fm[ix][iy][1]) *dt*dt/6.0;
			//Fp == a(n+1)
			Fp[ix][iy][0] = 0.0;
			Fp[ix][iy][1] = 0.0;
			for (is = 0; is < s.length; ++is) {
				del_x = xy[ix][iy][0] - xy[ix+s[is][0]][iy+s[is][1]][0];
				del_y = xy[ix][iy][1] - xy[ix+s[is][0]][iy+s[is][1]][1];
				
				c = (at_odl / Math.sqrt(del_x*del_x + del_y*del_y) - 1.0);
				Fp[ix][iy][0] += del_x * c;
				Fp[ix][iy][1] += del_y * c;
			}
			//v(n+1) = v(n) + (2*a(n+1)+5*a(n)-a(n-1))*h/6;
			vxy[ix][iy][0] += (2.0*Fp[ix][iy][0] + 5.0*F[ix][iy][0] - Fm[ix][iy][0])/6.0 * dt;
			vxy[ix][iy][1] += (2.0*Fp[ix][iy][1] + 5.0*F[ix][iy][1] - Fm[ix][iy][1])/6.0 * dt;
		}
	}
}

function liczPolozeniaAtomow(){
	onmessage = function(e) {
		at_odl = e.data['at_odl'];
		xy = e.data['xy'];
		vxy = e.data['vxy'];
		nx = e.data['nx'];
		ny = e.data['ny'];
		dt = e.data['dt'];
		klatka_co_ile = e.data['klatka_co_ile'];
		metoda_numeryczna = e.data['metoda_numeryczna'];
	};
	if (xy != undefined){
		
		switch(metoda_numeryczna){

			case "Varlet":
				liczPolozeniaAtomowVarlet();
				break;
			case "Euler":
				liczPolozeniaAtomowEuler();
				break;
			case "BeemanSchofield":
				liczPolozeniaAtomowBeemanSchofield();
				break;
			default:
				liczPolozeniaAtomowVarlet();
				break;
		}
		++i;
		if(!(i%klatka_co_ile)){
			postMessage({"xy":xy,"vxy":vxy});
		}
	}
	setTimeout("liczPolozeniaAtomow()",10);
}

liczPolozeniaAtomow();
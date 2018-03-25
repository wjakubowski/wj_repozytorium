window.onload = function(){
	let nazwa_tabeli = document.getElementById('nazwa_tabeli');
	let jqxhr = $.post("kontroler.php?sub=baza&action=table_names")
		.done(
			function(data) {
				data = JSON.parse(data);
				//console.log(data[0] + " " + data[1]);
				for(let t in data[1]){
					var node = document.createElement("option");
					node.setAttribute("id",data[1][t][0]);
					node.setAttribute("value",data[1][t][0]);
					node.appendChild(document.createTextNode(data[1][t][0]));
					nazwa_tabeli.appendChild(node);
				}
				update_forms();
				
			}
		)
		.fail(
			function() {
				console.log('cos poszlo nie tak.');
			}
		);
}

function typ_danych(typ_psql){
	if(typ_psql == "integer"){
		return "number";
	}
	else{
		return "text";
	}
}

function update_forms(){
	let method = document.getElementById('operacja').value;
	let prop = document.getElementById('prop');
	let cond = document.getElementById('cond');
	switch(method){
		case "insert":	
			prop.setAttribute("style","display: block;");
			cond.setAttribute("style","display: none;");
			break;
		case "delete":
		case "select":
			prop.setAttribute("style","display: none;");
			cond.setAttribute("style","display: block;");
			break;
		case "update":
			prop.setAttribute("style","display: block;");
			cond.setAttribute("style","display: block;");
			break;
	}
	let properties = document.getElementById('properties');
	properties.innerHTML = '';
	let conditions = document.getElementById('conditions');
	conditions.innerHTML = '';
	update_form(properties);
	update_form(document.getElementById('conditions'));
}

function update_form(form_node){
	let nazwa_tabeli = document.getElementById('nazwa_tabeli').value;
	//console.log("AAAAAAAAAAAAAAAAAAAAAAAAAA"+nazwa_tabeli);
	let jqxhr = $.post("kontroler.php?sub=baza&action=table_columns&relacja="+nazwa_tabeli)
		.done(
			function(data) {
				data = JSON.parse(data);
				//console.log(data[0] + " " + data[1]);
				for(let t in data[1]){
					form_node.appendChild(document.createTextNode(data[1][t][0]));
					let input = document.createElement("input");
					input.setAttribute("name",data[1][t][0]);
					input.setAttribute("class",form_node.getAttribute("id"));
					//console.log("XXXXXXXXXXXXXXX"+form_node.getAttribute("id"));
					input.setAttribute("type",typ_danych(data[1][t][1]));
					form_node.appendChild(input);
					form_node.appendChild(document.createElement("br"));
				}
			}
		)
		.fail(
			function() {
				console.log('cos poszlo nie tak.');
			}
		);
}

function submit_forms(){
	let properties_set = {};
	let conditions_set = {};
	let pola = document.getElementsByClassName('properties');
	for(let v in pola){
		if(pola[v].value)
			properties_set[pola[v].getAttribute("name")] = 
				pola[v].getAttribute('type') == "number" ? parseInt(pola[v].value) : pola[v].value;
	}
	pola = document.getElementsByClassName('conditions');
	for(let v in pola){
		if(pola[v].value)
			conditions_set[pola[v].getAttribute("name")] = 
				pola[v].getAttribute('type') == "number" ? parseInt(pola[v].value) : pola[v].value;
	}
	
	let method = document.getElementById('operacja').value;
	let nazwa_tabeli = document.getElementById('nazwa_tabeli').value;
	switch(method){
		case "insert":
			dane={"properties_set":properties_set};
			send_and_catch_data(method,nazwa_tabeli,dane);
			break;
		case "delete":
		case "select":
			dane={"conditions_set":conditions_set};
			send_and_catch_data(method,nazwa_tabeli,dane);
			break;
		case "update":
			dane={"properties_set":properties_set,"conditions_set":conditions_set};
			send_and_catch_data(method,nazwa_tabeli,dane);
			break;
	}
}


function send_and_catch_data(operacja,nazwa_tabeli,dane){
	//console.log(dane);
	let jqxhr = $.post("kontroler.php?sub=baza_obsluga&action="+operacja+"&relacja="+nazwa_tabeli,dane)
		.done(
			function(data) {
				//console.log(data);
				data = JSON.parse(data);
				let info_z_bazy = document.getElementById('info_z_bazy');
				info_z_bazy.innerHTML = "";
				
				
				if (operacja == "select"){
					var heading = new Array();
					/*if(data[1].length){
						let keys = Object.keys(data[1][0]);
						let len = keys.length;
						for(let i = len/2;i<len;++i ){
							heading.push(keys[i]);
						}
					}*/
					//console.log("DANE Z SERWERA:");
					//console.log(data[1]);
					
					let jqxhr2 = $.post("kontroler.php?sub=baza&action=table_columns&relacja="+nazwa_tabeli)
						.done(
							function(data2) {
								data2 = JSON.parse(data2);
								//console.log(data[0] + " " + data[1]);
								for(let t in data2[1]){
									/*form_node.appendChild(document.createTextNode(data[1][t][0]));
									let input = document.createElement("input");
									input.setAttribute("name",data[1][t][0]);
									input.setAttribute("class",form_node.getAttribute("id"));
									//console.log("XXXXXXXXXXXXXXX"+form_node.getAttribute("id"));
									input.setAttribute("type",typ_danych(data[1][t][1]));
									form_node.appendChild(input);
									form_node.appendChild(document.createElement("br"));*/
									heading.push(data2[1][t][0]);
								}
								dodajTabele('info_z_bazy',heading,data[1]);
							}
						)
						.fail(
							function() {
								console.log('cos poszlo nie tak.');
							}
						);
					
					
				}
				else{
					let node = document.createTextNode(data)
					info_z_bazy.appendChild(node);
				}
				
				
				//data = JSON.parse(data);
				//console.log(data[0] + " " + data[1]);
				/*for(let t in data[1]){
					form_node.appendChild(document.createTextNode(data[1][t][0]));
					let input = document.createElement("input");
					input.setAttribute("name",data[1][t][0]);
					input.setAttribute("class",form_node.getAttribute("id"));
					console.log("XXXXXXXXXXXXXXX"+form_node.getAttribute("id"));
					input.setAttribute("type",typ_danych(data[1][t][1]));
					form_node.appendChild(input);
					form_node.appendChild(document.createElement("br"));
				}*/
			}
		)
		.fail(
			function(d) {
				console.log('cos poszlo nie tak.');
				console.log(d);
			}
		);
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

	/*console.log(typeof(stock));
	console.log(stock);
	console.log(stock[0]);
	console.log(Object.keys(stock[0]));
	console.log(stock.length);*/
	//TABLE ROWS
	for (let i=0;i<stock.length;++i) {
		var tr = document.createElement('TR');
		//console.log("TR");
		for (let j =0; j<Object.keys(stock[i]).length/2;++j) {
			var td = document.createElement('TD');
			td.appendChild(document.createTextNode(stock[i][j]));
			tr.appendChild(td);
			//console.log("TD: " + key + "   " + stock[i][key]);
		}
		tableBody.appendChild(tr);
	}
}
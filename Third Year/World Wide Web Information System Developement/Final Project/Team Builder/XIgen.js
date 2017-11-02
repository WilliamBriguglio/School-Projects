window.addEventListener("load", init);

var player = "";
var squad = ""; //global variable which hold the chosen team name
var playerArr = new Array( '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'); //array that holds the player's number that is occupying the corresponding postion, 0 indicates no player selected
var posNum = 0;		//see comment on line 114 and 167
var hasReset = 0; //global variable used to determine if the reset button is present
var hasSave = 0; //global variable used to detemine if the save button is present
var amLoading = 0; 				//used to keep track of what kind of response function HandleServerResponse should expect
var amSaving = 0;				//^^^^Same as above
var amLoadingPlayers = 0;		//^^^^
var chosenformation = 0;  //global variable which holds the chosen formation
var teamset = 0; //global variable which keeps  track of if a team has been selected or not

function init(){ //after page loads, intialize javascript elements
	svg = document.createElement("svg");
	svg.setAttribute("id", "field");
	document.getElementById("XIcontainer").appendChild(svg);
	add_team_choice();	//add the Arsenal or Juventus options to right side of screen
	add_formation_choice();	//adds the 4-4-2 or 4-3-3- options to left side of sscren
	var xmlHttp = createXmlHttpRequestObject(); //creates xmlHttp request object
}

function add_team_choice(){
	team = document.getElementById("selectorContainer"); //selected right-hand side container and add inner div to position elements in
	ITF = document.createElement("div");
	ITF.setAttribute("id", "interface");
	team.appendChild(ITF);
	
	ArseBut = document.createElement("img"); //creat button and set its attributes Arsenal option
	ArseBut.setAttribute("id", "ArseBut");
	ArseBut.setAttribute("src", "Arse.png");
	ArseBut.setAttribute("alt", "Arsenal Logo");
	ITF.appendChild(ArseBut); //add button to inner div
	
	JuveBut = document.createElement("img"); //create button and set attributes for Juventus option
	JuveBut.setAttribute("id", "JuveBut");
	JuveBut.setAttribute("src", "Juve.png");
	JuveBut.setAttribute("alt", "Juventus Logo");
	ITF.appendChild(JuveBut); //add button to inner div
	
	document.getElementById("ArseBut").addEventListener("click", arse_click); 
	document.getElementById("JuveBut").addEventListener("click", juve_click);
}

function add_formation_choice(){
	
	formation = document.createElement("span"); //selected left-hand side container and add inner span to position elements in
	formation.setAttribute("id", "formation");
	document.getElementById("XIcontainer").appendChild(formation);
	
	//the following three blocks of code create divs to put insides the inner span and sets their class to button and adds event listeners
	
	but1 = document.createElement("div");
	but1.setAttribute("class", "button");
	t1 = document.createTextNode("4-4-2");
	but1.appendChild(t1);                                  
	document.getElementById("formation").appendChild(but1);
	but1.addEventListener("click", but1_click); //triggers 4-4-2 choice
	
	but2 = document.createElement("div");
	but2.setAttribute("class", "button");
	t2 = document.createTextNode("4-3-3");
	but2.appendChild(t2);
	document.getElementById("formation").appendChild(but2);
	but2.addEventListener("click", but2_click); //trigger 4-3-3 choie
	
	load = document.createElement("div");
	load.setAttribute("class", "button");
	t2 = document.createTextNode("Load");
	load.appendChild(t2);
	document.getElementById("formation").appendChild(load);
	load.addEventListener("click", load_click); //triggers load account's saved formation, team, and player choice
}

function but1_click(){	//create a 4-4-2 formation
	XI = document.getElementById("XIcontainer").removeChild(formation);
	
	//the following block creates the 4-4-2 formation by first adding the attackig line, then midfield, then defensive line, and keeper who is on his own line
	
	chosenformation = 1; //global variable used by other function that need to know which formation is active
	make_line("2");
	make_line("4");
	make_line("4");
	make_line("1");
	add_reset(); //adds reset button to the pages footer element
	add_players_dropable();	//sets all the payers necessary event listeners
	
}

function but2_click(){ //create a 4-3-3 formation
	XI = document.getElementById("XIcontainer").removeChild(formation);

	//the following block creates the 4-4-2 formation by first adding the attackig line, then midfield, then defensive line, and keeper who is on his own line
	
	chosenformation = 2;
	make_line("3");
	make_line("3");
	make_line("4");
	make_line("1");
	add_reset();	//adds the reset button to the pages footer element
	add_players_dropable(); //sets all the payers necessary event listeners
	
}

function add_players_dropable(){
	players = document.getElementsByClassName("player");
	for (var i = 0; i < players.length; i++) {
		players[i].addEventListener("drop", player_drop);
		players[i].addEventListener("dragover", player_dragover);
	}
}

function make_line(x){ //creates a div element that is appended as a child into the left-hand side container 
	line = document.createElement("div");
	line.setAttribute("class", "line");
	
	for(i = 0; i < x; i++){//uses the argument x to deteminethe number span elements of class player to put inside each line
		player = document.createElement("span");
	    player.setAttribute("class", "player");
		player.setAttribute("id", "posNum"+posNum);
		line.appendChild(player);
		posNum++;//incremented so each player slot has id 'posNum[x]' where x is in the range [0-10]
	}
	
	document.getElementById("XIcontainer").appendChild(line);	
}

function add_reset(){
	footer = document.getElementById("footer");
	if(hasReset == 0){ //appends a reset button as a child into the pages footer only if the reset button isnt already there
		_reset = document.createElement("span");
		_reset.setAttribute("id", "reset");
		_reset.setAttribute("class", "footBut");
		t1 = document.createTextNode("Reset");
		_reset.appendChild(t1);
		_reset.addEventListener("click", reset_page);
		footer.appendChild(_reset);
		hasReset = 1; //indicates reset button is present
	}
}

function reset_page(){
	reset_team();
	reset_formation();
	
	var foot = document.getElementById("footer");
	if(hasSave){
		foot.removeChild(foot.childNodes[1]);//remoes save button
		hasSave = 0;//indicates absence of a save button
	}
	foot.removeChild(foot.childNodes[0]);//removes reset button
	hasReset = 0; //indicates the absence of a reset button
	for( i = 0; i < 11; i++){ 
			playerArr[i] = 0; //resets the pages player choice
	}
}

function reset_team(){
	selCon = document.getElementById("selectorContainer");
	FC = selCon.firstElementChild.id;
	if(FC == "teamSheet"){
		selCon.removeChild(selCon.childNodes[1]);//removes the inner div from the right hand side container
		add_team_choice();//adds the options to choose between Juventus and Arsenal
	}
	teamset = 0;
}

function reset_formation(){
	
	var XIcon = document.getElementById("XIcontainer");
	if(XIcon.childElementCount > 2){//removes the child that is after the svg image
		XIcon.removeChild(XIcon.childNodes[2]);
		XIcon.removeChild(XIcon.childNodes[2]);
		XIcon.removeChild(XIcon.childNodes[2]);
		XIcon.removeChild(XIcon.childNodes[2]);//repeated 4 times to remove all 4 lines
		posNum = 0;//resets the variable used by make line to create the id's for the player slots
		add_formation_choice();//adds the options to choose between 4-4-2, 4-3-3, and Load
	}
	
}

function arse_click(){
	document.getElementById("ArseBut").removeEventListener("click", arse_click); //removes button functionality
	document.getElementById("ArseBut").setAttribute('class','no-hover');//causse the Arsenal logo to not highlight when hovered over
	ITF = document.getElementById("interface");
	ITF.setAttribute("id", "teamSheet");//changes class, and therfore changes css rules
	ITF.style.backgroundColor = "#CC0000"; //set her rather than in css since the attributes differ depending on team choice
	ITF.style.borderColor = "#000066"; 		//^^^^
	ITF.removeChild(ITF.childNodes[1]); //remove Juventus button
	add_reset();	//adds the reset button to the pages footer element
	add_team_name(ITF, "Arsenal");	
	add_players(ITF, "Arsenal");	//loads players in table
	squad = "Arsenal";	//sets global team choice variable 
	teamset = 1;
}

function juve_click(){
	document.getElementById("JuveBut").removeEventListener("click", juve_click); //removes button functionality
	document.getElementById("JuveBut").setAttribute('class','no-hover'); //causes the Arsenal logo to not highlight when hovered over
	ITF = document.getElementById("interface");
	ITF.setAttribute("id", "teamSheet"); //changes class, and therfore changes css rules
	ITF.style.backgroundColor = "#FFFFFF"; //set her rather than in css since the attributes differ depending on team choice
	ITF.style.borderColor = "#FFCC00";		//^^^^
	ITF.removeChild(ITF.childNodes[0]); //remove Arsenal button
	add_reset();	//adds the reset button to the pages footer element
	add_team_name(ITF, "Juventus");
	add_players(ITF, "Juventus");	//loads players in table
	squad = "Juventus";	//sets global team choice variable
	teamset = 1;
}

function add_team_name(ITF, name){ //adds team name besides logo at top of player selection area
	span = document.createElement("span"); 
	t = document.createTextNode(name);
	span.appendChild(t);
	span.setAttribute("class", "teamName");
	if(name == "Arsenal"){
		span.style.color = "#FFFFFF";
	}	
	ITF.appendChild(span);
}	

function add_players(ITF, name){
	div = document.createElement("div");
	div.setAttribute("id", "players"); //contains the list of players
	
	b = document.createElement("b"); //to be overwritten in handleServerResponse
	
	t = document.createTextNode("Loading Player List...");
	
	b.appendChild(t);
	div.appendChild(b);
	ITF.appendChild(div);
	load_players(name);//loads the players into the list
}

function load_players(team){ //load list of players with this function
	amLoadingPlayers = 1;//set variable set so that handleServerResponse knows what to do with response from server
	if(xmlHttp.readyState==4 || xmlHttp.readyState==0){//server not busy
		xmlHttp.open("GET", "load_players.php?team="+team, true);
		xmlHttp.onreadystatechange = handleServerResponse();
		xmlHttp.send(null);
	}else{
		setTimeout('load_players()', 1000);//wait 1 second for server then call process again
	}
}

function handleServerResponse(){
	if(xmlHttp.readyState==4){//server ready
		if(xmlHttp.status==200){//reponse code 200 OK
			if(amLoadingPlayers == 1){
				document.getElementById("players").innerHTML = xmlHttp.responseText; //reponse will container HTML code for a table with all the players and their stats in it
			
				prs = document.getElementsByClassName("pr");	//returns array with the row elements from the table return in xmlHttp.reponseText
				for (var i = 0; i < prs.length; i++) {
					prs[i].addEventListener("dragstart", pr_dragstart);//makes the rows of the table dragable
				}
				amLoadingPlayers = 0;
			}else if(amSaving == 1){
				alert(xmlHttp.responseText);//pass server response to the user
				amSaving = 0;	
			}else if(amLoading == 1){
				if(!(xmlHttp.responseText)){
					alert("No saved line up!");
				}else{
					var args = xmlHttp.responseText.split(","); //tokenize server response with ',' as the delimeter
					load_squad(args);	//pass server's response tokens to function that will load the users saved squad
				}
				amLoading = 0;
			}
		}else{
			alert('Somethign went wrong');
		}
	}else{
		setTimeout('handleServerResponse()', 1000);//wait for server and try again
	}
}

function createXmlHttpRequestObject(){ //creates the appropriate object for use of AJAX depending on users browser
	if(window.ActiveXObject){//internet explorer
		try{
			xmlHttp = new ActiveXObject("Microsoft.XMLHTTP");
		}catch(e){
			xmlHttp = flase;
		}
	}else{ //not internet explorer
		try{
			xmlHttp = new XMLHttpRequest();
		}catch(e){
			xmlHttp = flase;
		}
	}
	
	if(!xmlHttp){
		alert("can't create XMLHttpRequest!");
	}else{
		return xmlHttp;
	}
}


function pr_dragstart(evt){
	evt.dataTransfer.setData("text/plain", evt.target.id); //target is the row of the table that was dragged, and its id would be equal to the player number in that row
}

function player_drop(evt){
	evt.preventDefault();
	var data = evt.dataTransfer.getData("text/plain"); //set player number

	playerpic = document.createElement("img");
	playerpic.setAttribute("src", squad+"_Players/"+data+".png");
	playerpic.setAttribute("class", "playerpic");
	playerpic.setAttribute("id", evt.target.id);
	playerpic.addEventListener("dragover", player_dragover);
	playerpic.addEventListener("drop", new_player_drop);
	
	evt.target.appendChild(playerpic); //append as child the player's picture 
	evt.target.style.background ="url(trans.png)"; //set the background of the player slot to a tranparent image

	playerArr[evt.target.id.substr(6)] = data; //use the player slots's id to determine where in the array to store the player's number
	add_save();// add a save button
}

function player_dragover(evt){
	evt.preventDefault(); //remove dfault behaviour for dragover
}


function new_player_drop(evt){ // changes the image that is displayed and updates the player array
	evt.preventDefault();
	var data = evt.dataTransfer.getData("text/plain");
	evt.target.setAttribute("src", squad+"_Players/"+data+".png");
	playerArr[evt.target.id.substr(6,1)] = data;
}

function add_save(){ //adds the save button and its functionaility to the page's footer element
		footer = document.getElementById("footer");
		if(hasSave == 0){ //if there is no save button already
			save = document.createElement("span");
			save.setAttribute("id", "save");
			save.setAttribute("class", "footBut");
			t1 = document.createTextNode("Save");
			save.appendChild(t1);
			save.addEventListener("click", save_click);
			footer.appendChild(save);
			hasSave = 1; //indicate the presence of the save button
		}
	
}

function save_click(evt){
	amSaving = 1; //set global variable set so that handleServerResponse knows what to do with response from server
	if(xmlHttp.readyState==4 || xmlHttp.readyState==0){//server not busy
		xmlHttp.open("GET", "save.php?team="+squad+"&players="+playerArr+"&formation="+chosenformation, true);
		xmlHttp.onreadystatechange = handleServerResponse();
		xmlHttp.send(null);
	}else{
		setTimeout('save_click()', 1000);//wait 1 second for server then call process again
	}
}

function load_click(evt){
	amLoading = 1; //set global variable set so that handleServerResponse knows what to do with response from server
	if(xmlHttp.readyState==4 || xmlHttp.readyState==0){//server not busy
		xmlHttp.open("GET", "load.php", true);
		xmlHttp.onreadystatechange = handleServerResponse();
		xmlHttp.send(null);
	}else{
		setTimeout('load_click()', 1000);//wait 1 second for server then call process again
	}
}

function load_squad(args){
		if(args[1] == '4-4-2'){ // set formation
			but1_click(); //trigger the 4-4-3 button press actions
		}else{
			but2_click(); //trigger the 4-3-3 button press actions
		}
		
		if(teamSet = 1){
			reset_team();
		}
		squad = args[0];
		if( squad == 'Arsenal' ){ //set team
			arse_click();	//trigger the arsenal button press actions
		}
		else{
			juve_click(); //trigger the Juventus button press actions
		}
		
		for(x = 0; x < 11; x++){
			elem = document.getElementById("posNum"+x);
			dbload(elem, args[x+2]); //pass the next player slot to be filled and the player number that correspondes to the player that will fill it to dbload()
		}
		
	
}

function dbload(elem, shirtNum){
	
	playerpic = document.createElement("img");
	playerpic.setAttribute("src", squad+"_Players/"+shirtNum+".png");
	playerpic.setAttribute("class", "playerpic");
	playerpic.setAttribute("id", elem.id);
	playerpic.addEventListener("dragover", player_dragover)
	playerpic.addEventListener("drop", new_player_drop)
	
	elem.appendChild(playerpic); //append as child the player's picture to the player slot 
	elem.style.background ="url(trans.png)"; //set the background of the player slot to a tranparent image

	playerArr[elem.id.substr(6)] = shirtNum; //use the player slots's id to determine where in the array to store the player's number
	add_save(); // add a save button
}














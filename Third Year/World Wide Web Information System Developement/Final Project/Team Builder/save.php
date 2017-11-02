<?php

//creates an sql staement and queries the database with it using the arguments passed to it from the javescript XIgen
//the statement is of the form:
//INSERT INTO 	SQUADS (Username, TeamName, Formation, Out0, Out1, Out2, Out3, Out4, Out5, Out6, Out7, Out8, Out9, GK) 
//						VALUES ('aUsername, Arsenal, 4-4-3, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);

$team = $_GET['team'];
$playerString = $_GET['players'];
$formation = $_GET['formation'];

session_start();
$mysqli = new mysqli('localhost', 'root', '','accounts');
$sql = "INSERT INTO SQUADS (Username, TeamName, Formation, Out0, Out1, Out2, Out3, Out4, Out5, Out6, Out7, Out8, Out9, GK)
					VALUES ('".$_SESSION['username']."', '".$team."', ";


if($formation == 1){
	$sql = $sql."'4-4-2' ";
}else{
	$sql = $sql."'4-4-2' ";
}

$player = strtok($playerString, ","); //the player array is passed to the php script as a comma separated string of numbers, as such it must be tokenized before it can be used in our sql statement
while($player !== false) {
	$sql = $sql.", ".$player;
	$player = strtok(",");
} 

$sql = $sql.");";

$sql2 = "SELECT * FROM SQUADS WHERE username='".$_SESSION['username']."';";
$result = $mysqli->query($sql2);

if($result->num_rows > 0){ //overwrites the last save if ther is one
	echo "Overwriting last save..."; 
	$delete = "DELETE FROM SQUADS WHERE username='".$_SESSION['username']."';";
	if ($mysqli->query($delete) === TRUE) {
		echo " Overwriten.";
	} else {
		echo " Error Overwriting!";
	}
}


if($mysqli->query($sql) === TRUE) {
    echo " Your line up has been saved.";
} else {
    echo " Something went wrong during saving!";
}

$mysqli->close();
?>
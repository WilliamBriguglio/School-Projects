<?php

//this program queries the database for the saved team information with the user's user name as the primary key
//It echoes the information for the javascript XIgen to use
//the info is of the form:
//	[team name],[formation],[playernum],[playernum],[playernum],[playernum],[playernum],[playernum],[playernum],[playernum],[playernum],[playernum]

session_start();
$mysqli = new mysqli('localhost', 'root', '','accounts');

$usr = $_SESSION['username'];

$sql = "SELECT * FROM SQUADS WHERE username='".$usr."';";
$result = $mysqli->query($sql);

if ($result->num_rows > 0) {
	$row = $result->fetch_assoc();
	
	echo $row["TeamName"]. "," . $row["Formation"];
	
	for($x = 0; $x < 10; $x++){
		echo "," . $row["Out".$x];
	}
	echo ",". $row['GK'];
	
} else {
    echo FALSE;
}


$mysqli->close();
?>
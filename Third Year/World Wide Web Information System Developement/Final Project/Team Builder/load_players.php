<?php
//this code simply echos the html code for a table that has a row for each player on a team and a column for each of the players fields

$team = $_GET['team'];

session_start();
$mysqli = new mysqli('localhost', 'root', '','accounts');

if($team == "Arsenal"){ //determine which team to load
	$sqlO = "SELECT * FROM ARSENAL_OUT";
	$sqlG = "SELECT * FROM ARSENAL_GK";	
}elseif($team == "Juventus"){ 
	$sqlO = "SELECT * FROM JUVENTUS_OUT";	
	$sqlG = "SELECT * FROM JUVENTUS_GK";
}

$result = $mysqli->query($sqlO); //fetch out field players

	
echo "<table class='pt'";
if($team == "Arsenal"){
	echo "id='at'>";
}
echo "
<thead>
<tr id='head'>
<th>NUM</th>
<th colspan='2'>NAME</th>
<th>POS</th>
<th>OVERALL</th>
<th>PAC</th>
<th>SHO</th>
<th>PAS</th>
<th>DRI</th>
<th>DEF</th>
<th>PHY</th>
</tr>
</thead>
<tbody>";
	while($row = $result->fetch_assoc()){
		echo "<tr class='pr' id='";
		echo $row['KITNUM'];
		echo "' draggable='true'>";
		echo "<td>".$row['KITNUM']."</td>";
		echo "<td>".$row['FNAME']."</td>";
		echo "<td>".$row['LNAME']."</td>";
		echo "<td>".$row['POS']."</td>";
		echo "<td>".$row['OVERALL']."</td>";
		echo "<td>".$row['PACE']."</td>";
		echo "<td>".$row['SHOOTING']."</td>";
		echo "<td>".$row['PASSING']."</td>";
		echo "<td>".$row['DRIBBLING']."</td>";
		echo "<td>".$row['DEFENDING']."</td>";
		echo "<td>".$row['PHYSICAL']."</td>";
		echo "</tr>";
	}
echo "</tbody></table>";

$result = $mysqli->query($sqlG); //fetch goal keepers;

echo "<table class='pt'";
if($team == "Arsenal"){
	echo "id='at'>";
}
echo "
<thead>
<tr id='head'>
<th>NUM</th>
<th colspan='2'>NAME</th>
<th>POS</th>
<th>OVERALL</th>
<th>PAC</th>
<th>SHO</th>
<th>PAS</th>
<th>DRI</th>
<th>DEF</th>
<th>PHY</th>
</tr>
</thead>
<tbody>";	
	while($row = $result->fetch_assoc()){
		echo "<tr class='pr' id='";
		echo $row['KITNUM'];
		echo "' draggable='true'>";
		echo "<td>".$row['KITNUM']."</td>";
		echo "<td>".$row['FNAME']."</td>";
		echo "<td>".$row['LNAME']."</td>";
		echo "<td>".$row['POS']."</td>";
		echo "<td>".$row['OVERALL']."</td>";
		echo "<td>".$row['DIVING']."</td>";
		echo "<td>".$row['HANDLING']."</td>";
		echo "<td>".$row['KICKING']."</td>";
		echo "<td>".$row['REFLEXES']."</td>";
		echo "<td>".$row['SPEED']."</td>";
		echo "<td>".$row['POSITIONING']."</td>";
		echo "</tr>";
	}
echo "</tbody></table>";
	
$mysqli->close();
?>


















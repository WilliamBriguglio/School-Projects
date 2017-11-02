<html>
	<head>
		<title>Squad Builder</title>
    	<link rel="stylesheet" type="text/css" href="home.css" />
	</head>

	<body>

		<div class = "banner">
			<img src="images/banner.png">
		</div>
	 	
	 	<form method="POST" action="admin.php" id ="admin_form" onsubmit="return LoginValidation()" >

	 		<h1>ADD PLAYER</h1>

	 		<span id="success">SUCCESS</span><br>
			
	 		<label>Team: </label>
	 		<select name="team">
			  <option value="arsenal">Arsenal</option>
			  <option value="juve">Juventus</option>
			</select> <br>

			<label>Position: </label>
			<select name="position">
			  <option value="FW">Forward</option>
			  <option value="GK">Goal Keeper</option>	
			  <option value="DF">Defensive Midfielder</option>
			  <option value="MF">Midfielder</option>
			</select> <br>

			<label>First name: </label>
			<input type="text" name="fname" maxlength="20" value="<?php if (isset($_POST['fname'])) { echo $_POST['fname']; } ?>"><br>

			<label>Last name: </label>
			<input type="text" name="lname" required maxlength="20" value="<?php if (isset($_POST['lname'])) { echo $_POST['lname']; } ?>"><br>

			<label>Kit Num: </label> 
			<input type="number" name="kitnum" max="9999999999" required value="<?php if (isset($_POST['kitnum'])) { echo $_POST['kitnum']; } ?>">
			<span id="kitnum_taken_error">Kit Number is already in use.</span><br>
			
			<label>Nationality: </label>
			<input type="text" name="nationality" maxlength="3" required value="<?php if (isset($_POST['nationality'])) { echo $_POST['nationality']; } ?>"><br>

			<label>Overall: </label>
			<input type="number" name="overall" max="9999999999" required value="<?php if (isset($_POST['overall'])) { echo $_POST['overall']; } ?>"><br>

			<label>Pace: </label>
			<input type="number" name="pace" max="9999999999" value="<?php if (isset($_POST['pace'])) { echo $_POST['pace']; } ?>"><br>

			<label>Shooting: </label>
			<input type="number" name="shooting" max="9999999999" value="<?php if (isset($_POST['shooting'])) { echo $_POST['shooting']; } ?>"><br>

			<label>Passing: </label>
			<input type="number" name="passing" max="9999999999" value="<?php if (isset($_POST['passing'])) { echo $_POST['passing']; } ?>"><br>

			<label>Dribbling: </label> 
			<input type="number" name="dribbling" max="9999999999" value="<?php if (isset($_POST['dribbling'])) { echo $_POST['dribbling']; } ?>" ><br>

			<label>Defending: </label>
			<input type="number" name="defending" max="9999999999" value="<?php if (isset($_POST['defending'])) { echo $_POST['defending']; } ?>" ><br>

			<label>Physical: </label>
			<input type="number" name="physical" max="9999999999" value="<?php if (isset($_POST['physical'])) { echo $_POST['physical']; } ?>" ><br>

			<button onclick="" type="submit">Enter player</button>

		</form>

		<button onclick="location.href='index.php'" type="submit">Log Out</button>
		<button onclick="location.href='mainpage.php'" type="submit">Squad Builder </button>
	</body>
</html>

<?php

	session_start();
	$mysqli = new mysqli('localhost', 'root', '', 'accounts');

	if ($_SERVER['REQUEST_METHOD'] == 'POST') {

		$team = $_POST['team'];
		$fname = $_POST['fname'];
		$lname = $_POST['lname'];
		$kitnum = $_POST['kitnum'];
		$position = $_POST['position'];
		$nationality = $_POST['nationality'];
		$overall = $_POST['overall'];
		$pace = $_POST['pace'];
		$shooting = $_POST['shooting'];
		$passing = $_POST['passing'];
		$dribbling = $_POST['dribbling'];
		$defending = $_POST['defending'];
		$physical = $_POST['physical'];

		$sql = "INSERT INTO ";

		if ($team == "arsenal"){
			$sql .= "ARSENAL_";
		} else {
			$sql .= "JUVENTUS_";
		}

		if ($position == "GK"){
			$sql .= "GK";
		} else {
			$sql .= "OUT";
		}

		$sql .= " VALUES (";
		$sql .= "'" . $fname . "'" . ','. "'" . $lname . "'" . ',' . $kitnum . ',' . "'" . $position . "'" . ',' .
		"'" . $nationality . "'" . ',' . $overall . ',' . $pace . ',' . $shooting . ',' . $passing . ','
		. $dribbling . ',' . $defending . ',' . $physical . ')'; 

		$sql = $sql;
		

		if ( $mysqli->query($sql) ){
			echo '<script type="text/javascript">' ,
					'document.getElementById("success").style.display="block";',
					'</script>';
		} else {
			echo '<script type="text/javascript">' ,
					'document.getElementById("kitnum_taken_error").style.display="block";',
					'</script>';
		}
	}

?>

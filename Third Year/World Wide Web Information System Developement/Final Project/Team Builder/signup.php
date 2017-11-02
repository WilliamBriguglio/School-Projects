
<html>

	<head>
		<title>Sign Up</title>
    	<link rel="stylesheet" type="text/css" href="home.css" />
	</head>

	<body>
		<div class = "banner">
			<img src="images/banner.png">
		</div>

		<div class="signup-form">
			<form method="POST" action="signup.php" onsubmit="return Validate()" >
	            <label> Username </label>
	            <input type="text" placeholder="Enter username" id="username" name="username" value="<?php if (isset($_POST['username'])) { echo $_POST['username']; } ?>" required>
	            <span id="taken_error"> ERROR: USERNAME ALREADY IN USE </span>
	            <br>

	        	<p style="font-size: 12px">*must be <= 10 characters</p>

	            <label> Password </label>
	            <input type="password" id="password" placeholder="Enter Password" name="password" required>
	            <br>
	            <p style="font-size: 12px">*must be <= 10 characters</p>


	            <label> Repeat Password </label>
	            <input type="password" id="password_repeat" placeholder="Repeat Password" name="password_repeat" required>


		    	<p id="name_error" >ERROR: USERNAME IS TOO LONG</p>
		    	<p id="password_length_error" >ERROR: PASSWORD IS TOO LONG</p>
		    	<p id="password_error" >ERROR: REPEAT PASSWORD IS NOT SAME AS PASSWORD</p>

		    	<div id="submit_button">
		    		<input type="submit" value="REGISTER"/>
		    	</div>
			</form>
		</div>

		<script type="text/javascript">
		/*
		This script adds an event listener to the submit button which checks that the username is <= 10 chars and that both password entries are the same before creating an account for the user */

			var submitButton = document.getElementById("submit_button");

			submitButton.addEventListener("click", Validate);

			function Validate(evt){
				var username = document.getElementById("username");
				var password = document.getElementById("password");
				var password_repeat = document.getElementById("password_repeat");

				//CHECK USERNAME
				if (username.value.length > 10){ 
					document.getElementById("name_error").style.display = "block";
					return false;
				} else {
					document.getElementById("name_error").style.display = "none";
				}
				
				//Check that password doesn't exceed length
				if (password.value.length > 10 ){
					document.getElementById("password_length_error").style.display = "block";
					return false;
				} else {
					document.getElementById("password_length_error").style.display = "none";

				}

				//CHECK PASSWORD IS THE SAME

				if (password.value != password_repeat.value){
					document.getElementById("password_error").style.display = "block";
					return false;
				} else {
					document.getElementById("password_error").style.display = "none";
				}
				
				return true;
			} 
		</script>

	</body>

</html>

<?php
	include 'php-pre55-password-hash-utils.php';
	session_start();
	$mysqli = new mysqli('localhost', 'root', '', 'accounts');


	if ($_SERVER['REQUEST_METHOD'] == 'POST') {
		
		if ($_POST['password'] == $_POST['password_repeat']){

			$username = $_POST['username'];
			$password = password_hash($_POST['password'], PASSWORD_DEFAULT);
			$_SESSION['username'] = $username;

			$sql = "INSERT INTO users (id, pass) VALUES ('$username','$password')";

			if ($mysqli->query($sql) === true){
				header("location: index.php");
			} else {
				echo "<script>document.getElementById('taken_error').style.display='block';</script>";
			}
		}
	}
?>

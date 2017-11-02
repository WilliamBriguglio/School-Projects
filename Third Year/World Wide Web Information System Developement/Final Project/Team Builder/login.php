
<html>
	<head>
		<title>Squad Builder</title>
    	<link rel="stylesheet" type="text/css" href="home.css" />
	</head>

	<body>

		<div class = "banner">
			<img src="images/banner.png">
		</div>

		 <div class="login">
		 	<form method="POST" action="index.php" onsubmit="return LoginValidation()" >

			 	<div style="margin-bottom: 15px">
				    <label><b>Username</b></label>
				    <input type="text" placeholder="Enter Username" name="username" id="username"required>
				   </div>

			    <br>
			    <div style="margin-bottom: 15px">
				    <label><b>Password</b></label>
				    <input type="password" placeholder="Enter Password" name="password" id="password" required>
				</div>

			    <br>
			    <p id="login_error" >ERROR: INCORRECT USERNAME OR PASSWORD</p>
			    <button id="loginButton" type="submit">LOG IN</button>
			    <button onclick="location.href='signup.php'" type="submit">Sign Up</button>
				
			</form>
		  </div>

		  <script type="text/javascript">

		  	var submitButton = document.getElementById("loginButton");
		  	submitButton.addEventListener(click, LoginValidation);

		  	function LoginValidation(){
		  		//check with database that the info is registered first
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

			$username = $_POST['username'];
			$password = $_POST['password'];
			$sql = "SELECT password FROM users WHERE users.id = '$username' LIMIT 1";
			
		    foreach ($mysqli->query($sql) as $row) {
		        $dbpassword = $row['password'];
		    }

			$sql = "SELECT 1 FROM users WHERE users.id = '$username' LIMIT 1";

			$_SESSION['username'] = $username;

			if ($mysqli->query($sql) && mysqli_num_rows($mysqli->query($sql))> 0){
				if (password_verify($password, $dbpassword)){
					$_SESSION['message'] = 'Login Succesful!';
					header("location: index.html");
				} else {
					echo '<script type="text/javascript">document.getElementById("login_error").style.display = "block";</script>';
				}
				
			} else {
				echo '<script type="text/javascript">document.getElementById("login_error").style.display = "block";</script>';
			}
		}
?>

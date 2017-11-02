<!DOCTYPE html>
<html>
  <head>
    <title>Squad Builder</title>
	<link rel="stylesheet" type="text/css" href="builder.css"/>
	<script src="XIgen.js" type="application/javascript"></script>
  </head>
  <body>
  <div class="container">
	<img id="logo" src="SBlogo.jpg" alt="S.B. Logo">
    <nav>
		<ul>
			<li ><a href="index.php">Log out</a></li>
			<li ><a href="admin.php" class="admin_privilege" style="display:none;">Admin</a></li>
			<li id="nav"><span style="color:#FFFFFF"> Hello, <span id="username_topbar"></span></span></li>
			<li id="nav-right"><a href="contactus.php">Contact Us</a></li>
		</ul>
	</nav>
	<div id="builder">
		<span id="XI">
			<div id="XIcontainer">
			</div>
		</span>
		<span id="selector">
			<div id="selectorContainer">
			</div>
		</span>
	</div>
	<div id="footer"></div>
  </div>
  </body>
</html>

<?php
	session_start();
	echo "<script>document.getElementById('username_topbar').textContent = '" , $_SESSION['username'] , "';</script>";
	if ($_SESSION['username'] == 'admin'){
		echo "<script> var elements = document.getElementsByClassName('admin_privilege');" ,
		"for (var i=0; i <elements.length; i++){" ,
			"elements[i].style.display='inline';" ,
		"} </script>";
	}



?>
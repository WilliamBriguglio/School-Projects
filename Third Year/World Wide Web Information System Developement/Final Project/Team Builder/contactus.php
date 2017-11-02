<?php
require_once('common.php');


html5_prolog(
	'Contact Us',
	array('builder.css'), //CSS
	array() //Javascript
	);

echo '<script src="http://cloud.tinymce.com/stable/tinymce.min.js?apiKey=qjbob82x0e4tpkthwoe0dk9dsuysew4vnzhc3ate7zz4eijc"></script>';
echo  "<script>tinymce.init({ selector:'textarea' });</script>";

//echo "<script src='tinymce/tinymce.min.js'></script>";
//echo "<script>;";
//echo "tinymce.init({selector:'textarea'})";
//echo "</script>";

html5_body_header();

html5_body_menu();


echo "<form action='mailto:iacobelt@uwindsor.ca' method='post' enctype='text/plain' id='contact'>";
	echo "Name:  ";
  echo "<input type='text' name='name'><br>";
  echo "Email:  ";
  echo "<input type='text' name='email'><br>";
  echo "<input type='submit' value='Send'>";
  echo "<input type='reset' value='Reset'>";
	echo "</form>";

  echo "Comments:  <br>";
  echo "<textarea name='comments' rows='10' cols='100' form='contact'></textarea><br>";

html5_body_footer();

html5_epilog();

//Delete if unusable
//echo "<script src='tinymce/tinymce.min.js'></script>";
	//echo "<script>;";
		//echo "tinymce.init({selector:'textarea'})";
	//echo "</script>";

?>




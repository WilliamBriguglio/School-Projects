<?php
require_once('config.php');

session_start();

// Add additional require_once's here or anywhere in this file.
require_once('login-tools.php');

//
//
//
function html5_prolog($title, $css_files = array(), $js_files = array())
{
  echo "<!DOCTYPE html>\n".
    "<html><head><title>".
    htmlspecialchars($title).
    "</title>"
  ;
  
  foreach ($css_files as $css_file)
    echo "<link rel='stylesheet' type='text/css' href='$css_file' />";
  
  foreach ($js_files as $js_file)
    echo "<script src='$js_file' type='application/javascript'></script>";

  
}

//
//
//
function html5_epilog()
{
  echo "</div></body></html>";
}

//
//
//
function html5_body_header()
{
	echo "</head><body>\n";
  echo '<div class="container">';
  echo '<img id="logo" src="SBlogo.jpg" alt="S.B. Logo" />';
}

//
//
//
function html5_body_menu()
{
 
  echo '<nav>';
  echo '<ul>';
  echo '<li><a href="/">Home</a></li>';
  echo '<li><a href="/">Team</a></li>';
  echo '<li><a href="/">Formation</a></li>';
  echo '<li id="nav-right"><a href="/">Contact Us</a></li>';
  echo '</ul>';
  echo '</nav>';
}

//
//
//
function html5_body_content($content)
{
  echo '<div id="content">';
  echo $content;
  echo '</div>';
}

function html5_body_XI()
{
	echo '<div id="builder">';
	echo '<span id="XI">';
	echo '<div id="XI Container">';
	echo '</div>';
	echo '</span>';
	echo '<span id="XI"></span>';
}
function html5_body_midsection($content)
{
  html5_body_menu();
  html5_body_content($content);
}

//
//
//
function html5_body_footer()
{
  echo '<div id="footer">Footer</div>';
}

//
//
//
function html5_page($title, $css=array(), $js=array(), $content="")
{
  html5_prolog($title, $css, $js);
  html5_body_header($title);
  html5_body_midsection($content);
  html5_body_footer();
  html5_epilog();
}

?>

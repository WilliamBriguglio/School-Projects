<?php

function generate_html5_login_form()
{
  return <<<ZZEOF
<div id='login'>
  <form action='check-login.php' method='POST'>
    Login: <input type='text' name='login' /><br />
    Password: <input type='password' name='passwd' /><br />
    <input type='submit' />
  </form>
</div>
ZZEOF;
}

function is_logged_in()
{
  if (array_key_exists('logged_in', $_SESSION))
    return $_SESSION['logged_in_user'];
  else
    return FALSE;
}

function log_in_user($user)
{
  $_SESSION['logged_in'] = 1;
  $_SESSION['logged_in_user'] = 'admin';
}

function log_out_user()
{
  unset($_SESSION['logged_in'], $_SESSION['logged_in_user']);
}


?>

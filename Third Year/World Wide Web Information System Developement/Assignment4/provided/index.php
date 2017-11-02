<?php ?>
<!DOCTYPE html>
<html>
  <head>
    <title>2017W 03-60-334 Assignment 4</title>
    <link rel="stylesheet" href="basic.css" type="text/css" />
  </head>
  <body>
    <table>
      <thead>
        <tr>
          <th>Count</th>
          <th>Greek Letter Image</th>
          <th>Arabic Number Image</th>
        </tr>
      </thead>
      <tbody>
<?php
  for ($i=0; $i <= 25; $i++)
  {
?>
        <tr>
          <td><?php echo $i; ?></td>
          <td><img src="pic.php?i=<?php echo $i; ?>&purpose=g" alt="a Greek letter" /></td>
          <td><img src="pic.php?i=<?php echo $i; ?>&purpose=a" alt="an Arabic number" /></td>
        </tr>
<?php
  }
?>
        <tr>
          <td>BAD</td>
          <td><img src="pic.php?i=1&purpos=g" alt="bad image" /></td>
          <td><img src="pic.php?i=1&purpose=a&w=2" alt="bad image" /></td>
        </tr>
      </tbody>
    </table>
  </body>
</html>

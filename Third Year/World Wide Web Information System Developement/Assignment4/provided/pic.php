<?php

$IMAGE_DIR_PREFIX = 'images/';

require_once($IMAGE_DIR_PREFIX.'globals.php');

if ((array_key_exists('i', $_GET)) && (array_key_exists('purpose', $_GET)) && (count($_GET) == 2)){
  
  error_log("test1");
  
  $i = trim($_GET['i']);
  $purpose = trim($_GET['purpose']);
  if (is_numeric($i) && in_array($purpose, array("g", "a"))){
	$IMAGE = null;
	$IMAGE_MIME = null;
	$i = intval($i);

	if( $purpose == 'a' ){
		if(array_key_exists($i, $NUMBERS)){
			$IMAGE = $IMAGE_DIR_PREFIX.'/'.$NUMBERS[$i];
		}
	}else if( $purpose == 'g' ){
		if(array_key_exists($i, $LETTERS)){
			$IMAGE = $IMAGE_DIR_PREFIX.'/'.$LETTERS[$i];
		}
	}

	if($IMAGE != NULL){
		$fi = new finfo(FILEINFO_MIME);
		$result = $fi->file($IMAGE); // Returns MIME type unless an error occurred
		if($result){
			$IMAGE_MIME = $result;
		}
	}
	
	
	error_log($IMAGE);
	error_log($IMAGE_MIME);
	
	if(($IMAGE != null) && ($IMAGE_MIME != null)){
		header('Content-Type: '.$IMAGE_MIME);
		@readfile($IMAGE);
		exit;
	}else{
		error_log("test2");
		error();
		}		
	}else{
		error_log("test3");
		error();
		
	}
}else{
	error_log("test4");
	error();
}

function error(){
	http_response_code(404);
	echo <<<ZZEOF
		<!DOCTYPE html>
		<html>
		  <head><title>2017W 03-60-334 Assignment 2: 404 File Not Found</title></head>
		  <body><h1>404 File Not Found</h1><p>The requested resource could not be found.</p></body>
		</html>
ZZEOF;
	exit(0);
}	
?>
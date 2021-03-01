<?php
include('databaseinfo.php');
$ip = $_SERVER['REMOTE_ADDR'];
$unitid  = $_GET['unitid'];
$temp  = $_GET['temp'];
$ph  = $_GET['ph'];
$do  = $_GET['do'];

$thetimestamp=strtotime("now");
$posteddate=date("Y-m-d");
date_default_timezone_set('Jamaica');
$dat1=date('h:');
$dat=date('i:s A');
$dat2=$dat1+1;
$otherdate=$dat2.":". $dat;
$registered=$posteddate ." ".$dat2.":". $dat;

mysqli_query($con,"INSERT INTO waterquality VALUES (NULL,'$unitid','$temp','$ph','$do','$ip', '$thetimestamp', '$registered','$posteddate','$otherdate')" );
?>

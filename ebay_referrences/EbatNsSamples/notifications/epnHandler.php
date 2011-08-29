<?php
	require_once '../EbatNs/EbatNs_NotificationClient.php';
	
	$handler = new EbatNs_NotificationClient();
	$msg = file_get_contents('php://input');
	$res = $handler->getResponse($msg);
	
	ob_start();
	print_r($res);
	$msgRes = ob_get_clean();
	
	unlink('./incdata-raw.txt');
	unlink('./incdata-parsed.txt');
	
	error_log($msg, 3, './incdata-raw.txt');
	error_log($msgRes, 3, './incdata-parsed.txt');
?>
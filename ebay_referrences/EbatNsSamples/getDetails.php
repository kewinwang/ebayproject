<?php
	require_once 'EbatNs/EbatNs_ServiceProxy.php';
	require_once 'EbatNs/GeteBayDetailsRequestType.php';
	
	$session = new EbatNs_Session('config/ebay.config.php');
	
	$cs = new EbatNs_ServiceProxy($session);
	$req = new GeteBayDetailsRequestType();
	$req->setDetailName('RegionDetails');

	$res = $cs->GeteBayDetails($req);
	
	echo "<pre>";
	print_r($res);
?>
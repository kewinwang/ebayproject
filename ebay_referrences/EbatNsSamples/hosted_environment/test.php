<?php
	require_once 'setinclude.php';
	
	// ... start here with your script
	require_once 'EbatNs/EbatNs_ServiceProxy.php';
	require_once 'EbatNs/Type/GeteBayOfficialTimeRequestType.php';

	$session = new EbatNs_Session('config.php');
	$cs = new EbatNs_ServiceProxy($session);
	
	$req = new GeteBayOfficialTimeRequestType();
	
	$res = $cs->GeteBayOfficialTime($req);
	echo "<pre>";
	print_r($res);
?>
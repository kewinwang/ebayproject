<?php
	require_once 'EbatNs/EbatNs_ServiceProxy.php';
	require_once 'EbatNs/GetNotificationsUsageRequestType.php';
	
	$session = new EbatNs_Session('config/ebay.config.php');
	
	$cs = new EbatNs_ServiceProxy($session);
	$req = new GetNotificationsUsageRequestType();
	
	$req->StartTime = gmdate('Y-m-d H:i:s', time() - 60 * 60 * 24 * 2);
	$req->EndTime   = gmdate('Y-m-d H:i:s');

	$response = $cs->GetNotificationsUsage($req);
	
	echo "<pre>";
	print_r($response);
	echo "</pre>";
?>
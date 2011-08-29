<?php

	require_once 'EbatNs/EbatNs_ServiceProxy.php';
	require_once 'EbatNs/EbatNs_Logger.php';
	require_once 'EbatNs/GetNotificationPreferencesRequestType.php';
	require_once 'EbatNs/GetNotificationPreferencesResponseType.php';
	
	$session = new EbatNs_Session('config/ebay.config.php');
	$cs = new EbatNs_ServiceProxy($session);
	$cs->attachLogger(new EbatNs_Logger());
	
	$req = new GetNotificationPreferencesRequestType();
	$req->setPreferenceLevel('User');
	$req->setDetailLevel('ReturnAll');
	
	$res = $cs->GetNotificationPreferences($req);
	
	echo "<pre>";
	print_r($res);
?>
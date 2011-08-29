<?php
	error_reporting(E_ALL ^ E_NOTICE);;

	require_once 'EbatNs/EbatNs_ServiceProxy.php';
	require_once 'EbatNs/EbatNs_Logger.php';
	require_once 'EbatNs/GetAllBiddersRequestType.php';
	
	$session = new EbatNs_Session('config/ebay.config.php');
	
	$cs = new EbatNs_ServiceProxy($session);
	
	$logger = new EbatNs_Logger(true);
	$cs->attachLogger($logger);
	
	$req = new GetAllBiddersRequestType();
	$req->setItemID(5652664344);
	$req->setDetailLevel($Facet_DetailLevelCodeType->ReturnAll);
	$req->setCallMode($Facet_GetAllBiddersModeCodeType->ViewAll);
	
	$res = $cs->GetAllBidders($req);
	echo "<pre>";
	if ($res->getAck() != $Facet_AckCodeType->Success)
	{
		echo "we got a failure<br>";
		foreach ($res->getErrors() as $error)
		{
			echo "#" . $error->getErrorCode() . " " . htmlentities($error->getShortMessage()) . "/" . htmlentities($error->getLongMessage()) . "<br>"; 
		}	
	}
	else
	{
		echo "<pre>";
		print_r($res);
	}
	
?>
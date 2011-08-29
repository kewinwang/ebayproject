<?php
	require_once 'EbatNs/EbatNs_ServiceProxy.php';
	require_once 'EbatNs/GetPopularKeywordsRequestType.php';
	require_once 'EbatNs/PaginationType.php';
	
	$session = new EbatNs_Session('config/ebay.config.php');
	
	$cs = new EbatNs_ServiceProxy($session);
	$req = new GetPopularKeywordsRequestType();
	$req->CategoryID = '-1';
	$req->MaxKeywordsRetrieved = 10;
	$req->IncludeChildCategories = true;
	
	$req->Pagination = new PaginationType();
	$req->Pagination->EntriesPerPage = 100;
	$req->Pagination->PageNumber = 1;

	$res = $cs->GetPopularKeywords($req);
	
	echo "<pre>";
	print_r($res);
?>
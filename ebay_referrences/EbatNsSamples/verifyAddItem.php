<?php
	require_once 'EbatNs/EbatNs_ServiceProxy.php';
	require_once 'EbatNs/EbatNs_Logger.php';
	require_once 'EbatNs/VerifyAddItemRequestType.php';
	require_once 'EbatNs/AddItemRequestType.php';
	require_once 'EbatNs/ItemType.php';
	
	$session = new EbatNs_Session('config/ebay.config.php');
	
	$cs = new EbatNs_ServiceProxy($session);
	$cs->_logger = new EbatNs_Logger();
	
	$req = new VerifyAddItemRequestType();
	
	$item = new ItemType();
	$item->BuyItNowPrice	
	$item->Description = 'test צהü ב <b>Some bold text</b>';
	$item->ListingDuration = 'Days_7';
	$item->Title = 'הצü test-titel';
	$item->Currency = 'EUR';
	$item->ListingType = 'Chinese';
	$item->Quantity = 1;
	$item->StartPrice = new AmountType();
	$item->StartPrice->setTypeValue('1.0');
	$item->StartPrice->setTypeAttribute('currencyID', 'EUR');
	$item->Country = 'DE';
	$item->Location = '-- not given --';
	
	$item->PrimaryCategory = new CategoryType();
	$item->PrimaryCategory->CategoryID = 31448;
	
	$item->Site = 'Germany';
	
	$item->PaymentMethods[] = 'PayPal';
	$item->PaymentMethods[] = 'MoneyXferAccepted';
	$item->PayPalEmailAddress = 'paypal@intradesys.com';
	
	$req->Item = $item;
	
	$res = $cs->VerifyAddItem($req);
	
//	$req = new AddItemRequestType();
//	$req->Item = $item;
//	$res = $cs->AddItem($req);	
	
	echo "<pre>";
	print_r($res);
?>
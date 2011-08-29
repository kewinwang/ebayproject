<?php
require_once 'EbatNs/EbatNs_ServiceProxy.php';
require_once 'EbatNs/GetItemRequestType.php';

class EbayItemToLegacy
{
	var $_session;
	var $_cs;
	var $_dataConverter;
	
	var $_legacyMap;
	
	function EbayItemToLegacy( $configPath )
	{
		$this->_session = new EbatNs_Session( $configPath );
		$this->_cs = new EbatNs_ServiceProxy( $this->_session );

		$this->_cs->setHandler( 'ItemType', array( &$this, 'convertItem' ) );
		$this->_cs->setHandler( 'AttributeSetType', array( &$this, 'convertAttributes' ) );
		$this->_cs->setHandler( 'AmountType', array( &$this, 'convertAmount' ) );
		
		// chain in ourself as the data-converter
		$this->_dataConverter = & $this->_cs->_dataConverter;
		$this->_cs->_dataConverter = & $this;
		$this->_setupLegacyMap();
	} 

	function _setupLegacyMap()
	{
		$this->_legacyMap['BuyerProtectionCodeType'] = array('ItemIneligible' => '0' );
		$this->_legacyMap['CountryCodeType'] = array('FR' => '71', 'DE' => 77, 'US' => 0, 'AT' => 16, 'CH' => 193 );
		$this->_legacyMap['ListingType'] = array('Chinese' => '7', 'Dutch' => 2, 'Unknown' => 0 );
		$this->_legacyMap['PromotionItemPriceTypeCodeType'] = array('BuyItNowPrice' => 'BIN', 'AuctionPrice' => 'Bid');
		$this->_legacyMap['CurrencyCodeType'] = array('EUR' => 7);
		
		$this->_legacyMap['SiteCodeType'] = array('France' => 71, 'Germany' => 77);
		$this->_legacyMap['ListingDurationCodeType'] = array('Days_1' => 1, 'Days_3' => 3, 'Days_5' => 5, 'Days_7' => 7, 'Days_10' => 10);
		
	}
	
	function Retrieve($itemid, $detailLevel = 255, $IncludeWatchCount = false)
	{
		$req = new GetItemRequestType();
		$req->ItemID = $itemid;
		
		// not always true as this depends on the detailLevel
		// but anyway here is no way to distinguish and convert the legacy-value
		$req->DetailLevel = 'ReturnAll';
		$req->IncludeWatchCount = $IncludeWatchCount;
		
		$res = $this->_cs->GetItem($req);
		
		// still need to make a Ebay_Result out of the response here !
		// return new Ebay_Result();
		return null;
	}
	
	function convertItem( $type, &$data )
	{
		echo "Hello Item<pre><br>";
		print_r( $data );
		return true;
	} 

	function convertAmount( $type, & $data)
	{
		// this will flatten all amounts (anyway we will loose the currency information here !)
		$data = $data->value;
		settype($data, 'string');
		return false;
	}
	
	function convertAttributes( $type, &$data )
	{
		echo "Hello Attributes<pre><br>";
		// print_r( $data );
		return false;
	} 
	function decodeData( $data, $type = 'string' )
	{
		if ($this->_dataConverter)
			$data = $this->_dataConverter->decodeData($data, $type);
			
		if (strstr($type, 'CodeType') !== false)
		{
// echo "got CodeType " . $type . " " . $data . "<br>";

			$map = $this->_legacyMap[$type];
			if ($map)
			{
				if (isset($map[$data]))
					return $map[$data];
			}
			switch ( $type )
			{
				default:
					return $data;
			} 
		}
		
		return $data;
	} 

	function encodeData( $data, $type = 'string', $elementName = null )
	{
		if ($this->_dataConverter)
			$data = $this->_dataConverter->encodeData($data, $type);

		switch ( $type )
		{
			case 'string':
				if ( $elementName == 'Description' )
					return "<![CDATA[" . $data . "]]>";
		} 
		return $data;
	} 

	function encryptData( $data, $type = null )
	{
		if ($this->_dataConverter)
			$data = $this->_dataConverter->encryptData($data, $type);

		return $data;
	} 

	function decryptData( $data, $type = null )
	{
		if ($this->_dataConverter)
			$data = $this->_dataConverter->decryptData($data, $type);

		return $data;
	} 
} 

$query = new EbayItemToLegacy( 'config/ebay.config.php' );
$query->Retrieve(  4504181935 );
?>
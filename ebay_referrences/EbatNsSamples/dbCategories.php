#!/usr/bin/php
<?php
	set_time_limit(0);
	
	require_once 'EbatNs/EbatNs_ServiceProxy.php';
	require_once 'EbatNs/GetCategoriesRequestType.php';
	require_once 'EbatNs/CategoryType.php';
	
	require_once 'EbatNs/EbatNs_DatabaseProvider.php';
	
	require_once 'EbatNs/EbatNs_Logger.php';
	
	// ---------------------------------------------------
	class dbCategories extends EbatNs_DatabaseProvider
	{
		var $_session;
		var $_cs;

		var $_categoryVersion;
		// import-need to pass in by ref !
		function dbCategories()
		{
			$this->EbatNs_DatabaseProvider();
		}
		
		function _makeTable()
		{
			$sql  = 'create table ebat_cat (';
			$sql .= ' cat_id int, ';
			$sql .= ' parent_cat_id int, ';
			$sql .= ' level int, ';
			$sql .= ' leaf tinyint, ';
			$sql .= ' version int, ';
			$sql .= ' cat_name varchar(255) ';
			$sql .= ')';
			
			$this->executeSqlNoQuery($sql);
		}
		
		function downloadCategories($configPath, $siteid = 77)
		{
			// preparation
			$this->_session = new EbatNs_Session($configPath);
			$this->_cs = new EbatNs_ServiceProxy($this ->_session);
			
			$this->_cs->setHandler('CategoryType', array(& $this, 'storeCategory'));
			
			$this->_cs->_logger = new EbatNs_Logger('stdout');
			
			// we will not know the version till the first call went through !
			$this->_categoryVersion = -1;
			
			// truncate the db
			$this->executeSql('truncate ebat_cat');
			$this->_makeTable();
			
			// download the data of level 1 only !
			$req = new GetCategoriesRequestType();
			$req->CategorySiteID = $siteid;
			$req->LevelLimit = 1;
			$req->DetailLevel = 'ReturnAll';
			
			$res = $this->_cs->GetCategories($req);
			$this->_categoryVersion = $res->CategoryVersion;
			
			// let's update the version information on the top-level entries
			$data['version'] = $this->_categoryVersion;
			$this->executeUpdate('ebat_cat', $data, 'parent_cat_id', '0');
			
			// fetch the data back from the db and run a query for
			// each top-level id
			$rows = $this->querySqlSet('select cat_id from ebat_cat where parent_cat_id=0');
			foreach ($rows as $row)
			{
				echo "Loading tree for " . $row['cat_id'] . "<br>\n";
				
				$req = new GetCategoriesRequestType();
				$req->CategorySiteID = $siteid;
				$req->LevelLimit = 255;
				$req->DetailLevel = 'ReturnAll';
				$req->ViewAllNodes = true;
				$req->CategoryParent = $row['cat_id'];
				$this->_cs->GetCategories($req);
			}
		}
		
		function storeCategory($type, & $Category)
		{
			//#type $Category CategoryType
			$data['cat_id'] = $Category->CategoryID;
			if ($Category->CategoryParentID[0] == $Category->CategoryID)
				$data['parent_cat_id'] = '0';
			else
				$data['parent_cat_id'] = $Category->CategoryParentID[0];
			$data['cat_name'] = $Category->CategoryName;
			$data['level'] = $Category->CategoryLevel;
			$data['leaf'] = $Category->LeafCategory;
			$data['version'] = $this->_categoryVersion;
			
			$this->executeInsert('ebat_cat', $data);
						
			return true;
		}
	}
	// ---------------------------------------------------
	
	$dbCat = new dbCategories();
	
	$dbCat->_host		= 'localhost';
	$dbCat->_database	= 'ebatns';
	$dbCat->_user		= 'user';
	$dbCat->_password	= 'password';

	$dbCat->downloadCategories('config/ebay.config.php');
?>

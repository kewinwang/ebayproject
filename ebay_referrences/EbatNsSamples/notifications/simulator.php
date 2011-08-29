<html>
<head>
<title>EPN Posting Simulator</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
</head>

<body>
<pre>
<?php
	require_once 'HTML/QuickForm.php';
	
	$f = new HTML_QuickForm();
	
	$d = dir(".");
	while (false !== ($entry = $d->read())) {
		list($fname, $ext) = explode('.', $entry);
		if ($ext == 'xml')
		{
			$x = substr(strstr($fname, '_'), 1);
			$kindOf[$x] = $x;
		}
	}
	
	$f->addElement('text', 'epnUrl', 'EPN Url', array('style' => 'width: 600px'));
	$f->addElement('select', 'message', 'Message', $kindOf);
	$f->addElement('submit', 'cmdRun', 'Go');
	
	$urlPieces = parse_url($_SERVER['PHP_SELF']);
	$pInfo = pathinfo($urlPieces['path']);
	
	$defaults['epnUrl'] = 'http://' . $_SERVER['SERVER_NAME'] . $pInfo['dirname'] . '/epnHandler.php';
	$f->setDefaults($defaults);
		
	if ($f->validate())
		$f->process('doit');
	$f->display();
	
	function doit($values)
	{
		$handlerUrl = $values['epnUrl'];
	
		$fileToPost = './soapmsg_' . $values['message'] . '.xml';
		$request = file_get_contents($fileToPost);
	
		// print_r($request);
		$headers[] = 'Content-Type: text/xml; charset="utf-8"';
		$headers[] = "Content-Length: " . strlen($request);
		$headers[] = "SOAPACTION: HTTPS://developer.ebay.com/notification/xxx";
		
		$curl = curl_init(); 
		curl_setopt($curl, CURLOPT_URL, $handlerUrl);
		curl_setopt($curl, CURLOPT_POST, 1);
		curl_setopt($curl, CURLOPT_POSTFIELDS, $request);
		curl_setopt($curl, CURLOPT_HEADER, 0);
		curl_setopt($curl, CURLOPT_HTTPHEADER, $headers);
		curl_setopt($curl, CURLOPT_RETURNTRANSFER, 0);
		curl_setopt($curl, CURLOPT_USERPWD, 'ebena:eebena');
		curl_setopt($curl, CURLOPT_VERBOSE, 1);
		
		ob_start();
		curl_exec ($curl);
		$msg = ob_get_contents();
		ob_end_clean();
		curl_close ($curl); 
		echo htmlentities($msg);
	}
?>
</pre>
</body>
</html>
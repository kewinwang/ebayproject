<?php
	// please specify the folder INTO which the
	// package was placed. You will need an ABSOLUTE path here
	// so starting with / (on Unix or \ on Windows)
	$incPath = "/var/www";
	
	if (eregi('WIN',PHP_OS))
	{
		$pathConcatSeparator = ';';
	}
	else
	{
		$pathConcatSeparator = ':';
	}
    
    set_include_path($incPath 
		. $pathConcatSeparator 
		. get_include_path());
?>
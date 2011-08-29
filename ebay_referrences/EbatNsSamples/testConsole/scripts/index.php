<?php
	$d = dir(".");
	while (false !== ($entry = $d->read())) {
		if ($entry != '.' && $entry != '..' && $entry != 'index.php')
			echo "Run | Source " . $entry . "<br/>\n";
	}
	$d->close();
?>
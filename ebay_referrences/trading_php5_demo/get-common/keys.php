<?php
    //show all errors - useful whilst developing
    error_reporting(E_ALL);

    // these keys can be obtained by registering at http://developer.ebay.com
    
    $production         = false;   // toggle to true if going against production
    $compatabilityLevel = 551;    // eBay API version
    
    if ($production) {
        $devID = 'DDD';   // these prod keys are different from sandbox keys
        $appID = 'AAA';
        $certID = 'CCC';
        //set the Server to use (Sandbox or Production)
        $serverUrl = 'https://api.ebay.com/ws/api.dll';      // server URL different for prod and sandbox
        //the token representing the eBay user to assign the call with
        $userToken = 'YOUR_PROD_TOKEN';          
    } else {  
        // sandbox (test) environment
        $devID = 'DDD_SANDBOX';         // insert your devID for sandbox
        $appID = 'AAA_SANDBOX';   // different from prod keys
        $certID = 'CCC_SANDBOX';  // need three 'keys' and one token
        //set the Server to use (Sandbox or Production)
        $serverUrl = 'https://api.sandbox.ebay.com/ws/api.dll';
        // the token representing the eBay user to assign the call with
        // this token is a long string - don't insert new lines - different from prod token
        $userToken = 'YOUR_TOKEN_ABOUT_1000_CHARS';                 
    }
    
    
?>
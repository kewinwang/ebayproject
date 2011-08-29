/**
 * @file: ebay_auth_token.js 2010/11/02 tuan $
 */

/*
 * Define our ebay_auth_token js object namespace
 */
Drupal.ebay_auth_token = {};

/*
 * Initiate our functions to be executed by Drupal js
 */
Drupal.behaviors.ebay_auth_token_init = function() {
    // Open a popup for ebay signin
    Drupal.ebay_auth_token.handle_popup();
    
    // Process to remove an ebay user id
    Drupal.ebay_auth_token.remove();
};

/*
 * Process to remove an ebay uid
 */
Drupal.ebay_auth_token.remove = function() {
    $("a.auth_token_remove").click(function() {
        return confirm('Are you sure that you want to remove this eBay uid from your account?');
    });
};

/*
 * Handle a popup window used for eBay consent flow signin
 */
Drupal.ebay_auth_token.handle_popup = function() {    
    $("a.auth_token_new_window").click(function() {
        // Make a request back to the server for new session id
        $.post("/ebay_auth_token/get_sessionid", {}, 
            function(result){
                if (result['status'] == 'success') {
                    // Add session id into the link
                    $("a.auth_token_new_window").attr("href", $("a.auth_token_new_window").attr("href") + result['sessionid']);
                    $("p.p_auth_token_fetch a").attr("href", $("p.p_auth_token_fetch a").attr("href") + result['sessionid']);
                    
                    // Continue the flow
                    var popup = window.open($("a.auth_token_new_window").attr("href"));

                    if (!popup){
                        alert("A popup blocker has been detected, please unblock by adding an exception to allow it showing from your browser. After the popup is allowed, please refresh the browser to have your session reloaded.");
                        return false;
                    }

                    // Show the fetchtoken link
                    $("p.p_auth_token_fetch").show();
                    
                }else{
                    alert('Could not generate session id!');
                }
            }, "json"
        );
    
        return false;
    });
};

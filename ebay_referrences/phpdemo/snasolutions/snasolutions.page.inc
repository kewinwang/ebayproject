<?php
// $Id: snasolutions.page.inc,v 1.0 2010/06/08 - tuan Exp $

/**
* Dashboard page
*/
function snasolutions_page_dashboard() {
    if (!snasolutions_token_id_has_generated()) {
        drupal_set_message(t('You have not had any eBay auth token ids found in our system. Please '. l('click here', 'dashboard/ebay_tokens') .' to generate one now.'), 'warning');
    }
    
    return 'Coming soon!';
}

/**
* View products page
*/
function snasolutions_page_view_products() {
    $output = '<a href="" id="ebay_publish_popup" rel="simple_popup" style="display:none">&nbsp;</a>';
    $output .= views_embed_view('product_listing', 'page_1');
    
    return $output;
}

/**
* Listing settings page
*/
function snasolutions_page_listing_settings() {
    // Build a table
    $table_header = array(t('Name'), t('Updated'), t('Enabled'), t('Action'));
    $rows = array();

    // Get all listings by this user
    global $user;
    $listings = _ebay_get_listings_by_uid($user->uid);
    
    if (!empty($listings)) {
        foreach ($listings as $listing) {
            if ($listing->status) {
                $listing->status = 'Yes';
            }
            else{
                $listing->status = 'No';
            }
            
            $edit_link = l('Edit', 'dashboard/listing_settings/'. $listing->id .'/edit');
            $delete_link = l('Delete', 'dashboard/listing_settings/'. $listing->id .'/delete');
            
            $rows[] = array($listing->name, date('jS M Y', $listing->updated), $listing->status, $edit_link .' | '. $delete_link);
        }
    }
    else{
        $rows[] = array(array('data' => t('There are no templates at the moment.'), 'colspan' => 4));
    }
    
    return theme_table($table_header, $rows);
}

/**
* Listing status page
*/
function snasolutions_page_listing_status() {
    $user_token_id = snasolutions_primary_token_id_get();
    if (!$user_token_id) {
        snasolutions_primary_token_id_missing();
        return '';
    }
    else{
        global $user;
        $user_tokens_list = _ebay_auth_token_get_token($user->uid);
        
        if (!isset($user_tokens_list[$user_token_id]['token'])) {
            return '';
        }
    }
    
    $now = time();
    $time['StartTimeTo'] = date('Y-m-d\T00:00:00.000\Z', $now);
    $time['StartTimeFrom'] = date('Y-m-d\T00:00:00.000\Z', strtotime('-3 months', $now));
    $time['EndTimeTo'] = date('Y-m-d\T00:00:00.000\Z', $now);
    $time['EndTimeFrom'] = date('Y-m-d\T00:00:00.000\Z', strtotime('-3 months', $now));

    // Get listings and build a table to hold
    $table_header = array(t('Item'), t('Listed'), t('Type'), t('Status'), t('Qty left'), t('Current price'), t('Action'));
    $rows = array();
    
    $listings =  ebayapi_getsellerlist($user_tokens_list[$user_token_id]['token'], '', 20, 1, $time);
    $listing_type = array('FixedPriceItem' => 'Buy It Now', 'Chinese' => 'Auction');
    foreach ($listings['ItemArray'] as $listing) {
        $item = ebayapi_getitem($user_tokens_list[$user_token_id]['token'], $listing['ItemID']);
        $item = snasolutions_object_to_array($item);
        
        $action = array();
        $action[] = t('Remove');
        $action[] = t('End listing');
        
        $row = array(l($item['Title'], $item['ListingDetails']['ViewItemURL'], array('attributes' => array('target' => '_blank'))), date('jS M Y', strtotime($item['ListingDetails']['StartTime'])), $listing_type[$item['ListingType']], $item['SellingStatus']['ListingStatus'], $item['Quantity'] - $item['SellingStatus']['QuantitySold'], $item['SellingStatus']['CurrentPrice'], join(' ', $action));
        $rows[] = $row;
    }

    return theme_table($table_header, $rows);
}

/**
* eBay tokens page
*/
function snasolutions_page_ebay_tokens() {
    $output = '';
    $output .= drupal_get_form('snasolutions_token_selection_form');
    $output .= ebay_auth_token_page();
    
    return $output;
}

/**
* Publish ebay popup form - multistep form
*/
function snasolutions_publish_product_popup_steps_form($form_state) {
    //dsm($form_state);
    
    // Get total products
    if (arg(2) == '') {die("Invalid data!");}
    if (!isset($form_state['storage']['product_nids'])) {
        $form_state['storage']['product_nids'] = array();
        $total_products = preg_split('/\,/', arg(2));
        foreach ($total_products as $product_nid) {
            if (is_numeric($product_nid)) {
                $form_state['storage']['product_nids'][] = $product_nid;
            }
        }
    }
    else{
        if (empty($form_state['storage']['product_nids'])) {
            die("Invalid data!");
        }
    }

    // Get steps
    if (!$form_state['storage']['step']) {$form_state['storage']['step'] = 1;}
    $current_step = $form_state['storage']['step'];
    $total_steps = 4;

    // Build form
    $form = array();

    $form['step'] = array(
        '#type' => 'hidden',
        '#value' => $current_step,
    );

    $form['total_steps'] = array(
        '#type' => 'hidden',
        '#value' => $total_steps,
    );
    
    $form['total_products'] = array(
        '#type' => 'hidden',
        '#value' => count($form_state['storage']['product_nids']),
    );

    switch ($current_step) {
        // Step 1
        case 1:
            global $user;
        
            $form['notify'] = array(
                '#value' => t('Choose options below to list products on eBay.'),
            );
            
            // Get theme list
//            $themes = _ebay_listing_get_theme();
//            $theme_options = array();
//            $theme_thumbail = array();
//            $theme_thumbail['ebay_listing'] = array();
//            foreach ($themes as $id => $theme) {
//                $theme_options[$id] = $theme['Name'] .' ('. $id .')';
//                $theme_thumbail['ebay_listing'][$id] = $theme['ImageURL'];
//            }

            // Get listing template of this user
            $listings = _ebay_get_listings_by_uid($user->uid);
            $listing_options = array();
            foreach ($listings as $listing) {
                if ($listing->status) {
                    $listing_options[$listing->id] = $listing->name;
                }
            }
            
            if (empty($listing_options)) {
                drupal_set_message(t('You have not had any listing templates created or enabled.'), 'error');
                return '';
            }
            
            // Add to Drupal settings
//            drupal_add_js($theme_thumbail, 'setting');

//            $form['themeid'] = array(
//                '#type' => 'select',
//                '#title' => t('Description template'),
//                '#options' => $theme_options,
//                '#suffix' => '<div id="product-preview"><table border="0"><tr><td id="theme_thumbnail" align="center"></td></tr><tr><td align="center"><a href="#" id="preview" target="_blank">Preview</a></td></tr></table></div>',
//            );
            
            $form['templateid'] = array(
                '#type' => 'select',
                '#title' => t('Listing template'),
                '#options' => $listing_options,
                '#required' => true,
            );
            
            break;
    
        // Step 2
        case 2:
            $form['notify'] = array(
                '#value' => t('Shown below are features supported by the primary categories you chose for the template. Click "Next" to view the estimated fees.'),
            );
            
            // Get primary category's featues
            $user_token_id = snasolutions_primary_token_id_get();
            if (!$user_token_id) {
                snasolutions_primary_token_id_missing();
                return '';
            }
            else{
                global $user;
                $user_tokens_list = _ebay_auth_token_get_token($user->uid);
                
                if (!isset($user_tokens_list[$user_token_id]['token'])) {
                    drupal_set_message(t('Your token string is not found.'), 'error');
                    return '';
                }
            }

            $product_nid = $form_state['storage']['product_nids'][0];
            $product = node_load($product_nid);
            
            $category_features = ebayapi_getcategoryfeatures($user_tokens_list[$user_token_id]['token'], $product->field_ebay_categories[0]['value']);
            $category_features_output = '';
            if ($category_features) {
                $category_features_output .= '<ul class="category-features">';
                
                // Collect some features
                if (isset($category_features['VariationsEnabled'])) {
                    $category_features_output .= '<li>Products with variation can be listed in this category.</li>';
                }

                if (isset($category_features['PayPalRequired'])) {
                    $category_features_output .= '<li>PayPal is required as payment method.</li>';                
                }

                if (isset($category_features['ReturnPolicyEnabled'])) {
                    $category_features_output .= '<li>A return policy can be specified.</li>';
                }

                if (isset($category_features['ItemSpecificsEnabled'])) {
                    $category_features_output .= '<li>Item Specifics can be added.</li>';
                }
                
                $category_features_output .= '</ul>';
            }
            else{
                $category_features_output .= 'No category features are found.';
            }

            $form['category_features'] = array(
                '#value' => $category_features_output,
            );

            break;
    
        // Step 3
        case 3:
            $form['notify'] = array(
                '#value' => t('The approximate fees that will be charged to your eBay account for listing the selected product(s) are shown below. Click the "List on eBay" button below to start the listing process with eBay.'),
            );

            // Get token
            $user_token_id = snasolutions_primary_token_id_get();
            if (!$user_token_id) {
                snasolutions_primary_token_id_missing();
                return '';
            }
            
            // Get the pre-settings
            $estimated_fees = array();
            foreach ($form_state['storage']['product_nids'] as $product_nid) {
                // Get product listing template
                $template_id = $form_state['storage']['step_1']['templateid'];
                $product_listing_template = _ebay_listing_get($template_id);
                $product_listing_template->token_id = $user_token_id;
                $product_listing_template->nid = $product_nid;

                $results = ebay_list_get_estimated_fees($product_listing_template);

                if ($results['fees']) {
                    foreach ($results['fees']['Fee'] as $fee) {
                        $fee = (array)$fee;
                        if ($fee['Fee'] > 0) {
                            if (!isset($estimated_fees[$fee['Name']])) {
                                $estimated_fees[$fee['Name']] = 0;
                            }
                            $estimated_fees[$fee['Name']] += $fee['Fee'];
                        }
                    }
                }
            }
            
            $fees_output = '';
            if ($estimated_fees) {
                $fees_output .= '<ul>';
                
                foreach ($estimated_fees as $name => $fee) {
                    if ($fee > 0) {
                        $fees_output .= '<li>'. $name .' - '. $fee .'</li>';                    
                    }
                }
                
                $fees_output .= '</ul>';
            }
            else{
                $fees_output = t('There are no fees required by eBay for these listings');
            }

            $form['estimated_fees'] = array(
                '#value' => $fees_output,
            );

            break;
    
        // Step 4
        case 4:
            // Get token
            $user_token_id = snasolutions_primary_token_id_get();
            if (!$user_token_id) {
                snasolutions_primary_token_id_missing();
                return '';
            }

            // Publish products one by one
            foreach ($form_state['storage']['product_nids'] as $product_nid) {
                // Get product listing template
                $template_id = $form_state['storage']['step_1']['templateid'];
                $product_listing_template = _ebay_listing_get($template_id);
                $product_listing_template->token_id = $user_token_id;
                $product_listing_template->nid = $product_nid;

                ebay_list_publish($product_listing_template);
            }

            $form['notify'] = array(
                '#value' => t('Processes to publish products to eBay has been done!'),
            );

            break;
    }

    if ($current_step < $total_steps) {
        $form['cancel'] = array(
            '#type' => 'button',
            '#value' => t('Cancel'),
            '#attributes' => array('class' => 'button-cancel', 'onclick' => 'parent.Drupal.simple_popup.close(); return false;'),
        );

        if ($current_step > 1) {
            $form['back'] = array(
                '#type' => 'submit',
                '#value' => t('Back'),
                '#attributes' => array('class' => 'button-back'),
            );
        }

        if ($current_step == $total_steps - 1) {
            $form['next'] = array(
                '#type' => 'submit',
                '#value' => t('List on eBay'),
                '#attributes' => array('class' => 'button-next'),
            );    
        }
        else {
            $form['next'] = array(
                '#type' => 'submit',
                '#value' => t('Next'),
                '#attributes' => array('class' => 'button-next'),
            );
        }
    }
    
    return $form;
}

/**
* Steps form submmit
*/
function snasolutions_publish_product_popup_steps_form_submit($form, &$form_state) {
    // Store the step values
    $form_state['storage']['step_'. $form_state['values']['step']] = $form_state['values'];
    $form_state['rebuild'] = true;

    // Decide next / previsous step based on the current one
    if ($form_state['values']['op'] == 'Next' or $form_state['values']['op'] == 'List on eBay') {
        $form_state['storage']['step']++;
    }

    if ($form_state['values']['op'] == 'Back') {
        if ($form_state['storage']['step'] > 1) {
            $form_state['storage']['step']--;
        }
    }
    
    // If reached to the final step; go to actually submitting the form
    if ($form_state['storage']['step'] >= $form_state['values']['total_steps']) {
        //dsm('Done!');
    }
}
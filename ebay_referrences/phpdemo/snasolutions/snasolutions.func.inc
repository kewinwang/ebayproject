<?php
// $Id: snasolutions.func.inc,v 1.0 2010/06/08 - tuan Exp $


/**
* ebay template form create
*/
function snasolutions_ebay_template_form_submit($form, &$form_state) {
    // Convert from_state to object
    $product = (object)$form_state['values'];
    
    // Collect shipping values
    $shipping_domestic = array();
    $shipping_international = array();
    
    foreach ($form_state['clicked_button']['#post']['domestic_type'] as $index => $value) {
        if (!isset($form_state['clicked_button']['#post']['domestic_free'][$index])) {
            $form_state['clicked_button']['#post']['domestic_free'][$index] = 0;
        }
        
        $shipping_domestic[] = array(
            'type' => $form_state['clicked_button']['#post']['domestic_type'][$index],
            'cost' => $form_state['clicked_button']['#post']['domestic_cost'][$index],
            'surcharge' => $form_state['clicked_button']['#post']['domestic_surcharge'][$index],
            'free' => $form_state['clicked_button']['#post']['domestic_free'][$index],
        );
    }

    foreach ($form_state['clicked_button']['#post']['international_type'] as $index => $value) {
        if (!isset($form_state['clicked_button']['#post']['international_free'][$index])) {
            $form_state['clicked_button']['#post']['international_free'][$index] = 0;
        }
        
        $shipping_international[] = array(
            'type' => $form_state['clicked_button']['#post']['international_type'][$index],
            'cost' => $form_state['clicked_button']['#post']['international_cost'][$index],
            'location' => $form_state['clicked_button']['#post']['international_location'][$index],
            'free' => $form_state['clicked_button']['#post']['international_free'][$index],
        );
    }

    $product->shipping_domestic = serialize($shipping_domestic);
    $product->shipping_international = serialize($shipping_international);
    
    $result = _ebay_listing_save($product);

    if ($result == SAVED_NEW) {
        drupal_set_message(t('eBay publishing details have been created successfuly.'));
    }
    else if ($result == SAVED_UPDATED) {
        drupal_set_message(t('eBay publishing details have been updated successfuly.'));    
    }
    else{
        drupal_set_message(t('Failed to save the details'), 'error');
    }
    
    // Go back to listing page
    drupal_goto('dashboard/listing_settings');
}

/**
* Delete ebay template form
*/
function snasolutions_ebay_template_delete_form() {
    // Get params if edit mode
    if (is_numeric(arg(2)) and arg(3) == 'delete') {
        $ebay_listing = _ebay_listing_get(arg(2));
    }

    if (!is_object($ebay_listing)) {
        die("Invalid data!");
    }
    
    $form = array();
    
    $form['confirm_message'] = array(
        '#value' => t('Please confirm if you want to remove this template %template_name.<br /><br />', array('%template_name' => $ebay_listing->name)),
    );
    
    $form['id'] = array(
        '#type' => 'hidden',
        '#value' => $ebay_listing->id,
    );

    $form['name'] = array(
        '#type' => 'hidden',
        '#value' => $ebay_listing->name,
    );
    
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Confirm'),
    );
    
    return $form;
}

/**
* Delete ebay template form submit
*/
function snasolutions_ebay_template_delete_form_submit($form, &$form_state) {
    if ($form_state['values']['id']) {
        _ebay_listing_remove($form_state['values']['id']);
        drupal_set_message(t('The template %template_name has been removed successfully.', array('%template_name' => $form_state['values']['name'])));
        drupal_goto('dashboard/listing_settings');
    }
    else{
        die("Invalid data!");
    }
}

/**
* Create / edit ebay template form
*/
function snasolutions_ebay_template_form() {
    // Get params if edit mode
    if (is_numeric(arg(2)) and arg(3) == 'edit') {
        $ebay_listing = _ebay_listing_get(arg(2));
    }
    
    $form = array();
    
    // Id (for edit case)
    if ($ebay_listing->id) {
        $form['id'] = array(
            '#type' => 'hidden',
            '#value' => $ebay_listing->id,
        );
    }
    
    // Name
    $form['name'] = array(
        '#type' => 'textfield',
        '#title' => t('Template name'),
        '#default_value' => $ebay_listing->name ? $ebay_listing->name : '',
        '#required' => true,
    );

    $form['status'] = array(
        '#type' => 'checkbox',
        '#title' => t('Enable this template?'),
        '#return_value' => 1,
        '#default_value' => $ebay_listing->status ? $ebay_listing->status : 0,
    );
    
    // Listing type
    $form['listing_type_group'] = array(
        '#type' => 'fieldset',
        '#title' => t('Listing type'),
        '#collapsible' => true,
        '#collapsed' => false,
    );
    
    $form['listing_type_group']['listing_type'] = array(
        '#type' => 'radios',
        '#default_value' => $ebay_listing->listing_type ? $ebay_listing->listing_type : 'Standard Fixed Price',
        '#options' => array('Standard Auction' => t('Standard Auction'), 'Standard Fixed Price' => t('Standard Fixed Price')),
    );
    
    $form['listing_type_group']['auction_group'] = array(
        '#type' => 'fieldset',
        '#collapsible' => false,
        '#collapsed' => false,
        '#attributes' => array('class' => 'auction_group'),
    );

    $form['listing_type_group']['auction_group']['auction_starting_price'] = array(
        '#type' => 'textfield',
        '#title' => t('Starting price'),
        '#default_value' => $ebay_listing->auction_starting_price ? $ebay_listing->auction_starting_price : round($product->sell_price, 2),
        '#required' => true,
    );

    $form['listing_type_group']['auction_group']['auction_price'] = array(
        '#type' => 'textfield',
        '#title' => t('Buy now price'),
        '#default_value' => $ebay_listing->auction_price ? $ebay_listing->auction_price : round($product->sell_price, 2),
        '#required' => true,
    );

    $form['listing_type_group']['auction_group']['auction_quantity'] = array(
        '#type' => 'hidden',
        //'#default_value' => $ebay_listing->auction_quantity ? $ebay_listing->auction_quantity : 1,
        '#default_value' => 1,
    );

    $form['listing_type_group']['auction_group']['auction_duration'] = array(
        '#type' => 'select',
        '#title' => t('Duration'),
        '#options' => array(3 => '3 days', 5 => '5 days', 7 => '7 days', 10 => '10 days'),
        '#default_value' => $ebay_listing->auction_duration ? $ebay_listing->auction_duration : 3,
        '#description' => t('Select one of valid the eBay duration values. '. l('Visit here', 'https://ebay.custhelp.com/app/answers/detail/a_id/426', array('attributes' => array('target' => '_blank'))) .' for more info.'),
        //'#suffix' => t('days'),
    );
    
    $form['listing_type_group']['fixedprice_group'] = array(
        '#type' => 'fieldset',
        '#collapsible' => false,
        '#collapsed' => false,
        '#attributes' => array('class' => 'fixedprice_group'),
    );

    $form['listing_type_group']['fixedprice_group']['fixedprice_price'] = array(
        '#type' => 'textfield',
        '#title' => t('Buy now price'),
        '#default_value' => $ebay_listing->fixedprice_price ? $ebay_listing->fixedprice_price : round($product->sell_price, 2),
        '#required' => true,
    );

    $form['listing_type_group']['fixedprice_group']['fixedprice_quantity'] = array(
        '#type' => 'textfield',
        '#title' => t('Quantity'),
        '#default_value' => $ebay_listing->fixedprice_quantity ? $ebay_listing->fixedprice_quantity : 1,
    );

    $form['listing_type_group']['fixedprice_group']['fixedprice_duration'] = array(
        '#type' => 'select',
        '#title' => t('Duration'),
        '#options' => array(3 => '3 days', 5 => '5 days', 7 => '7 days', 10 => '10 days', 30 => '30 days'),
        '#default_value' => $ebay_listing->fixedprice_duration ? $ebay_listing->fixedprice_duration : 3,
        '#description' => t('Select one of valid the eBay duration values. '. l('Visit here', 'https://ebay.custhelp.com/app/answers/detail/a_id/426', array('attributes' => array('target' => '_blank'))) .' for more info.'),
        //'#suffix' => t('days'),
    );
    
    // Payment method
    $form['payment_settings'] = array(
        '#type' => 'fieldset',
        '#title' => t('Payment settings'),
        '#collapsible' => true,
        '#collapsed' => false,
    );

    $form['payment_settings']['paypal_email'] = array(
        '#type' => 'textfield',
        '#title' => t('Paypal email address'),
        '#default_value' => $ebay_listing->paypal_email ? $ebay_listing->paypal_email : $user->mail,
        '#required' => true,
    );
    
    // Shipping method
    $form['shipping_group'] = array(
        '#type' => 'fieldset',
        '#title' => t('Shipping methods'),
        '#collapsible' => true,
        '#collapsed' => false,
        '#attributes' => array('class' => 'shipping_group'),
    );
    
    $form['shipping_group']['shipping_method'] = array(
        '#type' => 'radios',
        '#default_value' => $ebay_listing->shipping_method ? $ebay_listing->shipping_method : 'Domestic Shipping',
        '#options' => array('Domestic Shipping' => t('Domestic Shipping'), 'International Shipping' => t('International Shipping')),
    );

    $form['shipping_group']['domestic_shipping_group'] = array(
        '#type' => 'fieldset',
        '#collapsible' => false,
        '#collapsed' => false,
        '#attributes' => array('class' => 'domestic_shipping_group'),
    );
    
    $form['shipping_group']['domestic_shipping_group']['domestic'] = array(
        '#value' => _ebay_listing_domestic_shipping_group(unserialize($ebay_listing->shipping_domestic)),
    );
    
    $form['shipping_group']['international_shipping_group'] = array(
        '#type' => 'fieldset',
        '#collapsible' => false,
        '#collapsed' => false,
        '#attributes' => array('class' => 'international_shipping_group'),
    );

    $form['shipping_group']['international_shipping_group']['international'] = array(
        '#value' => _ebay_listing_international_shipping_group(unserialize($ebay_listing->shipping_international)),
    );
    
    // Handling time
    $form['shipping_group']['handling_time'] = array(
        '#type' => 'textfield',
        '#title' => t('Handling time'),
        '#default_value' => $ebay_listing->handling_time ? $ebay_listing->handling_time : 1,
        '#suffix' => t('days'),
        '#required' => true,
    );

    // Listing site
    $form['listing_site'] = array(
        '#type' => 'fieldset',
        '#title' => t('Where to list'),
        '#collapsible' => true,
        '#collapsed' => false,
        '#attributes' => array('class' => 'listing_site_group'),
    );
    
    $form['listing_site']['ebay_site'] = array(
        '#type' => 'select',
        '#title' => t('eBay marketing site'),
        '#options' => _ebay_listing_get_ebay_sites(),
        '#default_value' => $ebay_listing->ebay_site ? $ebay_listing->ebay_site : 0,
    );

        
    // Submit button
    $form['save_and_exit'] = array(
        '#type' => 'submit',
        '#value' => $ebay_listing->id ? t('Save changes') : t('Create new'),
    );
    
    return $form;
}

/**
* Import products from CSV file
*/
function snasolutions_import_products() {
    return 'Please confirm with website admin before runing this script!';
    
    // We need this script for as long as possible
    set_time_limit(0);
    
    // Columns
    $columns = array('Use eBay ID', 'Listing Type', 'Market', 'Item Condition', 'Quantity', 'Duration', 'Schedule 1', 'Domestic Shipping 1', 'Cost 1', 'Free 1', 'Surcharge 1', 'Domestic Shipping 2', 'Cost 2', 'Free 2', 'Surcharge 2', 'Domestic Shipping 3', 'Cost 3', 'Free 3', 'Surcharge 3', 'International Shipping 1', 'Cost 4', 'Free 4', 'iShipToLocation 1', 'International Shipping 2', 'Cost 5', 'Free 5', 'iShipToLocation 2', 'International Shipping 3', 'Cost 6', 'Free 6', 'iShipToLocation 3', 'Item location', 'Paypal', 'Inventory SKU', 'Category', 'Listing Title', 'Price', 'Gallery Image', 'Description', 'Optional Store category 1', 'Optional Store category 2', 'Brand', 'Style', 'Sleeve Style', 'Size Type', 'Size', 'Occasion', 'Material', 'Dress Length', 'Color', 'Visitor Counter');
    $fields = array();
    
    // Open the CSV files
    $filename = drupal_get_path('module', 'snasolutions') .'/data/Content.csv';
    
    // Collect data
    $row_count = 0;
    if (($handle = fopen($filename, 'r')) !== FALSE) {
        while (($data = fgetcsv($handle, 1000, ",")) !== FALSE) {
            // Skip row 1 as header
            if ($row_count == 0) {
                $row_count++;
                continue;
            }
            
            // Return if no data
            if (!$data[0] or $data[0] == '') {break;}
            
            // Collect data into column array
            for ($i=0; $i<count($data); $i++) {
                $fields[$columns[$i]][] = trim($data[$i]);
            }
                        
            $row_count++;
        }
    }

//    dsm($fields);
//    return '';
    
    // Load node module
    module_load_include('inc', 'node', 'node.pages');

    // Import into product
    $customfield_index = 0;
    foreach ($fields['Use eBay ID'] as $index => $value) {
        // Create product here
        $product_node = new stdClass();
        
        // Drupal values
        global $user;
        $product_node->type = 'ebaylister';
        $product_node->language = 'en';
        $product_node->status = 1;
        $product_node->uid = $user->uid;
        $product_node->name = $user->name;
        $product_node->created = time();
        $product_node->title = $fields['Listing Title'][$index];
        $product_node->body = $fields['Description'][$index];
        
        // Ubercart values
        $product_node->model = $fields['Inventory SKU'][$index];
        $product_node->sell_price = preg_replace('/\$/', '', $fields['Price'][$index]);
        $product_node->default_qty = $fields['Quantity'][$index];
        
        // Default CCK field values
        $product_node->field_product_type[0]['value'] = 'physical';
        $product_node->field_product_availability[0]['value'] = 0;
        $product_node->field_tracking_method[0]['value'] = 0;
        
        // Categories
        $category_id = _ebay_category_mapping_db_get_categoryid_by_categoryname($fields['Category'][$index]);
        if (!$category_id) {
            drupal_set_message('Category name "'. $fields['Category'][$index] .'" not found.', 'warning');
        }
        else{
            $product_node->field_ebay_categories[0]['value'] = $category_id;
        }
        
        // Custom fields
        if ($fields['Brand'][$index]) {
            $product_node->field_customfield_name[$customfield_index]['value'] = 'Brand';
            $product_node->field_customfield_value[$customfield_index]['value'] = $fields['Brand'][$index];
            $customfield_index++;
        }
        if ($fields['Style'][$index]) {
            $product_node->field_customfield_name[$customfield_index]['value'] = 'Style';
            $product_node->field_customfield_value[$customfield_index]['value'] = $fields['Style'][$index];
            $customfield_index++;
        }
        if ($fields['Sleeve Style'][$index]) {
            $product_node->field_customfield_name[$customfield_index]['value'] = 'Sleeve Style';
            $product_node->field_customfield_value[$customfield_index]['value'] = $fields['Sleeve Style'][$index];
            $customfield_index++;
        }
        if ($fields['Size Type'][$index]) {
            $product_node->field_customfield_name[$customfield_index]['value'] = 'Size Type';
            $product_node->field_customfield_value[$customfield_index]['value'] = $fields['Size Type'][$index];
            $customfield_index++;
        }
        if ($fields['Size'][$index]) {
            $product_node->field_customfield_name[$customfield_index]['value'] = 'Size';
            $product_node->field_customfield_value[$customfield_index]['value'] = $fields['Size'][$index];
            $customfield_index++;
        }
        if ($fields['Occasion'][$index]) {
            $product_node->field_customfield_name[$customfield_index]['value'] = 'Occasion';
            $product_node->field_customfield_value[$customfield_index]['value'] = $fields['Occasion'][$index];
            $customfield_index++;
        }
        if ($fields['Material'][$index]) {
            $product_node->field_customfield_name[$customfield_index]['value'] = 'Material';
            $product_node->field_customfield_value[$customfield_index]['value'] = $fields['Material'][$index];
            $customfield_index++;
        }
        if ($fields['Dress Length'][$index]) {
            $product_node->field_customfield_name[$customfield_index]['value'] = 'Dress Length';
            $product_node->field_customfield_value[$customfield_index]['value'] = $fields['Dress Length'][$index];
            $customfield_index++;
        }
        if ($fields['Color'][$index]) {
            $product_node->field_customfield_name[$customfield_index]['value'] = 'Color';
            $product_node->field_customfield_value[$customfield_index]['value'] = $fields['Color'][$index];
            $customfield_index++;
        }
        
        // Product images
        if ($fields['Gallery Image'][$index]) {
            $file = snasolutions_create_filefield('sites/default/files/products/'. $fields['Gallery Image'][$index]);
            if ($file) {
                $product_node->field_image_cache[0] = array(
                    'fid' => $file->fid,
                    'filename' => $file->filename,
                    'filepath' => $file->filepath,
                    'filesize' => $file->filesize,
                    'data' => array(
                        'description' => basename($file->filename),
                    ),
                    'list' => 1,
                );
            }
            else{
                drupal_set_message('File '. $fields['Gallery Image'][$index] .' not found or filefield object could not be created.', 'warning');
            }
        }
        
        // Prepare and store the product
        node_object_prepare($product_node);
        node_save($product_node);
        //dsm($product_node);
        
        if (!$product_node->nid) {
            drupal_set_message('Could not create node for '. $product_node->title, 'warning');
            continue;
        }
        
        // Collect data for publishing details for ebay
        $product = new stdClass();
        $product->nid = $product_node->nid;
        $product->primary_category = $product_node->field_ebay_categories[0]['value'];
        //$product->secondary_category
        $product->listing_type = $fields['Listing Type'][$index];
        $product->auction_starting_price = $product_node->sell_price;
        $product->auction_price = $product_node->sell_price;
        $product->auction_quantity = 1;
        $product->auction_duration = 3; //$fields['Duration'][$index]; // 3-5-7-10
        $product->fixedprice_price = $product_node->sell_price;
        $product->fixedprice_quantity = $fields['Quantity'][$index];
        $product->fixedprice_duration = 3; //$fields['Duration'][$index]; // 3-5-7-10
        $product->payment_type = 'Paypal';
        $product->paypal_email = $fields['Paypal'][$index];
        $product->shipping_method = 'Domestic Shipping';
        $product->domestic_type = 544; // Economy Shipping From Out side US
        $product->domestic_cost = preg_replace('/\$/', '', $fields['Cost 1'][$index]);
        $product->domestic_surcharge = preg_replace('/\$/', '', $fields['Surcharge 1'][$index]);
        $product->domestic_free = $fields['Free 1'][$index] ? 1 : 0;
        $product->international_type = 591; // Economy Int'l Shipping
        $product->international_cost = preg_replace('/\$/', '', $fields['Cost 4'][$index]);
        $product->international_location = 548; // Worldwide
        $product->international_free = $fields['Free 4'][$index] ? 1 : 0;
        $product->handling_time = 3; // Default
        
        _ebay_listing_save($product);
        //dsm($product);
        
        //break;
    }
    
    return 'Done!';
}

/**
* Create filefield
*/
function snasolutions_create_filefield($filepath) {
    global $user;
  
    $file = new stdClass();
    $file->filename = basename($filepath);
    $file->filepath = $filepath;
    $file->filesize = filesize($filepath);

    $file->uid = $user->uid;
    $file->status = FILE_STATUS_PERMANENT;
    $file->timestamp = time();
    
    if (drupal_write_record('files', $file)) {
        return $file;
    }
    else {
        return false;
    }
}

/**
* Get primary token id of the user
*/
function snasolutions_primary_token_id_get() {
    global $user;
    if (!$user->uid) {return false;}
    
    $user_tokens_list = variable_get('snasolutions_token_list', array());
    return $user_tokens_list[$user->uid] ? $user_tokens_list[$user->uid] : false;
}

/**
* Warning user if primary token id has not been selected
*/
function snasolutions_primary_token_id_missing() {
    drupal_set_message(t('You have not selected your primary token id.'), 'error');
}

/**
* Warning user if there is no token id has been generated
*/
function snasolutions_token_id_has_generated() {
    global $user;
    $user_tokens = _ebay_auth_token_get_token($user->uid);
    
    return !empty($user_tokens) ? true : false;
}

/**
* Generate token id selection form
*/
function snasolutions_token_selection_form() {
    global $user;
    
    // Get list of all stored tokens of all users
    $user_tokens_list = variable_get('snasolutions_token_list', array());

    // Tokens for this user
    $user_tokens = _ebay_auth_token_get_token($user->uid);

    // Build the form for token id selection
    $form = array();
    
    // eBay token id
    $user_tokens_select = array(0 => t('- Please select'));
    if (!empty($user_tokens)) {
        foreach ($user_tokens as $token_id => $token_values) {
            $user_tokens_select[$token_id] = $token_id;
        }
    }

    $form['token_id'] = array(
        '#type' => 'select',
        '#title' => t('Select your primary token id'),
        '#options' => $user_tokens_select,
        '#required' => true,
        '#default_value' => $user_tokens_list[$user->uid] ? $user_tokens_list[$user->uid] : '0',
        '#description' => t('This token will be used when publishing products on eBay.'),
    );
    
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
    );
    
    return $form;
}

/**
* Token validate form
*/
function snasolutions_token_selection_form_validate($form, &$form_state) {
    if ($form_state['values']['token_id'] == '0') {
        form_set_error('token_id', t('Please select one for your primary token id.'));
    }
}

/**
* Token submit form
*/
function snasolutions_token_selection_form_submit($form, &$form_state) {
    global $user;
    $user_tokens_list = variable_get('snasolutions_token_list', array());
    $user_tokens_list[''. $user->uid] = $form_state['values']['token_id'];
    
    variable_set('snasolutions_token_list', $user_tokens_list);
    drupal_set_message(t('Your primary token id is now selected as %token_id', array('%token_id' => $form_state['values']['token_id'])));
}

/**
* Convert object to array recursively
*/
function snasolutions_object_to_array($item) {
    // Return
    if (!is_object($item) and !is_array($item)) {return $item;}
    
    // Convert
    if (is_object($item)) {
        $item = (array)$item;
    }
    
    // All sub-items as well
    $items = array();
    foreach ($item as $key => $value) {
        $items[$key] = snasolutions_object_to_array($value);
    }
    
    return $items;
}

/**
* Validate ebaylister form
*/
function snasolutions_validate_ebaylister_form($form, &$form_state) {
    //dsm($form_state);
    
    // Category must be selected
    if (!$form_state['clicked_button']['#post']['category_id']) {
        form_set_error('ebay_category', t('Please select at least one category for the product.'));
    }
    
    // Customfield must be filled if presense
    if (count($form_state['values']['field_customfield_name']) != count($form_state['values']['field_customfield_value'])) {
        form_set_error('field_customfield_name', t('Please fill or remove the Custom Field which is no more needed.'));
    }
}

/**
* Set ebay category to taxonomy
*/
function snasolutions_set_category($form, &$form_state) {
    $form_state['values']['field_ebay_categories'] = array();
    if ($form_state['clicked_button']['#post']['category_id']) {
        foreach ($form_state['clicked_button']['#post']['category_id'] as $index => $category_id) {
            $form_state['values']['field_ebay_categories'][$index]['value'] = $category_id;
        }
    }
}

/**
* Build eBay categories based on local data
* $current_categories - Current categories
*/
function snasolutions_ebay_categories($current_categories) {
    // Collect values
    $current_categories_value = array();
    $index = 0;
    while (true) {
        if (is_numeric($current_categories[$index]['#default_value']['value'])) {
            $current_categories_value[] = $current_categories[$index]['#default_value']['value'];
        }
        else if (!isset($current_categories[$index]['#default_value']['value'])){
            break;
        }
        
        $index++;
    }
    
    // Get all level 1 categories
    //$categories = _ebay_category_mapping_db_get_by_categoryparentid(-1);
    
    if (!$categories) {
        $categories = _ebay_category_mapping_get_category_array(ebayapi_getcategoryinfo(-1));
        
        // Remove the first category as it is the parent
        unset($categories[-1]);
    }
    

    $output = '<div id="categories_holder">';
    
    $output .= '<div class="category-left"><div class="label">Categories: <span title="This field is required." class="form-required">*</span></div>';
    $output .= '<div id="ebay_categories_holder">';
    foreach ($categories as $cid => $values) {
        $output .= '<ul><li id="folder_'. $cid .'" class="level_'. $values['CategoryLevel'] .'"><span class="is_closed not_built li_row_label" id="'. $cid .'" onclick="Drupal.snasolutions.tree_click(this);">'. $values['CategoryName'] .'</span></li></ul>';
    }
    $output .= '</div></div>';
    
    $output .= '<div class="category-right"><div class="label">Current selected categories:</div>';
    $output .= '<div id="ebay_curent_categories_holder">';
    $output .= '<ul>';
    if ($current_categories_value) {
        foreach ($current_categories_value as $category_id) {
            // Get the category object
            //$category = _ebay_category_mapping_db_get_by_categoryid($category_id);
            if (!$category) {
                $category = _ebay_category_mapping_get_category_array(ebayapi_getcategoryinfo($category_id));
            }
            
            $output .= '<li><input type="checkbox" name="category_id[]" checked="checked" value="'. $category_id .'" class="category_checkbox" /><span>'. $category[$category_id]['CategoryName'] .' ('. $category_id .')</span></li>';
        }
    }
    else{
        $output .= '<li class="empty_text">No categories have been selected currently.</li>';
    }
    $output .= '</ul>';
    $output .= '</div></div>';
    
    $output .= '</div>';
    return $output;    
}

/**
* Wrapper function for taxonomy_get_tree.
* This function will auto convert into an array of key->value pair
*/
function _snasolutions_taxonomy_get_tree($vid, $parent = 0, $depth = -1, $max_depth = null) {
    $tree = taxonomy_get_tree($vid, $parent, $depth, $max_depth);
    $result = array();
    foreach ($tree as $term) {
        $result[$term->tid] = $term->name;
    }
    
    return $result;
}

/**
* Taxonomy name to tid (create new if not exist)
* @return: term object
*/
function _snasolutions_taxonomy_save($vid, $name) {
    $term = _snasolutions_taxonomy_get_terms_by_name($vid, $name);
    
    if (empty($term)) {
        // Create a forum under the container here
        $term = array(
            'vid' => $vid,
            'name' => $name,
            //'parent' => array($form_state['values']['tid']),
            //'description' => $form_state['values']['description'],
        );    
    
        taxonomy_save_term($term);
    }
    else{
        // Get first term only
        $term = $term[0];
    }

    return (object)$term;
}

/**
* Get term by name, revamp from taxonomy_get_term_by_name
* We need to get name under a parent, that's why
*/
function _snasolutions_taxonomy_get_terms_by_name($vid, $name) {
  $db_result = db_query(db_rewrite_sql("SELECT t.tid, t.* FROM {term_data} t WHERE LOWER(t.name) = LOWER('%s') AND t.vid = %d", 't', 'tid'), trim($name), $vid);
  $result = array();
  while ($term = db_fetch_object($db_result)) {
    $result[] = $term;
  }

  return $result;
}

/**
* Get tid from a term object
* This helps to assign tid into the CSV conversion
*/
function _snasolutions_get_tid_from_term($term) {
    return $term->tid;
}

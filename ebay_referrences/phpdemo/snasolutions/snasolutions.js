/**
 * @file: snasolutions.js 2010/09/13 - tuan $
 */

// Define our snasolutions js object namespace
Drupal.snasolutions = {};

/*
 * Initiate our functions to be executed by Drupal js
 */
Drupal.behaviors.snasolutions_init = function() {
    // Handle tabs
    Drupal.snasolutions.tabs();
    
    // Handle listing type
    Drupal.snasolutions.listing_types();
    
    // Update ubercart prices
    Drupal.snasolutions.prices_update();
    
    // eBay category checkbox updater
    Drupal.snasolutions.select_category();
    
    // Shipping methods
    Drupal.snasolutions.shipping_methods();
    
    // Select ebay template thumbnail
    Drupal.snasolutions.ebay_template_select();
    
    // Select products in view_products page
    Drupal.snasolutions.select_product();
};

// View product page
Drupal.snasolutions.select_product = function() {
    // List product on ebay
    $("#list_on_ebay").click(function() {
        // Get all checked products
        var product_nids = $(".select_nid:checked");
        
        if (!product_nids.length) {
            alert('Please select at least one product!');
            return false;
        }
        
        var nids = new Array();
        for (var i=0; i<product_nids.length; i++) {
            nids.push($(product_nids[i]).val());
        }
        
        // Go click on the hidden popup link
        $("#ebay_publish_popup").attr("href", "/publish/products/"+ nids.join(','));
        $("#ebay_publish_popup").click();
    });

    // Select a product
    $(".select_nid").click(function() {
        if (this.checked) {
            $(this).parents('tr:first').addClass('row-selected');
        }
        else{
            $(this).parents('tr:first').removeClass('row-selected');
        }
    });
    
    // Select all
    $("#select_all_nid").click(function() {
        var check = this.checked;
        $(".select_nid").each(function() {
            this.checked = check;
            if (this.checked) {
                $(this).parents('tr:first').addClass('row-selected');
            }
            else{
                $(this).parents('tr:first').removeClass('row-selected');
            }
        });
    });
    
};

// Select an ebay template
Drupal.snasolutions.ebay_template_select = function() {
    // Init load
    if ($("#edit-themeid").length) {
        $("#theme_thumbnail").html('<img src="'+ Drupal.settings['ebay_listing'][$("#edit-themeid").val()] +'" border="0" />');
    }

    // Change event
    $("#edit-themeid").change(function() {
        $("#theme_thumbnail").html('<img src="'+ Drupal.settings['ebay_listing'][$(this).val()] +'" border="0" />');
    });
    
    // Preview
    $("#preview").click(function() {
        var theme_id = parseInt($("#edit-themeid").val());
        if (theme_id == 10) {return false;}
        
        $(this).attr("href", "/ebay_listing/preview/"+ $("#edit-nid").val() +"/"+ theme_id);
        return true;
    });
};

// Handle the checkboxes for eBay category selection
Drupal.snasolutions.select_category = function() {
    // When event happens in eBay category
    $(".ebay_category_checkbox").live('click', function() {
        if ($(".category_checkbox[value='"+ $(this).val() +"']").length) {
            $(".category_checkbox[value='"+ $(this).val() +"']").attr("checked", this.checked);
        }
        else{
            // Add new
            $("#ebay_curent_categories_holder ul").append('<li><input type="checkbox" class="category_checkbox" value="'+ $(this).val() +'" checked="checked" name="category_id[]"><span>'+ $(this).next().text() +' ('+ $(this).val() +')</span></li>');
            
            // Remove empty text
            $("#ebay_curent_categories_holder ul li.empty_text").remove();
        }
    });

    // When event happens in current selected category
    $(".category_checkbox").live('click', function() {
        if ($(".ebay_category_checkbox[value='"+ $(this).val() +"']").length) {
            $(".ebay_category_checkbox[value='"+ $(this).val() +"']").attr("checked", this.checked);
        }
    });

};

// Get category from eBay and update to the tree
Drupal.snasolutions.get_category = function(category_id) {
    // Make request to eBay for categories
    $.post('/ebay_category_mapping/build_category/'+ category_id, {}, function(output) {
        if (output['status'] == 'success') {
            // Process to update the tree under the parent id 'category_id'
            var parent_label = $("#"+ category_id); // hold all classes and handler
            var parent_id = $(parent_label).parent().attr("id"); // holder the sub folder id only
            
            // Add subfolder for new fetched children
            $(parent_label).parent().find('ul').remove(); // Remove duplication
            $(parent_label).parent().append('<ul>');
            
            for (key in output['list']) {
                if (output['list'][key]['LeafCategory'] == 'false' || output['list'][key]['LeafCategory'] == '0') {
                    $("#"+ parent_id +" ul").append('<li id="folder_'+ key +'" class="level_'+ output['list'][key]['CategoryLevel'] +' leaf_'+ output['list'][key]['LeafCategory'] +'"><span id="'+ key +'" class="not_built is_closed li_row_label" onclick="Drupal.snasolutions.tree_click(this);">'+ output['list'][key]['CategoryName'] +'</span></li>');
                }
                else{
                    var is_checked = "";
                    if ($(".category_checkbox[value='"+ key +"']").length) {
                        if ($(".category_checkbox[value='"+ key +"']").attr("checked")) {
                            is_checked = "checked='checked'";
                        }
                    }
                
                    $("#"+ parent_id +" ul").append('<li id="cid_'+ key +'" class="level_'+ output['list'][key]['CategoryLevel'] +' leaf_'+ output['list'][key]['LeafCategory'] +'"><input type="checkbox" name="ebay_category_id[]" value="'+ key +'" class="ebay_category_checkbox" '+ is_checked +' /><span class="li_row_label">'+ output['list'][key]['CategoryName'] +'</span></li>');
                }
            }
            
            // Add attributes for detection later
            if ($("#"+ parent_id +" ul").children().length > 0) {
                $(parent_label).addClass('has_children');
                $(parent_label).addClass('is_opened');
                $(parent_label).removeClass('is_closed');
            }
            
            $(parent_label).removeClass('not_built'); // Not send request to eBay second time
                        
        }else{
            // No children at all, mark as "built" already
            var parent_label = $("#"+ category_id); // hold all classes and handler
            $(parent_label).removeClass('not_built');
            $(parent_label).addClass('not_found');
        }
    }, 'json');
};

// Handle click on the ebay tree
Drupal.snasolutions.tree_click = function(obj) {
    if ($(obj).hasClass('is_closed')) {
        if ($(obj).hasClass('not_built')) {
            Drupal.snasolutions.get_category($(obj).attr("id"));
            
        }else if ($(obj).hasClass('has_children')) {
            $(obj).next().slideDown();
            $(obj).addClass('is_opened');
            $(obj).removeClass('is_closed');
        }
        
    }else if ($(obj).hasClass('is_opened')) {
        $(obj).next().slideUp();
        $(obj).addClass('is_closed');
        $(obj).removeClass('is_opened');
    }
    
    return false;
};


// Update selling prices
Drupal.snasolutions.prices_update = function() {
    if ($("div.form-table-body-tab-label li#tab-auction").hasClass('active')) {
        $("input#edit-sell-price").val($("#edit-field-auction-sell-price-0-value").val());
    }
    else if ($("div.form-table-body-tab-label li#tab-fixedprice").hasClass('active')) {
        $("input#edit-sell-price").val($("#edit-field-fixed-sell-price-0-value").val());
    }
};

// Handling listing type
Drupal.snasolutions.listing_types = function() {
    // For first load, get the default selection then show the group field accordingly
    $("input.form-radio[name='listing_type']").each(function() {
        if (this.checked) {
            if ($(this).val() == 'Standard Auction') {
                $("fieldset.auction_group").fadeIn();
            }
            else if ($(this).val() == 'Standard Fixed Price') {
                $("fieldset.fixedprice_group").fadeIn();
            }
        }
    });

    $("input.form-radio[name='listing_type']").click(function() {
        if ($(this).val() == 'Standard Auction') {
            $("fieldset.fixedprice_group").css('display', 'none');
            $("fieldset.auction_group").fadeIn();
        }
        else if ($(this).val() == 'Standard Fixed Price') {
            $("fieldset.auction_group").css('display', 'none');
            $("fieldset.fixedprice_group").fadeIn();
        }
    });
};

// Handling shipping methods
Drupal.snasolutions.shipping_methods = function() {
    // For first load, get the default selection then show the group field accordingly
    $("input.form-radio[name='shipping_method']").each(function() {
        if (this.checked) {
            if ($(this).val() == 'Domestic Shipping') {
                $("fieldset.domestic_shipping_group").fadeIn();
            }
            else if ($(this).val() == 'International Shipping') {
                $("fieldset.international_shipping_group").fadeIn();
            }
        }
    });

    // Switch between tabs
    $("input.form-radio[name='shipping_method']").click(function() {
        if ($(this).val() == 'Domestic Shipping') {
            $("fieldset.international_shipping_group").css('display', 'none');
            $("fieldset.domestic_shipping_group").fadeIn();
        }
        else if ($(this).val() == 'International Shipping') {
            $("fieldset.domestic_shipping_group").css('display', 'none');
            $("fieldset.international_shipping_group").fadeIn();
        }
    });
    
    // Add more item buttons
    $("input.shipping_add_more").click(function() {
        // Get field index
        var item_index = $(this).parents('fieldset:first').find('.item').length;
        
        // Clone the group then add back
        var item = $(this).parent().prev().clone(true);
        $(this).parent().prev().after(item);

        // Update field array with indexes
        if ($(item).hasClass('domestic_shipping_item')) {
            var domestic_type = $(item).find("select[name='domestic_type["+ (item_index - 1) +"]']");
            $(domestic_type).attr('name', 'domestic_type['+ item_index +']');

            var domestic_cost = $(item).find("input[name='domestic_cost["+ (item_index - 1) +"]']");
            $(domestic_cost).attr('name', 'domestic_cost['+ item_index +']');

            var domestic_surcharge = $(item).find("input[name='domestic_surcharge["+ (item_index - 1) +"]']");
            $(domestic_surcharge).attr('name', 'domestic_surcharge['+ item_index +']');

            var domestic_free = $(item).find("input[name='domestic_free["+ (item_index - 1) +"]']");
            $(domestic_free).attr('name', 'domestic_free['+ item_index +']');
        }
        else if ($(item).hasClass('international_shipping_item')) {

            var international_type = $(item).find("select[name='international_type["+ (item_index - 1) +"]']");
            $(international_type).attr('name', 'international_type['+ item_index +']');

            var international_cost = $(item).find("input[name='international_cost["+ (item_index - 1) +"]']");
            $(international_cost).attr('name', 'international_cost['+ item_index +']');

            var international_location = $(item).find("select[name='international_location["+ (item_index - 1) +"]']");
            $(international_location).attr('name', 'international_location['+ item_index +']');

            var international_free = $(item).find("input[name='international_free["+ (item_index - 1) +"]']");
            $(international_free).attr('name', 'international_free['+ item_index +']');
        }
    });
};


// Handling tabs
Drupal.snasolutions.tabs = function() {
    $(".tab-label").click(function() {
        if ($(this).hasClass('active')) {
            return false;
        }
        
        // Hide all other, Show the current
        $(".tab-label.active").removeClass('active');
        $(".tab-content.active").removeClass('active');
        $(this).addClass('active');
        $(".tab-content."+ $(this).attr("id")).addClass('active');
        
        return false;
    });
};
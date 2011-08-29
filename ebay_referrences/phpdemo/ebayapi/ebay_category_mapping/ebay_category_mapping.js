/**
 * @file: ebay_category_mapping.js 2010/11/09 tuan $
 */

/*
 * Define our ebay_category_mapping js object namespace
 */
Drupal.ebay_category_mapping = {};

/*
 * Initiate our functions to be executed by Drupal js
 */
Drupal.behaviors.ebay_category_mapping_init = function() {
    // Process to build categories tree initiately
    
    if (!parseInt($("#category_id").val())) {
        Drupal.ebay_category_mapping.build_category_selector(-1);
    }else{
        Drupal.ebay_category_mapping.build_category_modifier($("#category_id").val());
    }
    
    // Handle when "Continue" is clicked
    Drupal.ebay_category_mapping.continue_button();
};

// Make a recusive function to process calling categories
Drupal.ebay_category_mapping.build_category_by_ids = function(current_category_id, category_list) {
    // Get position of the category id in the list
    var pos = $.inArray(current_category_id, category_list);
    var next_pos = pos + 1;
    if (pos < 0) return;

    // Process to make a call, assumming the category id is always valid
    $.post('/ebay_category_mapping/build_category/'+ current_category_id, {}, function(output) {
        if (output['status'] == 'success') {
            var added_object;
            var holder_width = 210;

            // Detect whether it is LeafCategory; If so, mean finishing the selection
            if (output['list'][current_category_id] && output['list'][current_category_id]['LeafCategory'] == 'true') {
                // Create a div object to hold "success" content
                var div = document.createElement('div');
                div.className = 'category_success';
                $(div).html("<img src='"+ path_to_ebay_category_mapping +"/images/icon_success_32_32.gif' align='left' border='0'><span>You've selected a category. Click <b>Continue</b>.</span>");
                
                // Update current level
                var current_level = output['list'][current_category_id]['CategoryLevel'];
                $("#category_level").val(current_level);
                
                // Update category id
                $("#category_id").val(current_category_id);
                
                added_object = div;
            }
            else {
            
                // Create a selection object
                var select = document.createElement('select');
                select.size = 15;

                for (key in output['list']) {
                    if (key == current_category_id) {
                        // Set parent id of this children
                        select.id = 'category_parent_'+ key;
                    
                        // Level of the children, current lowest level
                        var current_level = output['list'][key]['CategoryLevel'] + 1;
                        select.className = 'level_'+ current_level;
                        $("#category_level").val(current_level);
                        
                        // Skip the process below
                        continue;
                    }
                
                    var name = output['list'][key]['CategoryName'];
                    if (output['list'][key]['LeafCategory'] == 'false') {
                        name += ' > ';
                    }
                
                    var option = new Option(name, key);
                    if (key == category_list[next_pos]) {
                        option.selected = true; // Select this key as it is
                    }
                
                    select.options[select.length] = option;
                }// For
            
                // Add event handler
                select.onchange = function() {
                    // Remove current categories / content if neccessary
                    var selected_level = parseInt(this.className.replace(/^level_/, ''));
                    var current_level = parseInt($("#category_level").val());
                    
                    for (var i = selected_level + 1; i <= current_level; i++) {
                        $("#ebay_category_select_holder select.level_"+ i).remove();
                        $("#ebay_category_select_holder").width($("#ebay_category_select_holder").width() - holder_width);
                        $("#ebay_category_holder").scrollLeft($("#ebay_category_select_holder").width());
                    }
                    
                    // Remove "success" message if any
                    if ($("div.category_success").length) {
                        $("div.category_success").remove();
                        $("#ebay_category_select_holder").width($("#ebay_category_select_holder").width() - holder_width);
                        $("#ebay_category_holder").scrollLeft($("#ebay_category_select_holder").width());
                    }
                    
                    // Update new changes
                    Drupal.ebay_category_mapping.build_category_selector(this.value);
                };
                
                added_object = select;
            
            }
                
            // Append to the holder (expand the holder as well)
            $("#ebay_category_select_holder").width($("#ebay_category_select_holder").width() + holder_width);
            $("#ebay_category_holder").scrollLeft($("#ebay_category_select_holder").width());
            $("#ebay_category_holder #ebay_category_select_holder").append(added_object);
                        
        }// endif
        
        // Process the next if possible
        Drupal.ebay_category_mapping.build_category_by_ids(category_list[next_pos], category_list);
        
    }, 'json');

};

// Build category in edit mode where a category id does exist
Drupal.ebay_category_mapping.build_category_modifier = function(category_id) {
    // Call this to get all its categories parent
    $.post('/ebay_category_mapping/build_category/'+ category_id, {}, function(output) {
        if (output['status'] == 'success') {
            var category_ids = output['list'][category_id]['CategoryIDPath'].split(/\:/);
            
            // Add root category id into begining of the array as always needed
            category_ids.unshift(-1);
            
            // For each category, we will get its info from eBay API call then update to the categories table tree
            Drupal.ebay_category_mapping.build_category_by_ids(category_ids[0], category_ids);
            
        }else{
            // No children at all, mark as "built" already
            alert("There are no categories found!");
        }
    }, 'json');
};

// Event when continue is clicked
Drupal.ebay_category_mapping.continue_button = function() {
    $("#category_continue").click(function() {
        if (!$("#category_id").val()) {
            alert("You have not selected a category to list your product!");
            return false;
        }
    
        if ($("#nid").val() == 'new') {
            var content_type = 'product-clothing';        
            window.location.href = '/node/add/'+ content_type +'/'+ $("#category_id").val();
            
        }else{
            window.location.href = '/node/'+ $("#nid").val() +'/edit/'+ $("#category_id").val();
        }
    
        return false;
    });
};

// Build category selector
Drupal.ebay_category_mapping.build_category_selector = function(category_id) {
    // Make request to eBay for categories
    $.post('/ebay_category_mapping/build_category/'+ category_id, {}, function(output) {
        if (output['status'] == 'success') {
            var added_object;
            var holder_width = 210;

            // Detect whether it is LeafCategory; If so, mean finishing the selection
            if (output['list'][category_id] && output['list'][category_id]['LeafCategory'] == 'true') {
                // Create a div object to hold "success" content
                var div = document.createElement('div');
                div.className = 'category_success';
                $(div).html("<img src='"+ path_to_ebay_category_mapping +"/images/icon_success_32_32.gif' align='left' border='0'><span>You've selected a category. Click <b>Continue</b>.</span>");
                
                // Update current level
                var current_level = output['list'][category_id]['CategoryLevel'];
                $("#category_level").val(current_level);
                
                // Update category id
                $("#category_id").val(category_id);
                
                added_object = div;
            }
            else {
                // Create a selection object
                var select = document.createElement('select');
                select.size = 15;
        
                for (key in output['list']) {
                    if (key == category_id) {
                        // Set parent id of this children
                        select.id = 'category_parent_'+ key;
                    
                        // Level of the children, current lowest level
                        var current_level = parseInt(output['list'][key]['CategoryLevel']) + 1;
                        select.className = 'level_'+ current_level;
                        $("#category_level").val(current_level);
                        
                        // Skip the process below
                        continue;
                    }
                    
                    var name = output['list'][key]['CategoryName'];
                    if (output['list'][key]['LeafCategory'] == 'false') {
                        name += ' > ';
                    }
                
                    select.options[select.length]=new Option(name, key);
                }
            
                // Add event handler
                select.onchange = function() {
                    // Remove current categories / content if neccessary
                    var selected_level = parseInt(this.className.replace(/^level_/, ''));
                    var current_level = parseInt($("#category_level").val());
                    
                    for (var i = selected_level + 1; i <= current_level; i++) {
                        $("#ebay_category_select_holder select.level_"+ i).remove();
                        $("#ebay_category_select_holder").width($("#ebay_category_select_holder").width() - holder_width);
                        $("#ebay_category_holder").scrollLeft($("#ebay_category_select_holder").width());
                    }
                    
                    // Remove "success" message if any
                    if ($("div.category_success").length) {
                        $("div.category_success").remove();
                        $("#ebay_category_select_holder").width($("#ebay_category_select_holder").width() - holder_width);
                        $("#ebay_category_holder").scrollLeft($("#ebay_category_select_holder").width());
                    }
                    
                    // Update new changes
                    Drupal.ebay_category_mapping.build_category_selector(this.value);
                };
                
                added_object = select;
            }
            
            // Append to the holder (expand the holder as well)
            $("#ebay_category_select_holder").width($("#ebay_category_select_holder").width() + holder_width);
            $("#ebay_category_holder").scrollLeft($("#ebay_category_select_holder").width());
            $("#ebay_category_holder #ebay_category_select_holder").append(added_object);
            
        }else{
            // No children at all, mark as "built" already
            alert("There are no categories found!");
        }
    }, 'json');
    
};

// Get category from Drupal taxonomy and update to the tree
Drupal.ebay_category_mapping.get_taxonomy = function(obj, vid, parent) {
    // Make request to eBay for categories
    $.post('/ebay_category_mapping/build_taxonomy/'+ vid +'/'+ parent, {}, function(output) {
        if (output['status'] == 'success') {
            // Process to update the tree under the parent id 'parent' (tid)
            if (parent == 0) {
                parent = vid;
            }
            
            var parent_label = $(obj); // hold all classes and handler
            var parent_id = $(parent_label).parent().attr("id"); // holder the sub folder id only
            
            // Add subfolder for new fetched children
            $(parent_label).parent().append('<ul>');
            
            for (key in output['list']) {
                $("#"+ parent_id +" ul").append('<li id="folder_'+ output['list'][key]['tid'] +'" class="level_'+ output['list'][key]['depth'] +'"><p id="'+ output['list'][key]['tid'] +'" class="not_built is_closed li_row_label" onclick="Drupal.ebay_category_mapping.drupal_tree_click(this, '+ output['list'][key]['vid'] +', '+ output['list'][key]['tid'] +');">'+ output['list'][key]['name'] +'</p></li>');
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
            var parent_label = $(obj); // hold all classes and handler
            $(parent_label).removeClass('not_built');
            $(parent_label).addClass('no_children');
        
        }
    }, 'json');
};

// Get category from eBay and update to the tree
Drupal.ebay_category_mapping.get_category = function(category_id) {
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
                    $("#"+ parent_id +" ul").append('<li id="folder_'+ key +'" class="level_'+ output['list'][key]['CategoryLevel'] +' leaf_'+ output['list'][key]['LeafCategory'] +'"><p id="'+ key +'" class="not_built is_closed li_row_label" onclick="Drupal.ebay_category_mapping.tree_click(this);">'+ output['list'][key]['CategoryName'] +'</p></li>');
                }
                else{
                    $("#"+ parent_id +" ul").append('<li id="cid_'+ key +'" class="level_'+ output['list'][key]['CategoryLevel'] +' leaf_'+ output['list'][key]['LeafCategory'] +'"><p class="li_row_label">'+ output['list'][key]['CategoryName'] +'</p></li>');
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
Drupal.ebay_category_mapping.tree_click = function(obj) {
    if ($(obj).hasClass('is_closed')) {
        if ($(obj).hasClass('not_built')) {
            Drupal.ebay_category_mapping.get_category($(obj).attr("id"));
            
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

// Handle click on the drupal tree
Drupal.ebay_category_mapping.drupal_tree_click = function(obj, vid, parent) {
    if ($(obj).hasClass('is_closed')) {
        if ($(obj).hasClass('not_built')) {
            Drupal.ebay_category_mapping.get_taxonomy(obj, vid, parent);
            
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
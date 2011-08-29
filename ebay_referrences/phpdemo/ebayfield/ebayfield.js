/**
 * @file: ebayfield.js 2010/12/08 - tuan $
 */

// Define our ebayfield js object namespace
Drupal.ebayfield = {};

/*
 * Initiate our functions to be executed by Drupal js
 */
Drupal.behaviors.ebayfield_init = function() {
    Drupal.ebayfield.select_list_add_last_child();
    Drupal.ebayfield.textfield_overlay();
};

// Add a class to detect last-child for the select list
Drupal.ebayfield.select_list_add_last_child = function() {
    $("select.ebayfield_widget_list").each(function() {
        $(this.options[this.options.length-1]).addClass('last-child');
    });
}

// Create an input textfield placed overlay the Select list for user to input new item
Drupal.ebayfield.textfield_overlay = function() {
    $("select.ebayfield_widget_list").change(function() {
        // Add new item is always placed at the end of the list
        if (this.selectedIndex == this.options.length - 1) {
            // Remove if textfield does exist
            if (this.textfield) {
                $(this.textfield).remove();
            }
            
            this.textfield = document.createElement('input');
            this.textfield.type = 'text';
            this.textfield.id = 'ebayfield_add_new_item';            
            $(this.textfield).css({
                width: (this.offsetWidth - 4) +'px',
                position: 'absolute',
                display: 'none'
            });
            
            // If there is a valid value for the "new" item field, get it into the textfield
            if (this.options[this.selectedIndex].text != 'Add new item') {
            //if ($(this).val() != 'Add new item') {
                this.textfield.value = $(this).val();
            }
            
            // Set event when user leaves the textfield
            var select = this;
            $(this.textfield).blur(function() {
                if ($(this).val()) {
                    // Check though the existing list, auto select if found
                    var found = false;
                    for (var i=1; i<select.options.length; i++) {
                        if ($(this).val().toLowerCase() == select.options[i].text.toLowerCase()) {
                            $(select).val(select.options[i].value);
                            found = true;
                            break;
                        }
                    }
                    
                    // Set new item into the list
                    if (!found) {
                        //select.options[select.options.length-1].value = $(this).val();
                        select.options[select.options.length-1].text = $(this).val();
                        
                        // Auto select the new item
                        select.options.selectedIndex = select.options.length-1;
                    }
                }
                else{
                    // There is no value entered, set to default
                    $(select).val("");
                }
                
                // Hide the textfield
                $(this).fadeOut(function() {
                    // Update the selected item to hidden input field
                    var textfield = document.getElementById($(select).attr("id").replace(/\-list$/, '-value'));
                    if (!textfield) {
                        alert("Input field not found!");
                        return false;
                    }
                    
                    textfield.value = select.options[select.options.length-1].text;
                });
            });
  
            // Place the textfield overlay the select list
            $(this).before(this.textfield);
            
            // Show the field
            $(this.textfield).fadeIn(function() {
                this.focus();
            });
        }
        else{
            // Update the selected item to hidden input field
            var textfield = document.getElementById($(this).attr("id").replace(/\-list$/, '-value'));
            if (!textfield) {
                alert("Input field not found!");
                return false;
            }
        
            textfield.value = $(this).val();
        }
                
    });
};
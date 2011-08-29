<?php
// @created on 16 May 2011 - tuan

// Remove some fields
unset($form['buttons']['preview']);
unset($form['buttons']['save_continue']);
?>

<div id="tabs_holder" class="clear-block">
    <ul>
        <li id="tab_details" class="tab-label active">Details</li>
        <li id="tab_images" class="tab-label">Images</li>
        <li id="tab_inventory" class="tab-label">Inventory</li>
        <li id="tab_customfield" class="tab-label">Custom Fields</li>
    </ul>
</div>

<div id="content_holder" class="clear-block">
    <div class="tab-content tab_details active">
        <fieldset class="catalog">
            <legend><?php echo t('Catalog Information'); ?></legend>
            <div class="row"><?php print drupal_render($form['field_product_type']); ?></div>
            <div class="row"><?php print drupal_render($form['title']); ?></div>
            <div class="row"><?php print drupal_render($form['base']['model']); ?></div>
            <div class="row">
                <?php print drupal_render($form['ebay_category']); ?>
            </div>
        </fieldset>

        <fieldset class="description">
            <legend><?php echo t('Product Description'); ?></legend>
            <div class="row"><?php print drupal_render($form['body_field']); ?></div>
        </fieldset>

        <fieldset class="pricing">
            <legend><?php echo t('Pricing & Pre-Order Options'); ?></legend>
            <div class="row"><?php print drupal_render($form['field_product_availability']); ?></div>
            <div class="row"><?php print drupal_render($form['base']['prices']['sell_price']); ?></div>
        </fieldset>

        <fieldset class="shipping">
            <legend><?php echo t('Shipping Details'); ?></legend>
            <div class="row"><?php print drupal_render($form['base']['weight']); ?></div>
            <div class="row"><?php print drupal_render($form['base']['dimensions']); ?></div>
        </fieldset>
    </div>
    
    <div class="tab-content tab_images">
        <fieldset class="images">
            <legend><?php echo t('Product Images'); ?></legend>
            <div class="row"><?php print drupal_render($form['field_image_cache']); ?></div>
        </fieldset>
    </div>
    
    <div class="tab-content tab_inventory">
        <fieldset class="inventory">
            <legend><?php echo t('Inventory Tracking'); ?></legend>
            <div class="row"><?php print drupal_render($form['field_tracking_method']); ?></div>
        </fieldset>
    </div>
    
    <div class="tab-content tab_customfield">
        <div class="row notify">
            Please note that some item specifics are pre-defined by eBay so please do <?php echo l('checking', 'ebay_listing/category_specifics', array('attributes' => array('target' => '_blank'))); ?> to make sure values are matched.
        </div>
        <div class="row"><?php print drupal_render($form['group_customfield']); ?></div>
    </div>
</div>

<div id="buttons_holder" class="clear-block">
    <?php print drupal_render($form['buttons']); ?>
</div>

<div id="hidden_fields">
    <?php print drupal_render($form); ?>
</div>
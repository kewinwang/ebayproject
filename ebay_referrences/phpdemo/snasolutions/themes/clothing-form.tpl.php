<?php

/**
 * TPL for clothing products form
 * @created: 16 Nov 2010
 * @updated: 16 Nov 2010
 * @author: Tuan
 */ 
?>

<?php    
// Get the category info based on the id
$category_id = 'new';
if (arg(2) == 'edit') {
    // In edit mode, when category is changed
    if (is_numeric(arg(3))) {
        $form['field_category_id'][0]['value']['#default_value'] = arg(3);
        $form['field_category_id'][0]['value']['#value'] = arg(3);
    }
    
    // In edit mode, when loading category from stored value
    if (is_numeric($form['field_category_id'][0]['value']['#value'])) {
        $category = ebayapi_getcategoryinfo($form['field_category_id'][0]['value']['#value']);
        if ($category->CategoryArray->Category->CategoryNamePath) {
            $category_id = $form['field_category_id'][0]['value']['#value'];
        }
    }
}
else{
    if (is_numeric(arg(3))) {
        // In new mode, just get from redirected URL
        $category = ebayapi_getcategoryinfo(arg(3));
        if ($category->CategoryArray->Category->CategoryNamePath) {
            $category_id = arg(3);
            $form['field_category_id'][0]['value']['#default_value'] = $category_id;
            $form['field_category_id'][0]['value']['#value'] = $category_id;
        }
    }
}

$nid = 'new';
if (is_numeric(arg(1))) {
    $nid = arg(1);
}

?>

<div id="clothing-form">
    <div class="form-table">
        <div class="form-table-header">
            <?php print t('Categories where your listing will appear');?>
        </div>
        
        <div class="form-table-body">
            <div class="form-table-body-row">
                <div class="form-table-body-row-cell">
                    <?php if ((string)$category->CategoryArray->Category->CategoryNamePath): ?>
                        <?php print preg_replace('/\:/', ' > ', (string)$category->CategoryArray->Category->CategoryNamePath); ?>
                        <br/>
                        [<a href="/create/product_category/<?php echo $category_id; ?>/<?php echo $nid;?>">Change category</a>]
                    <?php else: ?>
                        <?php echo t('You haven\'t selected a category. Please '. l('click here', 'create/product_category/'. $category_id .'/'. $nid) .' to add one.'); ?>
                    <?php endif; ?>
                    
                    <div style="display:none" id="category_id">
                        <?php print drupal_render($form['field_category_id']); ?>
                    </div>
                </div>
            </div>
        </div>
    </div>

    <div class="form-table">
        <div class="form-table-header">
            <?php print t('Create and Describe your item');?>
        </div>
        
        <div class="form-table-body">
            <div class="form-table-body-row">
                <div class="form-table-body-column float-left">
                    <div class="form-table-body-row-cell">
                        <?php print drupal_render($form['base']['model']); ?>
                    </div>
                    <div class="form-table-body-row-cell">
                        <?php print drupal_render($form['field_sdesc_cn']); ?>
                    </div>
                    <div class="form-table-body-row-cell">
                        <?php print drupal_render($form['field_material']); ?>
                    </div>
                    <div class="form-table-body-row-cell">
                        <?php print drupal_render($form['field_design_style']); ?>
                    </div>
                </div>

                <div class="form-table-body-column float-right">
                    <div class="form-table-body-row-cell">
                        <?php print drupal_render($form['field_condition']); ?>
                    </div>
                    <div class="form-table-body-row-cell">
                        <?php print drupal_render($form['field_occasion']); ?>
                    </div>
                    <div class="form-table-body-row-cell">
                        <?php print drupal_render($form['field_color']); ?>
                    </div>
                    <div class="form-table-body-row-cell">
                        <?php print drupal_render($form['field_pattern']); ?>
                    </div>
                </div>
            </div>
        </div>
    </div>

    <div class="form-table">
        <div class="form-table-header">
            <?php print t('Manage your item');?>
        </div>
      
        <div class="form-table-body">
            <div class="form-table-body-row">
                <div class="form-table-body-column float-left">
                    <div class="form-table-body-row-cell">
                        <?php print drupal_render($form['base']['prices']['cost']); ?>
                    </div>
                    <div class="form-table-body-row-cell">
                        <?php print drupal_render($form['field_weight']); ?>
                    </div>
                    <div class="form-table-body-row-cell">
                        <?php print drupal_render($form['field_location']); ?>
                    </div>
                </div>

                <div class="form-table-body-column float-right">
                    <div class="form-table-body-row-cell">
                        <?php print drupal_render($form['field_quantity']); ?>
                    </div>
                    <div class="form-table-body-row-cell">
                        <?php print drupal_render($form['field_reorder_level']); ?>
                    </div>
                    <div class="form-table-body-row-cell">
                        <?php print drupal_render($form['field_remark']); ?>
                    </div>
                </div>
            </div>
        </div>
    </div>
    
    <div class="form-table">
        <div class="form-table-header">
            <?php print t('Bring your item to life with pictures');?>
        </div>
        
        <div class="form-table-body">
            <div class="form-table-body-row">
                <div class="form-table-body-row-cell">
                    <?php print drupal_render($form['field_image_cache']); ?>
                </div>
            </div>
        </div>
    </div>

    <div class="form-table">
        <div class="form-table-header">
            <?php print t('Help buyers fund your item with a great title');?>
        </div>
        
        <div class="form-table-body">
            <div class="form-table-body-row">
                <div class="form-table-body-row-cell">
                    <?php print drupal_render($form['title']); ?>
                </div>
            </div>

            <div class="form-table-body-row">
                <div class="form-table-body-row-cell">
                    <?php print drupal_render($form['item_specifics']); ?>
                </div>
            </div>
        </div>
    </div>

    <div class="form-table">
        <div class="form-table-header">
            <?php print t('Which Market is your Auction or Fixed Price');?>
        </div>
        
        <div class="form-table-body">
            <div class="form-table-body-row">
                <div class="form-table-body-row-cell">
                    <?php print drupal_render($form['field_listing_type']); ?>
                </div>
            </div>

            <div class="form-table-body-row">
                <div class="form-table-body-row-cell">
                    <?php print drupal_render($form['field_market']); ?>
                </div>
            </div>
        </div>
    </div>
    
    <div class="form-table">
        <div class="form-table-header">
            <?php print t('Describe the item you are selling');?>
        </div>
        
        <div class="form-table-body">
            <div class="form-table-body-row">
                <div class="form-table-body-row-cell">
                    <?php print drupal_render($form['body_field']); ?>
                </div>
            </div>        
        </div>
    </div>

    <div class="form-table">
        <div class="form-table-header">
            <?php print t('Choose how you\'d like to sell your item');?>
        </div>
        
        <div class="form-table-body">
            <div class="form-table-body-row">
                <div class="form-table-body-row-cell">
                    <div class="form-table-body-tab">
                        <div class="form-table-body-tab-label">
                            <ul>
                                <li id="tab-auction" class="active"><?php echo t('Online Auction'); ?></li>
                                <li id="tab-fixedprice"><?php echo t('Fixed Price'); ?></li>
                            </ul>
                        </div>
                    
                        <div class="form-table-body-tab-body">
                            <div class="tab-body tab-auction active">
                                <?php print drupal_render($form['group_auction_price']); ?>
                            </div>
                        
                            <div class="tab-body tab-fixedprice">
                                <?php print drupal_render($form['group_fixed_price']); ?>
                            </div>
                        </div>
                    </div>
                </div>                
            </div>            
        </div>        
    </div>

    <div class="form-table">
        <div class="form-table-header">
            <?php print t('Decide how you\'d like to be paid');?>
        </div>
        
        <div class="form-table-body">
            <div class="form-table-body-row">
                <div class="form-table-body-row-cell">
                    <?php print drupal_render($form['field_paypal_account_id']); ?>
                </div>
            </div>        
        </div>
    </div>
    
    <div class="form-table">
        <div class="form-table-header">
            <?php print t('Give buyers shipping details');?>
        </div>
        
        <div class="form-table-body">
            <div class="form-table-body-row">
                <div class="form-table-body-row-cell">
                    <?php print drupal_render($form['group_domestic_shipping']); ?>
                </div>
            </div>

            <div class="form-table-body-row">
                <div class="form-table-body-row-cell">
                    <?php print drupal_render($form['group_international_shipping']); ?>
                </div>
            </div>

            <div class="form-table-body-row">
                <div class="form-table-body-row-cell">
                    <?php print drupal_render($form['field_handling_time']); ?>
                </div>
            </div>
        </div>
    </div>

    <div class="form-table">
        <div class="form-table-header">
            <?php print t('Other things you\'d like buyers to know');?>
        </div>
        
        <div class="form-table-body">
            <div class="form-table-body-row">
                <div class="form-table-body-row-cell">
                    <?php print drupal_render($form['field_return_policy']); ?>
                </div>
            </div>        
        </div>
    </div>

    <div class="form-table">
        <div class="form-table-header">
            <?php print t('Schedule');?>
        </div>
        
        <div class="form-table-body">
            <div class="form-table-body-row">
                <div class="form-table-body-row-cell">
                    <?php print drupal_render($form['field_schedule']); ?>
                </div>
            </div>        
        </div>
    </div>
   
    <div class="form-buttons">
        <?php print drupal_render($form); ?>
    </div>
</div>

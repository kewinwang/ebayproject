<?php
/**
 * TPL for user register form
 * @created: 14 Oct 2010
 * @updated: 14 Oct 2010
 * @author: Tuan
 */ 
?>
<?php    
    // Form alter
    $form['pass']['pass1']['#title'] = 'Desired password';
    $form['pass']['pass2']['#title'] = 'Retype password';
    $form['pass']['pass1']['#size'] = 60;
    $form['pass']['pass2']['#size'] = 60;
    $form['name']['#title'] = 'Desired username';
    $form['mail']['#title'] = 'Email address';
    $form['name']['#attributes'] = array('class'=>'user-register-textfield');
    $form['mail']['#attributes'] = array('class'=>'user-register-textfield');
    
    $form['mail']['#weight'] = 8;
    $form['field_ebay_user_id']['#weight'] = 9;
    $form['field_ebay_user_id'][0]['#weight'] = 9;

//echo '<pre>';
//var_dump($form['field_ebay_user_id']);
//echo '</pre>';
//die("");
    
?>
<div id="user-profile-form">
    <div class="user-profile-row">
        <?php print drupal_render($form['name']); ?>
    </div>
    <div class="user-profile-row">
        <?php print drupal_render($form['pass']); ?>
    </div>
    <div class="user-profile-row">
        <?php print drupal_render($form['title']); ?>
    </div>
    <div class="user-profile-row">
        <?php print drupal_render($form['mail']); ?>
    </div>
    <div class="user-profile-row">
        <?php print drupal_render($form['field_ebay_user_id']); ?>
    </div>
    
    <?php print drupal_render($form); ?>
</div>

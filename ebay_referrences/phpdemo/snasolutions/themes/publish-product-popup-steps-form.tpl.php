<?php
// @created on 10 June 2011 - tuan
//dsm($form);
?>
<div id="top-content">
    <div class="title float-left">List <?php echo $form['total_products']['#value']; ?> products on eBay (Step <?php echo $form['step']['#value']; ?> of <?php echo $form['total_steps']['#value']; ?>)</div>
    <div class="close-button float-right"><a href="javascript:parent.Drupal.simple_popup.close();">Close</a></div>
</div>

<div id="content_holder">

    <?php if ($form['step']['#value'] == 1): ?>
    <div class="row"><?php print drupal_render($form['notify']); ?></div>
    <!--<div class="row"><?php //print drupal_render($form['themeid']); ?></div>-->
    <div class="row"><?php print drupal_render($form['templateid']); ?></div>

    <?php elseif ($form['step']['#value'] == 2): ?>
    <div class="row"><?php print drupal_render($form['notify']); ?></div>
    <div class="row"><?php print drupal_render($form['category_features']); ?></div>

    <?php elseif ($form['step']['#value'] == 3): ?>
    <div class="row"><?php print drupal_render($form['notify']); ?></div>
    <div class="row"><?php print drupal_render($form['estimated_fees']); ?></div>

    <?php elseif ($form['step']['#value'] == 4): ?>
    <div class="row"><?php print drupal_render($form['notify']); ?></div>
    <?php endif; ?>
</div>

<?php if ($form['cancel'] or $form['back'] or $form['next']): ?>
<div id="buttons_holder" class="clear-block">
    <div class="column"><?php print drupal_render($form['cancel']); ?></div>
    <?php if ($form['back']): ?>
    <div class="column"><?php print drupal_render($form['back']); ?></div>
    <?php endif; ?>
    <?php if ($form['next']): ?>
    <div class="column"><?php print drupal_render($form['next']); ?></div>
    <?php endif; ?>
</div>
<?php endif; ?>

<div id="hidden_fields">
    <?php print drupal_render($form); ?>
</div>
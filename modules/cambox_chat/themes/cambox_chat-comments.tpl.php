<div id="comments-<?php print $type;?>" class="comments-wrapper">
<?php if (isset($form)):?>
  <div id="comment-form">
    <?php print $form;?>
  </div>
<?php endif;?>
  <div class="comments">
    <?php print $comments;?>
  </div>
</div>
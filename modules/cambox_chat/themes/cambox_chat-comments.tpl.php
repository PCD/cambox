<div id="comments-<?php print $type;?>">
  <div class="comments">
    <?php print $comments;?>
  </div>
<?php if (isset($form)):?>
  <div id="comment-form">
    <?php print $form;?>
  </div>
<?php endif;?>
</div>
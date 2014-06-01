<div id="comment-<?php print $cid;?>" data-cid="<?php print $cid;?>" class="comment <?php print $zebra;?> <?php print $class;?>">
  <div class="image">
    <img src="<?php print $user_picture;?>" />
  </div>
  <div class="info">
    <div class="submitted">
      <span class="author"><?php print $user_name;?></span> 
<?php if (isset($time) && !empty($time)):?>
      <span class="time"><?php print $time;?></span>
<?php endif;?>
<?php if (isset($unpublished_text) && !empty($unpublished_text)):?>
      <span class="unpublished_text"><?php print $unpublished_text;?></span>
<?php endif;?>
    </div>
    <p><?php print $message;?></p>
  </div>
<?php if (isset($approve_link) || isset($save_link)):?>
  <ul class="actions">
    <?php if (isset($approve_link)):?>
    <li><?php print $approve_link;?></li>
    <?php endif;?>
    <?php if (isset($save_link)):?>
    <li><?php print $save_link;?></li>
    <?php endif;?>
  </ul>
<?php endif;?>
</div>

<div id="comments-<?php print $type;?>" class="comments-wrapper" data-pull-url="<?php print $data_pull_url;?>">
<?php if (isset($form) && !empty($form) ):?>
  <div id="comment-form">
    <?php print $form;?>
  </div>
<?php endif;?>
<?php if (isset($login_form) && !empty($login_form) ):?>
  <div id="login-form">
    <h2>Chat en Vivo</h2>
    <div class="content">
      <p>Para entrar al Chat inicia sesión</p>
      <?php print $login_form;?>
    </div>
  </div>
<?php endif;?>
  <div class="comments">
    <?php print $comments;?>
  </div>
</div>
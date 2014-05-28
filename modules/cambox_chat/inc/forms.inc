<?php
/**
 * Provides a Chat form.
 */
function cambox_chat_form($form, &$form_state) {
  $form = array();
  $form['#action'] = url('node/' . arg(1) . '/chat/comment');
  $form['#attributes']['data-pull-url'] = base_path() . 'node/' . arg(1) . '/chat/pull/';
  global $user;
  $cuser = user_load($user->uid);
  $picture = image_style_url('profile', $cuser->picture->uri);
  $form['image'] = array(
    '#type' => 'markup', 
    '#markup' => "<div class=\"image\"><img src=\"{$picture}\" /></div>", 
  );
  $form['message'] = array(
    '#prefix' => '<div class="info">', 
    '#type' => 'textarea', 
    '#max_length' => 150, 
    '#placeholder' => t('Type your Message'), 
    '#required' => TRUE, 
    '#rows' => 3, 
    '#resizable' => FALSE, 
  );
  $form['actions'] = array(
    '#type' => 'actions', 
    'submit' => array(
      '#type' => 'submit', 
      '#value' => t('Submit'), 
    ), 
    '#suffix' => '</div>', 
  );
  return $form;
}
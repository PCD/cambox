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
  $text_limit = variable_get('cambox-message-limit', 150);
  $time_left = variable_get('cambox-time-left', 60);
  $form['message'] = array(
    '#prefix' => '<div class="info">', 
    '#type' => 'textarea', 
    '#max_length' => $text_limit, 
    '#placeholder' => t('Type your Message'), 
    '#required' => TRUE, 
    '#rows' => 3, 
    '#resizable' => FALSE, 
  );
  $form['image'] = array(
    '#type' => 'markup', 
    '#markup' => "<div class=\"image\"><img src=\"{$picture}\" /></div>", 
  );
  $form['actions'] = array(
    '#type' => 'actions', 
    'submit' => array(
      '#type' => 'submit', 
      '#value' => 'Publicar', 
    ), 
    '#suffix' => '</div>', 
    '#weight' => 7, 
  );
  $form['chars_left'] = array(
    '#type' => 'markup', 
    '#markup' => '<div class="message-limit">' . $text_limit . '</div>', 
    '#weight' => 8, 
  );
  $form['time_left'] = array(
    '#type' => 'markup', 
    '#markup' => '<div class="time-left">Tiempo para comentar: <span class="seconds">' . $time_left . '</span></div>', 
    '#weight' => 9, 
  );
  return $form;
}
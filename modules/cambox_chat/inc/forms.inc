<?php
/**
 * Provides a Chat form.
 */
function cambox_chat_form($form, &$form_state) {
  $form = array();
  $form['#action'] = url('node/' . arg(1) . '/chat/comment');
  $text_limit = variable_get('cambox-message-limit', 150);
  $time_left = cambox_get_time_limit();
  $form['h3'] = array(
    '#type' => 'markup', 
    '#markup' => '<h3 class="title">Chat en Vivo</h3>', 
    '#weight' => 3, 
  );
  $form['message'] = array(
    '#prefix' => '<div class="info">', 
    '#type' => 'textarea', 
    '#max_length' => $text_limit, 
    '#placeholder' => t('Escribe tu Mensaje'), 
    '#required' => TRUE, 
    '#rows' => 3, 
    '#resizable' => FALSE, 
    '#attributes' => array(
      'data-limit' => $text_limit, 
      'data-time-wait' => $time_left, 
      'data-pause' => 0, 
    ), 
    '#weight' => 5, 
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
    '#markup' => '<div class="time-left"></div>', 
    '#weight' => 9, 
  );
  
  // Stuff for logged users.
  cambox_chat_form_logged($form);
  return $form;
}

/**
 * Logged Users.
 */
function cambox_chat_form_logged(&$form) {
  global $user;
  if ( $user->uid > 0 ) {
    $cuser = user_load($user->uid);
    // Picture
    $picture = image_style_url('profile', $cuser->picture->uri);
    $form['image'] = array(
      '#type' => 'markup', 
      '#markup' => "<div class=\"image\"><img src=\"{$picture}\" /></div>", 
      '#weight' => 6, 
    );
    
    // Logout Line
    $form['logout'] = array(
      '#type' => 'markup', 
      '#markup' => cambox_chat_form_logout(), 
      '#weight' => 4, 
    );
    
    // Time Left before Comment Again
    $last_comment_nid = cambox_chat_data_get_last_comment_time(arg(1), $cuser->uid);
    if ( $last_comment_nid > 0 ) {
      $time_ago = time() - $last_comment_nid;
      $time_limit = cambox_get_time_limit();
      //$time_limit = 1000;
      if ( $time_ago < $time_limit ) {
        $time_left = $time_limit - $time_ago;
        $form['message']['#placeholder'] = "Espere {$time_left} segundos para volver a comentar.";
        $form['message']['#attributes']['placeholder'] = "Espere {$time_left} segundos para volver a comentar.";
        $form['message']['#attributes']['data-pause'] = $time_left;
      }
    }
  }
}

/**
 * Chat Logout.
 */
function cambox_chat_form_logout() {
  global $user;
  $cuser = user_load($user->uid);
  $name = $cuser->field_first_name[LANGUAGE_NONE][0]['safe_value'];
  $name .= ' ' . $cuser->field_last_name[LANGUAGE_NONE][0]['safe_value'];
  $name = trim($name);
  $logout_link = l('Salir', 'user/logout', array('query'=>array('destination'=>'node/' . arg(1))));
  $output = "<div class=\"logout\">
    <div class=\"name\">{$name}</div>
    <div class=\"logout-link\">{$logout_link}</div>
  </div>";
  return $output;
}
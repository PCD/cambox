<?php
/**
 * Provides a Chat form.
 */
function cambox_chat_form($form, &$form_state) {
  $form = array();
  $form['#action'] = url('node/' . arg(1) . '/chat/comment');
  $form['#attributes']['data-pull-url'] = base_path() . 'node/' . arg(1) . '/chat/pull/';
  $form['message'] = array(
    '#type' => 'textarea', 
    '#max_length' => 150, 
    '#placeholder' => t('Type your Message'), 
    '#required' => TRUE, 
    '#resizable' => FALSE, 
  );
  $form['actions'] = array(
    '#type' => 'actions', 
    'submit' => array(
      '#type' => 'submit', 
      '#value' => t('Submit'), 
    ), 
  );
  return $form;
}
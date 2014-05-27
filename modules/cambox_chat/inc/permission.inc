<?php
/**
 * Implements hook_permission.
 */
function cambox_chat_permission() {
  return array(
    'Access Chat' => array(
      'title' => 'Access Chat', 
      'description' => 'Access Chat', 
    ), 
  );
}
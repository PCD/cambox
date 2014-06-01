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
    'Post Chat' => array(
      'title' => 'Post Chat', 
      'description' => 'Post Chat', 
    ), 
    'Approve Chat Messages' => array(
      'title' => 'Approve Chat Messages', 
      'description' => 'Approve Chat Messages', 
    ), 
    'Save Chat Messages' => array(
      'title' => 'Save Chat Messages', 
      'description' => 'Save Chat Messages', 
    ), 
  );
}
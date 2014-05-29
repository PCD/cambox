<?php
/**
 * Implements hook_block_info.
 */
function cambox_chat_block_info() {
  $blocks['chat_saved'] = array(
    'info' => t('Chat Saved'), 
  );
  $blocks['chat_approved'] = array(
    'info' => t('Chat Approved'), 
  );
  $blocks['chat_raw'] = array(
    'info' => t('Chat Raw'), 
  );
  return $blocks;
}

/**
 * Implements hook_block_view.
 */
function cambox_chat_block_view($delta = '') {
  $block = array();
  if ( !(arg(0) == 'node' && intval(arg(1)) > 0 && ($node = node_load(arg(1))) && $node->type == 'chat' && $node->status == 1) ) {
    return false;
  }
  
  switch($delta) {
    case 'chat_saved':
      break;
    
    case 'chat_approved':
    case 'chat_raw':
      if ( user_access('Access Chat') ) {
        $block['content'] = cambox_chat_block_raw($node);
      }
      break;
  }
  return $block;
}

/**
 * Returns a chatbox that has a Raw Conversation ready to be approved.
 */
function cambox_chat_block_raw($node) {
  drupal_add_css(drupal_get_path('module', 'cambox_chat') . '/css/cambox_chat.css');
  drupal_add_js(drupal_get_path('module', 'cambox_chat') . '/js/cambox_chat.js');
  $comments = cambox_chat_data_get_comments($node->nid, 20);
  return cambox_chat_theme_comments($comments, 'raw');
}
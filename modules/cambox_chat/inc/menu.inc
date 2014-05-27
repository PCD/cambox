<?php
/**
 * Implements hook_menu.
 */
function cambox_chat_menu() {
  $items = array();
  $items['node/%node/chat'] = array(
    'title' => 'Chat', 
    'page callback' => 'cambox_chat_menu_chat', 
    'page arguments' => array(1), 
    'access arguments' => array('Access Chat'), 
    'type' => MENU_CALLBACK, 
  );
  $items['node/%node/chat/comment'] = array(
    'title' => 'Chat', 
    'page callback' => 'cambox_chat_menu_post_comment', 
    'page arguments' => array(1), 
    'access arguments' => array('Access Chat'), 
    'type' => MENU_CALLBACK, 
  );
  $items['node/%node/chat/pull/%'] = array(
    'title' => 'Chat', 
    'page callback' => 'cambox_chat_menu_pull_comment', 
    'page arguments' => array(1, 4), 
    'access arguments' => array('Access Chat'), 
    'type' => MENU_CALLBACK, 
  );
  return $items;
}

/**
 * Provides a Chat Page.
 */
function cambox_chat_menu_chat($node) {
  drupal_add_css(drupal_get_path('module', 'cambox_chat') . '/css/cambox_chat.css');
  drupal_add_js(drupal_get_path('module', 'cambox_chat') . '/js/cambox_chat.js');
  $raw_chat = cambox_chat_block_raw($node);
  return $raw_chat;
}

/**
 * Post Comment.
 */
function cambox_chat_menu_post_comment($node) {
  global $user;
  $comment = new stdClass();
  $comment->uid = $user->uid;
  $comment->name = $user->name;
  $comment->mail = $user->mail;
  $comment->nid = $node->nid;
  $comment->pid = 0;
  $comment->cid = 0;
  $comment->status = 0;
  $comment->is_anonymous = 0;
  $comment->subject = '';
  $comment->language = LANGUAGE_NONE;
  $comment->comment_body[LANGUAGE_NONE][0]['value'] = $_POST['message'];
  $comment->comment_body[LANGUAGE_NONE][0]['format'] = 'filtered_html';
  $comment = comment_submit($comment);
  comment_save($comment);
  
  // Is Ajax
  if(!empty($_SERVER['HTTP_X_REQUESTED_WITH']) && strtolower($_SERVER['HTTP_X_REQUESTED_WITH']) == 'xmlhttprequest') {
    $response = array(
      'valid' => 1, 
      'comment' => cambox_chat_theme_comments_item_ajax($comment), 
    );
    print drupal_json_output($response);
    exit;
  } else {
    drupal_set_message(t('Your message has been posted.'));
    drupal_goto('node/' . $node->nid . '/chat');
  }
}

/**
 * Pull new messages.
 */
function cambox_chat_menu_pull_comment($node, $last_cid) {
  $response = array(
    'valid' => 1, 
    'comments' => '', 
    'count' => 0, 
  );
  $start = time();
  $time_limit = 30;
  $time_pause = 2;
  $time_spent = 0;
  while(($time_spent<$time_limit)) {
    // Is there any more Comments?
    $comments = cambox_chat_data_get_comments($node->nid, 100, $last_cid);
    if ( !empty($comments) ) {
      break;
    }
    
    // Validate Time again, for saving an extra second.
    $time_spent = time() - $start;
    if ( $time_spent < $time_limit ) {
      sleep($time_pause);
    } else {
      break;
    }
  }
  
  // Check No Comments
  if ( empty($comments) ) {
    print drupal_json_output($response);
    exit;
  }
  
  // We have comments, lets style them.
  $response['comments'] = cambox_chat_theme_comments_for_ajax($comments);
  print drupal_json_output($response);
  exit;
}
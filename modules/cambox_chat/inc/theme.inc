<?php
/**
 * Themes out a list of comments to be used for ajax.
 */
function cambox_chat_theme_comments_for_ajax($comments) {
  $comments_output = '';
  $i = 1;
  $t = count($comments);
  $approve_chat_messages = user_access('Approve Chat Messages');
  foreach($comments as $comment) {
    $classes = array();
    if ( $i == $t ) {
      $classes[] = 'last';
    }
    $comment->unpublished_text = '';
    if ( $approve_chat_messages ) {
      if ( $comment->status == 0 ) {
        $classes[] = 'unpublished';
        $comment->unpublished_text = 'No Publicado';
      }
    }
    $class = implode(' ', $classes);
    $comments_output = cambox_chat_theme_comments_item($comment, $class);
    $i++;
  }
  
  return $comments_output;
}

/**
 * Themes out a list of comments.
 */
function cambox_chat_theme_comments($comments, $type) {
  $output = $comments_output = '';
  $i = 1;
  $t = count($comments);
  $approve_chat_messages = user_access('Approve Chat Messages');
  global $user;
  foreach($comments as $comment) {
    $classes = array();
    if ( $i == 1 ) {
      $classes[] = 'first';
    }
    if ( $i == $t ) {
      $classes[] = 'last';
    }
    $comment->unpublished_text = '';
    if ( $approve_chat_messages ) {
      if ( $comment->status == 0 ) {
        $classes[] = 'unpublished';
        $comment->unpublished_text = 'No Publicado';
      }
    }
    $class = implode(' ', $classes);
    $comments_output .= cambox_chat_theme_comments_item($comment, $class);
    $i++;
  }
  
  $form = '';
  $login_form = '';
  global $user;
  if ( user_access('Post Chat') ) {
    $form = drupal_get_form('cambox_chat_form');
    $form = drupal_render($form);
  } else if ( $user->uid == 0 ) {
    $login_form = fboauth_block_view(1);
    $login_form = $login_form['content'];
  }
  $data_pull_url = base_path() . 'node/' . arg(1) . '/chat/pull/';
  $output = theme('cambox_chat-comments', array(
    'type' => $type, 
    'comments' => $comments_output, 
    'form' => $form, 
    'login_form' => $login_form, 
    'data_pull_url' => $data_pull_url, 
  ));
  return $output;
}

/**
 * Themes out a single comment.
 */
function cambox_chat_theme_comments_item($comment, $class) {
  $params = (array) $comment;
  $params['class'] = $class;
  return theme('cambox_chat-comments-item', $params);
}

/**
 * Themes out a single comment for ajax.
 */
function cambox_chat_theme_comments_item_ajax($comment) {
  $params = cambox_chat_data_comment_prepare($comment);
  return theme('cambox_chat-comments-item', $params);
}

/**
 * Implements hook_theme.
 */
function cambox_chat_theme() {
  return array(
    'cambox_chat-comments-item' => array(
      'arguments' => array(
        'user_picture' => NULL, 
        'user_name' => NULL, 
        'message' => NULL, 
        'time' => NULL, 
        'created' => NULL, 
        'approve_link' => NULL, 
        'save_link' => NULL, 
        'class' => NULL, 
      ), 
      'template' => 'themes/cambox_chat-comments-item', 
    ), 
    'cambox_chat-comments' => array(
      'arguments' => array(
        'type' => NULL, 
        'comments' => NULL, 
        'form' => NULL, 
      ), 
      'template' => 'themes/cambox_chat-comments', 
    ), 
  );
}
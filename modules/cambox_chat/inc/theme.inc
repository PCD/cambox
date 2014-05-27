<?php
/**
 * Themes out a list of comments to be used for ajax.
 */
function cambox_chat_theme_comments_for_ajax($comments) {
  $comments_output = '';
  $i = 1;
  $t = count($comments);
  foreach($comments as $comment) {
    $classes = array();
    if ( $i == $t ) {
      $classes[] = 'last';
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
  foreach($comments as $comment) {
    $classes = array();
    if ( $i == 1 ) {
      $classes[] = 'first';
    }
    if ( $i == $t ) {
      $classes[] = 'last';
    }
    $class = implode(' ', $classes);
    $comments_output .= cambox_chat_theme_comments_item($comment, $class);
    $i++;
  }
  
  $form = drupal_get_form('cambox_chat_form');
  $form = drupal_render($form);
  $output = theme('cambox_chat-comments', array(
    'type' => $type, 
    'comments' => $comments_output, 
    'form' => $form, 
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
<?php
/**
 * Retrieve the latest comments.
 */
function cambox_chat_data_get_comments($nid, $limit, $last_cid = null) {
  $sql = "SELECT c.cid, c.uid, c.nid, c.created, c.status, b.comment_body_value message FROM {comment} c 
  INNER JOIN {field_data_comment_body} b ON b.entity_id = c.cid \n";
  if ( !is_null($last_cid) ) {
    global $user;
    $sql .= sprintf("WHERE c.cid > %d AND c.uid != %d \n", $last_cid, $user->uid);
  }
  $sql .= sprintf("ORDER BY c.created DESC LIMIT %d", $limit);
  $results = db_query($sql);
  $comments = $results->fetchAllAssoc('created');
  if ( !empty($comments) ) {
    foreach($comments as $comment) {
      $baseurl = 'node/' . $comment->nid . '/chat';
      $cuser = user_load($comment->uid);
      $comment->message = check_markup($comment->message, 'filtered_html', '');
      $comment->user_picture = image_style_url('profile', $cuser->picture->uri);
      $comment->user_name = $cuser->field_first_name[LANGUAGE_NONE][0]['safe_value'];
      $comment->user_name .= ' ' . $cuser->field_last_name[LANGUAGE_NONE][0]['safe_value'];
      $comment->user_name = trim($comment->user_name);
      $comment->time = cambox_chat_timeago($comment->created);
      if ( $comment->status == 0 ) {
        $comment->approve_link = l(t('Approve'), $baseurl . '/approve/' . $comment->cid);
      } elseif ( $comment->status == 1 ) {
        $comment->save_link = l(t('Save'), $baseurl . '/save/' . $comment->cid);
      }
    }
    //ksort($comments);
  }
  return $comments;
}

/**
 * Prepares a comment from drupal api to comment on chat.
 */
function cambox_chat_data_comment_prepare($comment) {
  $baseurl = 'node/' . $comment->nid . '/chat';
  $cuser = user_load($comment->uid);
  $name = $cuser->field_first_name[LANGUAGE_NONE][0]['safe_value'];
  $name .= ' ' . $cuser->field_last_name[LANGUAGE_NONE][0]['safe_value'];
  $name = trim($name);
  $params = array(
    'cid' => $comment->cid, 
    'user_picture' => image_style_url('profile', $cuser->picture->uri),  
    'user_name' => $name, 
    'message' => check_markup($comment->comment_body[LANGUAGE_NONE][0]['value'], 'filtered_html', ''), 
    'time' => cambox_chat_timeago($comment->created), 
    'class' => 'first', 
  );
  if ( $comment->status == 0 ) {
    $params['approve_link'] = l(t('Approve'), $baseurl . '/approve/' . $comment->cid);
  } elseif ( $comment->status == 1 ) {
    $params['save_link'] = l(t('Save'), $baseurl . '/save/' . $comment->cid);
  }
  return $params;
}

/**
 * Find out timeago.
 */
function cambox_chat_timeago($time) {
  $now = time();
  $left = $now - $time;
  $seconds = $minutes = $hours = $days = $weeks = 0;
  
  $timeago = array();
  
  // Seconds
  $seconds = $left%60;
  if ( $seconds >= 0 ) {
    $timeago[0] = t("@seconds seconds", array('@seconds' => $seconds));
  }
  
  // Minutes
  $left = intval($left/60);
  if ( $left > 0 ) {
    $minutes = $left%60;
    if ( $minutes > 0 ) {
      $timeago[1] = t("@minutes minutes", array('@minutes' => $minutes));
    }
    
    // Hours
    $left = intval($left/60);
    if ( $left > 0 ) {
      $hours = $left%24;
      if ( $hours > 0 ) {
        $timeago[2] = t("@hours hours", array('@hours' => $hours));
      }
      
      // Days
      $left = intval($left/24);
      if ( $left > 0 ) {
        $days = $left%24;
        if ( $days > 0 ) {
          $timeago[3] = t("@days days", array('@days'=>$days));
        }
        
        // Weeks
        $left = intval($left/7);
        if ( $left > 0 ) {
          $weeks = $left%7;
          if ( $weeks > 0 ) {
            $timeago[4] = t("@weeks weeks", array('@weeks' => $weeks));
          }
        }
      }
    }
  }
  
  while(count($timeago)>2) {
    array_shift($timeago);
  }
  
  // Build String
  krsort($timeago);
  return implode(', ', $timeago);
}
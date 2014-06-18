<?php
/**
 * Retrieve the latest comments.
 */
function cambox_chat_data_get_comments($nid, $limit, $last_time = null) {
  global $user;
  $fields = "SELECT c.cid, c.uid, c.nid, c.created, c.status, b.comment_body_value message ";
  $tables = "FROM {comment} c 
  INNER JOIN {field_data_comment_body} b ON b.entity_id = c.cid \n";
  $where_clauses = array(
    sprintf('c.nid = %d', $nid), 
  );
  if ( !is_null($last_time) ) {
    $where_clauses[] = sprintf(" c.created > %d ", $last_time);
    $where_clauses[] = sprintf(" c.uid != %d ", $user->uid);
  }
  if ( !user_access('Approve Chat Messages') ) {
    if ( $user->uid > 0 ) {
      $where_clauses[] = sprintf(" IF(c.uid=%d, 1, c.status) = 1 ", $user->uid);
    } else {
      $where_clauses[] = " c.status = 1 ";
    }
  }
  $where = '';
  if ( !empty($where_clauses) ) {
    $where = 'WHERE ' . implode(' AND ', $where_clauses);
  }
  $order = sprintf("ORDER BY c.created DESC LIMIT %d", $limit);
  $sql = "{$fields} 
  {$tables} 
  {$where} 
  {$order}";
  
  $results = db_query($sql);
  $comments = $results->fetchAllAssoc('created');
  $user_can_approve = user_access('Approve Chat Messages');
  $user_can_save = user_access('Save Chat Messages');
  if ( !empty($comments) ) {
    foreach($comments as $comment) {
      $baseurl = 'node/' . $comment->nid . '/chat';
      $cuser = user_load($comment->uid);
      $comment->message = check_markup($comment->message, 'filtered_html', '');
      $comment->user_picture = image_style_url('profile', $cuser->picture->uri);
      $comment->user_name = $cuser->field_first_name[LANGUAGE_NONE][0]['safe_value'];
      $comment->user_name .= ' ' . $cuser->field_last_name[LANGUAGE_NONE][0]['safe_value'];
      $comment->user_name = trim($comment->user_name);
      //$comment->time = cambox_chat_timeago($comment->created);
      if ( $comment->status == 0 ) {
        if ( $user_can_approve ) {
          $comment->approve_link = l(t('Approve'), $baseurl . '/approve/' . $comment->cid);
        }
      } elseif ( $comment->status == 1 ) {
        if ( $user_can_save ) {
          $comment->save_link = l(t('Save'), $baseurl . '/save/' . $comment->cid);
        }
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
  $user_can_approve = user_access('Approve Chat Messages');
  $user_can_save = user_access('Save Chat Messages');
  $params = array(
    'cid' => $comment->cid, 
    'user_picture' => image_style_url('profile', $cuser->picture->uri),  
    'user_name' => $name, 
    'message' => check_markup($comment->comment_body[LANGUAGE_NONE][0]['value'], 'filtered_html', ''), 
    'created' => $comment->created, 
    //'time' => cambox_chat_timeago($comment->created), 
    'class' => 'first', 
  );
  if ( $comment->status == 0 ) {
    if ( $user_can_save ) {
      $params['approve_link'] = l(t('Approve'), $baseurl . '/approve/' . $comment->cid);
    }
  } elseif ( $comment->status == 1 ) {
    if ( $user_can_save ) {
      $params['save_link'] = l(t('Save'), $baseurl . '/save/' . $comment->cid);
    }
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

/**
 * Get last comment time from current chat and current user.
 */
function cambox_chat_data_get_last_comment_time($nid, $uid) {
  $sql = sprintf("SELECT MAX(created) time FROM {comment} WHERE nid = %d AND uid = %d", $nid, $uid);
  $results = db_query($sql);
  if ( ($row = $results->fetchAssoc()) ) {
    return intval($row['time']);
  }
  return 0;
}

/**
 * Approves a comment.
 */
function cambox_chat_data_approves_comment($nid, $cid) {
  $sql = "UPDATE {comment} SET status = 1, created = %d WHERE nid = %d AND cid = %d";
  $sql = sprintf($sql, time(), $nid, $cid);
  db_query($sql);
  return true;
}

/**
 *
 */
function cambox_get_time_limit() {
  return variable_get('cambox-time-left', 30);
}
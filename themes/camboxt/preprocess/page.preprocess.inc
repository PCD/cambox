<?php

/**
 * Implements hook_preprocess_page().
 */
function camboxt_preprocess_page(&$variables) {
  // You can use preprocess hooks to modify the variables before they are passed
  // to the theme function or template file.
  if ( arg(0) == 'node' && intval(arg(1)) > 0 && is_null(arg(2)) && ($node = node_load(arg(1))) ) {
    //camboxt_social_buttons_block($url);
  }
}
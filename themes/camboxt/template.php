<?php

/**
 * @file
 * Template overrides as well as (pre-)process and alter hooks for the
 * Cambox Theme theme.
 */

/**
 * Return a link to initiate a Facebook Connect login or association.
 *
 * @param $link
 *   An array of properties to be used to generate a login link. Note that all
 *   provided properties are required for the Facebook login to succeed and
 *   must not be changed. If $link is FALSE, Facebook OAuth is not yet
 *   configured.
 * @see fboauth_link_properties()
 */
function camboxt_fboauth_action__connect($variables) {
  $action = $variables['action'];
  $link = $variables['properties'];
  $url = url($link['href'], array('query' => $link['query']));
  $link['attributes']['class'] = isset($link['attributes']['class']) ? $link['attributes']['class'] : 'facebook-action-connect';
  $link['attributes']['rel'] = 'nofollow';
  $attributes = isset($link['attributes']) ? drupal_attributes($link['attributes']) : '';
  $title = isset($link['title']) ? check_plain($link['title']) : '';
  $src = ($GLOBALS['is_https'] ? 'https' : 'http') . '://www.facebook.com/images/fbconnect/login-buttons/connect_light_big_short.gif';
  return '<a ' . $attributes . ' href="' . $url . '">' . t('Facebook Login') . '</a>';
}
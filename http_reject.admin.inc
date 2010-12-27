<?php
// $Id$
/**
 * Settings form.
 */
function http_reject_admin_form() {
  // Load the settings.
  $settings = variable_get('http_reject', http_reject_defaults());

  $form['http_reject'] = array('#tree' => TRUE);
  $form['http_reject']['methods']['allowed'] = array(
    '#title' => 'Allowed HTTP Methods',
    '#description' => t('Add HTTP request methods that should be allowed, one per line. The GET, POST and HEAD method will always be allowed, even if you remove them here, as Drupal can not function without them. Other request methods include: PUT, DELETE, CONNECT, OPTIONS, PATCH, PROPFIND, PROPPATCH, MKCOL, COPY, MOVE, LOCK, UNLOCK, TRACE'),
    '#type' => 'textarea',
    '#default_value' => (empty($settings['methods']['allowed'])) ? "GET\nPOST\nHEAD" : implode("\n", $settings['methods']['allowed']),
  );

  $form['http_reject']['methods']['code'] = array(
    '#type' => 'radios',
    '#title' => 'Status Code',
    '#description' => t('Select which HTTP status code should be used to reject access on requests made via a non-permitted method. The default is 405.'),
    '#options' => http_reject_codes(TRUE),
    '#default_value' => (empty($settings['methods']['code'])) ? 405 : $settings['methods']['code'],
  );

  return system_settings_form($form);
}

/**
 * Validator for the settings form.
 */
function http_reject_admin_form_validate($form, &$form_state) {
  if (strpos($form_state['values']['http_reject']['method']['allowed'], ' ') !== FALSE) {
    form_set_error('http_reject][method][allowed', t('Please enter one valid request method per line.'));
  }
  elseif (check_plain($form_state['values']['http_reject']['method']['allowed']) != $form_state['values']['http_reject']['method']['allowed']) {
    form_set_error('http_reject][method][allowed', t('Please enter one valid request method per line.'));
  }
  elseif (strtoupper($form_state['values']['http_reject']['method']['allowed']) != $form_state['values']['http_reject']['method']['allowed']) {
    form_set_error('http_reject][method][allowed', t('Please enter one valid request method per line.'));
  }

  // Turns the methods into an array.
  if (!empty($form_state['values']['http_reject']['methods']['allowed'])) {
    $methods = explode("\n", $form_state['values']['http_reject']['methods']['allowed']);
  }
  else {
    $methods = array();
  }
  $form_state['values']['http_reject']['methods']['allowed'] = $methods;
}

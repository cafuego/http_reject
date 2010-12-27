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

  $form['http_reject']['ua']['reject'] = array(
    '#title' => 'Disallowed User Agents',
    '#description' => t('Add preg compatible regular expressions, one per line, that match user agent strings you wish to block. Any expressions that match your current user agent will not be accepted. See the <a href="!link">preg_match</a> api page on php.net for details on the syntax. Your current user-agent string is "%ua"', array('!link' => url('http://php.net/manual/en/function.preg-match.php'), '%ua' => $_SERVER['HTTP_USER_AGENT'])),
    '#type' => 'textarea',
    '#default_value' => (empty($settings['ua']['reject'])) ? '' : implode("\n", $settings['ua']['reject']),
  );

  $form['http_reject']['ua']['code'] = array(
    '#type' => 'radios',
    '#title' => 'Status Code',
    '#description' => t('Select which HTTP status code should be used to reject access on requests made via a listed user-agent. The default is 405.'),
    '#options' => http_reject_codes(TRUE),
    '#default_value' => (empty($settings['ua']['code'])) ? 405 : $settings['ua']['code'],
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

  // Check the UA pregs.
  $pregs =  explode("\n", $form_state['values']['http_reject']['ua']['reject']);

  $form_state['values']['http_reject']['ua']['reject'] = array();
  foreach ($pregs as $preg) {
    // Just explode() seems to leave trailing whitespace, so we first trim() the
    // preg and then add it back into the array that will be saved if it passes
    // validation.
    $preg = trim($preg);

    if (preg_match($preg, $_SERVER['HTTP_USER_AGENT']) === FALSE) {
      form_set_error('http_reject][ua][reject', t('The expression "%preg" is not a valid regular expression.', array('%preg' => check_plain($preg))));
    }
    else if (preg_match($preg, $_SERVER['HTTP_USER_AGENT']) == 1) {
      form_set_error('http_reject][ua][reject', t('The expression "%preg" matches your current user-agent. Saving it would prevent you from accessing Drupal.', array('%preg' => check_plain($preg))));
    }
    else {
      $form_state['values']['http_reject']['ua']['reject'][] = $preg;
    }
  }
}

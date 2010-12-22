The http_reject module looks at the HTTP request being sent to your
Drupal instance and will reject it if set criterial are met.

The goal is to allow users to configure their Drupal installations to
reject HTTP requests from such things as FrontPage or a WebDAV client
early on in the bootstrap process, so the site uses fewer resources
and the logs aren't spammed with useless request entries.

If possible, this should in fact be done in .htaccess or the web server
configuration files, but not all users have access to those.

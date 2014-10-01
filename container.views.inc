<?php
/**
 * Created by PhpStorm.
 * User: ZM
 * Date: 25/9/2014
 * Time: 2:14 PM
 */

/**
 * Implementation of hook_views_plugins().
 */
function container_views_plugins() {
  $module_path = drupal_get_path('module', 'container');

  return array(
    'style' => array(
      'views_zm_template_plugin_style' => array(
        'title' => t('ZM Template'),
        'help' => t('ZM Template Style'),
        'path' => $module_path . '/plugins/zm_template',
        'handler' => 'ViewsZMTemplatePluginStyle',
        'parent' => 'default',
        'theme' => 'views_zm_template_plugin_style',
        'theme path' => $module_path . '/templates/zm_template',
        'theme file' => 'theme.inc',
        'uses row plugin' => TRUE,
        'uses grouping' => FALSE,
        'uses options' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
}

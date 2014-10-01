<?php
/**
 * Created by PhpStorm.
 * User: ZM
 * Date: 7/26/2014
 * Time: 10:27 PM
 */

class Drupal {
  /**
   * Gets the current active user.
   */
  public static function currentUser() {
    global $user;

    return $user;
  }

  /**
   * Gererater URL.
   *
   * @param $route_name
   * @param array $route_parameters
   * @param array $options
   *
   * @return string
   */
  public static function url($route_name, $route_parameters = array(), $options = array()) {
    $options['query'] = $route_parameters;

    return url($route_name, $options);
  }

  /**
   * Gererater link.
   *
   * @param $text
   * @param $route_name
   * @param array $route_parameters
   * @param array $options
   *
   * @return string
   */
  public static function l($text, $route_name, array $route_parameters = array(), array $options = array()) {
    $options['query'] = $route_parameters;
    if ( ! isset( $options['html'] )) {
      $options['html'] = TRUE;
    }

    return l($text, $route_name, $options);
  }


  public static function l_class($text, $route_name, $class = '', array $options = array()) {
    if (!empty($class)) {
      $options['attributes']['class'][] = $class;
    }
    if ( ! isset( $options['html'] )) {
      $options['html'] = TRUE;
    }

    return l($text, $route_name, $options);
  }

  /**
   * Return themed image.
   *
   * @param $path
   * @param array $options
   *
   * @return string
   * @throws Exception
   */
  public static function image($path, $options = array(), $class = '') {
    $image = array(
      'path' => $path,
    );
    if(!empty($class)){
      $options['attributes']['class'][] = $class;
    }

    return theme('image', $image + $options);
  }


  /**
   * Return themed image style.
   *
   * @param $path
   * @param $style_name
   * @param array $options
   *
   * @return string
   * @throws Exception
   */
  public static function image_style($path, $style_name, $options = array()) {
    $image = array(
      'path'       => $path,
      'style_name' => $style_name,
    );

    return theme('image_style', $image + $options);
  }

  /**
   * Generate image link.
   *
   * @param $path
   * @param $route_name
   * @param array $route_parameters
   * @param array $options
   *
   * @return string
   */
  public static function link_image($path, $route_name, array $route_parameters = array(), array $options = array()) {
    $image           = self::image($path, $options);
    $options['html'] = TRUE;

    return self::l($image, $route_name, $route_parameters, $options);
  }

  /**
   * Drupal static store for one request.
   *
   * @param $key
   * @param $set_data
   */
  public static function drupal_static($key, $set_data = NULL){
    $data = &drupal_static($key);

    if ($set_data) {
      $data = $set_data;
    }
    else {
      return $data;
    }
  }

  /**
   * Fast render template to html markup.
   *
   * @param $template
   * @param $data
   */
  public static function render_template($template, $data = array()) {
    return render(static::template($template, $data));
  }

  /**
   * Create template array ready for render.
   *
   * @param $template
   * @param $data
   */
  public static function template($template, $data = array(), $template_id = NULL) {
    return array(
      'container_template' => array(
        '#theme'       => 'container_template',
        '#template'    => $template,
        '#data'        => $data,
        '#template_id' => $template_id,
      )
    );
  }

  /**
   * Get path of module and file in module.
   *
   * @param $module
   * @param $file
   *
   * @return string
   */
  public static function module_path($module, $file = NULL) {
    static $path = NULL;
    if ( ! isset( $path[$module] )) {
      $path[$module] = drupal_get_path('module', $module);
    }
    if ($file) {
      return $path[$module] . "/" . $file;
    }

    return $path[$module];
  }

  /**
   * Get path of theme and file in theme.
   *
   * @param $theme
   * @param $file
   *
   * @return string
   */
  public static function theme_path($theme, $file = NULL) {
    static $path = NULL;
    if ( ! isset( $path[$theme] )) {
      $path[$theme] = drupal_get_path('theme', $theme);
    }
    if ($file) {
      return $path[$theme] . "/" . $file;
    }

    return $path[$theme];
  }

  /**
   * Add css to Drupal.
   *
   * @param $css
   *  Path to file or array of path.
   * @param array $options
   */
  public static function add_css($css, $options = array()) {
    if (is_array($css)) {
      foreach ($css as $item) {
        drupal_add_css($item, $options);
      }
    }
    else {
      drupal_add_css($css, $options);
    }
  }

  /**
   * Add js to Drupal.
   *
   * @param $css
   *  Path to file or array of path.
   * @param array $options
   */
  public static function add_js($js, $options = array()) {
    if (is_array($js)) {
      foreach ($js as $item) {
        drupal_add_js($item, $options);
      }
    }
    else {
      drupal_add_js($js, $options);
    }
  }

  /**
   * Get real path.
   *
   * @param $path
   *
   * @return mixed|string
   */
  public static function real_path($path) {
    return container_realpath($path);
  }

  /**
   * Implement dpm.
   *
   * @param $vars
   * @param string $title
   */
  public static function dpm($vars, $name = NULL){
    if (function_exists('dpm')) {
      dpm($vars, $name);
    }
  }

  /**
   * Implement dpm one.
   *
   * @param $vars
   * @param $name
   */
  public static function dpm_one($vars, $name = NULL){
    $key = isset( $name ) ? "dpm__" . $name : "dpm__one";
    if ( ! Drupal::drupal_static($key)) {
      static::dpm($vars, $name);
      Drupal::drupal_static($key, TRUE);
    }
  }
}
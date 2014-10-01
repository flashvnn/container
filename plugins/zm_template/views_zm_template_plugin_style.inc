<?php
/**
 * @file
 * Definition of views_bootstrap_plugin_style.
 */

/**
 * Class to define a style plugin handler.
 */
class ViewsZMTemplatePluginStyle extends views_plugin_style {
  /**
   * Definition.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['template']    = array('default' => '');
    $options['render_type'] = array('default' => 'raw');
    return $options;
  }

  /**
   * Options form.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['template'] = array(
      '#type'          => 'textfield',
      '#required'      => TRUE,
      '#title'         => t('Template'),
      '#description'   => t('Template used render views output data. Support token {theme-name}, {module-name}'),
      '#default_value' => $this->options['template'],
    );

    $type_options = array(
      'raw'    => 'Raw',
      'styled' => 'Styled',
    );

    $form['render_type'] = array(
      '#title'         => t('Field Render Format'),
      '#type'          => 'select',
      '#options'       => $type_options,
      '#default_value' => $this->options['render_type'],
      '#description'   => t('Select field render format.'),
    );
  }
}

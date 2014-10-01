<?php

/*
 * module      = container
 * PLUGINOWNER = ctools. The plugin owner.
 * PLUGINTYPE  = content_types.  One plugin owner can define more than one plugin type.
 * PLUGINNAME  = container_template.
 *
 */

$plugin = array(
  'category'    => t('Container'),
  'title'       => t('Container: Container template'),
  'description' => t('Render a tpl template'),
  'defaults' => array(
    'template'    => '',
    'template_id' => '',
  ),
  'edit form'        => 'container_ctools_content_types_container_template_edit_form',
  'edit form submit' => 'container_ctools_content_types_container_template_edit_form_submit',
  'admin info'       => 'container_ctools_content_types_container_template_admin_info',
  'admin title'       => 'container_ctools_content_types_container_template_admin_title',
  'render callback'  => 'container_ctools_content_types_container_template_render',
);

/**
 * Edit form callback.
 */
function container_ctools_content_types_container_template_edit_form($form, &$form_state) {
  $defaults = array();
  if (isset($form_state['subtype']['defaults'])) {
    $defaults = $form_state['subtype']['defaults'];
  }
  elseif (isset($form_state['plugin']['defaults'])) {
    $defaults = $form_state['plugin']['defaults'];
  }

  $conf = $form_state['conf'] + $defaults;

  $key = 'template';

  $form[$key] = array(
    '#type' => 'textfield',
    '#title' => t('Template Path'),
    '#default_value' => $conf[$key],
  );

  $key = 'template_id';
  $form[$key] = array(
    '#type' => 'textfield',
    '#title' => t('Template ID'),
    '#description' => t('Template ID, usefull when you need preprocess template data'),
    '#default_value' => $conf[$key],
  );

  return $form;
}

/**
 * Edit form submit callback.
 */
function container_ctools_content_types_container_template_edit_form_submit($form, &$form_state) {
  $defaults = array();
  if (isset($form_state['subtype']['defaults'])) {
    $defaults = $form_state['subtype']['defaults'];
  }
  elseif (isset($form_state['plugin']['defaults'])) {
    $defaults = $form_state['plugin']['defaults'];
  }

  foreach (array_keys($defaults) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
 * Admin info callback.
 *
 * @param string $subtype
 *   Identifier of subtype.
 * @param array $conf
 *   Pane configuration. Array keys are depends on the _edit_form.
 *   - override_title
 *   - override_title_text
 * @param array $context
 *   An array of context objects available for use. These may be placeholders.
 *
 * @return object
 *   Administrative title and description of the $subtype pane.
 *   The keys are:
 *   - title: string
 *   - content: string|array
 */
function container_ctools_content_types_container_template_admin_info($subtype, $conf, $context) {
  $return = new stdClass();
  $return->title = $conf['template'];
  $content = $conf['template'];
  if (!empty($conf['template_id'])) {
    $content .= t("<div>Template Id: @template_id</div>", array( "@template_id" => $conf['template_id'] ));
  }

  $return->content = $content;

  return $return;
}

function container_ctools_content_types_container_template_admin_title($subtype, $conf, $context){
  if (!empty($conf['override_title_text'])) {
    return "Container Template: " . $conf['override_title_text'];
  }
  $path = pathinfo($conf['template']);

  return "Container Template:" . str_replace('.tpl.php','', $path['basename']);
}

/**
 * Render callback.
 *
 * "CTools:Content types" plugin render callback for "Container:Container template".
 *
 * @param string $subtype
 *   Subtype identifier.
 * @param array  $conf
 *   Configuration of the $subtype instance.
 * @param array  $args
 *   Documentation missing.
 * @param array  $pane_context
 *   Documentation missing.
 * @param array  $incoming_content
 *   Documentation missing.
 *
 * @return object
 *   The content.
 */
function container_ctools_content_types_container_template_render($subtype, $conf, $args, $pane_context, $incoming_content) {
  $block         = new stdClass();
  $block->module = 'container';
  $block->delta  = $subtype;

  $template = container_realpath($conf['template']);
  if ( ! empty( $template )) {
    $template_id    = @$conf['template_id'] ?: NULL;
    $block->content = array(
      'content' => Drupal::template($template, array(), $template_id),
    );
  }

  return $block;
}

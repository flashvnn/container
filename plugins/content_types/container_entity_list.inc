<?php
/*
 * module      = container
 * PLUGINOWNER = ctools. The plugin owner.
 * PLUGINTYPE  = content_types.  One plugin owner can define more than one plugin type.
 * PLUGINNAME  = container_entity_list.
 */

$plugin = array(
  'category' => t('Container'),
  'title' => t('Container: Container entity list'),
  'description' => t('Create list of entity'),
  'defaults' => array(
    'entity_type'    => '',
    'view_mode'      => 'teaser',
    'pager'          => 0,
    'sort'           => 'DESC',
    'template'       => FALSE,
    'items_per_page' => 10,
  ),
  'admin info'         => 'container_ctools_content_types_container_entity_list_admin_info',
  'admin title'        => 'container_ctools_content_types_container_entity_list_admin_title',
  'edit form'          => 'container_ctools_content_types_container_entity_list_edit_form',
  'edit form submit'   => 'container_ctools_content_types_container_entity_list_edit_form_submit',
  'render callback'    => 'container_ctools_content_types_container_entity_list_render',
);

/**
 * Edit form callback.
 */
function container_ctools_content_types_container_entity_list_edit_form($form, &$form_state) {
  $defaults = array();
  if (isset($form_state['subtype']['defaults'])) {
    $defaults = $form_state['subtype']['defaults'];
  }
  elseif (isset($form_state['plugin']['defaults'])) {
    $defaults = $form_state['plugin']['defaults'];
  }

  $conf = $form_state['conf'] + $defaults;



  $node_types = array();
  foreach (node_type_get_types() as $type => $item) {
    $node_types[$type] = $item->name;
  }

  $key = 'entity_type';
  $form[$key] = array(
    '#type'          => 'select',
    '#required'      => TRUE,
    '#title'         => t('Entity Type'),
    '#description'   => t('Select entity type to create list'),
    '#default_value' => $conf[$key],
    '#empty_option'  => t('- Select -'),
    '#options'       => $node_types,
  );

  $view_modes  = array();
  $view_modes[''] = 'None';
  $entity_info = entity_get_info('node');
  foreach ($entity_info['view modes'] as $key => $item) {
    $view_modes[$key] = $item['label'];
  };

  $key        = 'view_mode';
  $form[$key] = array(
    '#type'          => 'select',
    '#title'         => t('View Mode'),
    '#options'       => $view_modes,
    '#default_value' => $conf[$key],
    '#description'   => t('Select display view mode'),
  );

  $key = 'items_per_page';
  $form[$key] = array(
    '#type'          => 'select',
    '#required'      => TRUE,
    '#title'         => t('Item per page'),
    '#default_value' => $conf[$key],
    '#empty_option'  => t('- Select -'),
    '#options'       => range(0, 50)
  );

  $key = 'pager';
  $form[$key] = array(
    '#type'          => 'select',
    '#required'      => TRUE,
    '#title'         => t('Use pager'),
    '#default_value' => $conf[$key],
    '#empty_option'  => t('- Select -'),
    '#options'       => array(
      0 => "No",
      1 => "Yes",
    ),
  );

  $key = 'sort';
  $form[$key] = array(
    '#type'          => 'select',
    '#required'      => TRUE,
    '#title'         => t('Sort'),
    '#default_value' => $conf[$key],
    '#empty_option'  => t('- Select -'),
    '#options'       => array(
      'DESC' => "DESC",
      'ASC' => "ASC",
    ),
  );

  $key = 'template';
  $form[$key] = array(
    '#type'          => 'textfield',
    '#required'      => FALSE,
    '#title'         => t('Template'),
    '#description'   => t('Choose template display with entity list'),
    '#default_value' => $conf[$key],
  );

  return $form;
}

/**
 * Edit form submit callback.
 */
function container_ctools_content_types_container_entity_list_edit_form_submit($form, &$form_state) {
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
function container_ctools_content_types_container_entity_list_admin_info($subtype, $conf, $context) {
  $return = new stdClass();
  $return->title = 'Entity Type:<strong>' . $conf['entity_type'] . '</strong> ::View mode:<strong> ' . $conf['view_mode'] . "</strong>";
  if ($conf['template']) {
    $return->content = "Template:{$conf['template']}";
  }

  return $return;
}

function container_ctools_content_types_container_entity_list_admin_title($subtype, $conf, $context){
  if (!empty($conf['override_title_text'])) {
    return "Entity List: " . $conf['override_title_text'];
  }

  return "Entity List: " . $conf['entity_type'];
}


/**
 * Render callback.
 *
 * "CTools:Content types" plugin render callback for "Container:Container entity list".
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
function container_ctools_content_types_container_entity_list_render($subtype, $conf, $args, $pane_context, $incoming_content) {
  $block = new stdClass();
  $block->module = 'container';
  $block->delta = $subtype;


  $query = new EntityFieldQuery();
  $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', $conf['entity_type'])
    ->propertyCondition('status', 1)
    ;
  $query->propertyOrderBy('created', $conf['sort']);
  if ($conf['items_per_page'] > 0) {
    $query->range(0, $conf['items_per_page']);
  }
  if ($conf['pager'] == 1) {
    $query->pager($conf['items_per_page']);
  }
  $result = $query->execute();

  $build = array();
  if ( ! empty( $result['node'] )) {
    $entities = entity_load('node', array_keys($result['node']));
    if (!empty($conf['view_mode'])) {
      $build    = entity_view('node', $entities, $conf['view_mode']);
    }else{
      $build['node'] = $entities;
    }
  }
  if ( ! empty( $build )) {
    if ( ! empty( $conf['template'] )) {
      $template       = Drupal::real_path($conf['template']);
      if (isset($build['node']['#sorted'])) {
        unset($build['node']['#sorted']);
      }
      $block->content = array(
        'content' => Drupal::template($template, $build),
      );
    }
    else {
      $block->content = array(
        'content' => $build,
      );
    }
  }

  return $block;
}

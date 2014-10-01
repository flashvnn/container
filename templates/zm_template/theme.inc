<?php

/**
 * Implementation of template preprocess for the view.
 */
function template_preprocess_views_zm_template_plugin_style(&$vars) {
  if(!empty($vars['options']['template'])){
    $vars['options']['template'] = Drupal::real_path($vars['options']['template']);
  	if($vars['options']['render_type'] == 'styled'){
  		$vars['template_rendered'] = Drupal::template($vars['options']['template'],$vars['rows']);
  	}else{
  		if(isset($vars['view']->style_plugin->rendered_fields)){
  			$vars['template_rendered'] = Drupal::template($vars['options']['template'], $vars['view']->style_plugin->rendered_fields);
  		}else{
  			$vars['template_rendered'] = '';
  		}
  	}
  }
}

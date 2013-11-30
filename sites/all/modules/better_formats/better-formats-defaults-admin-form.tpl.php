<?php
// $Id: better-formats-defaults-admin-form.tpl.php,v 1.3.2.7 2009/09/27 14:27:13 dragonwize Exp $

/**
 * @file
 * Default theme implementation to configure Better Formats defaults admin page.
 *
 * Available variables:
 * - $form_submit: Form submit button.
 *
 * Each $node_default_rows contains a row
 *
 * Each $data in $node_default_rows contains:
 * - $data->role: Role name.
 * - $data->format_select: Drop-down menu for setting format.
 * - $data->weight_select: Drop-down menu for setting weights.
 */
?>
<?php
  // Add table javascript
  drupal_add_tabledrag('node-format-defaults', 'order', 'sibling', 'better-formats-role-node-weight');
  drupal_add_tabledrag('comment-format-defaults', 'order', 'sibling', 'better-formats-role-comment-weight');
  drupal_add_tabledrag('block-format-defaults', 'order', 'sibling', 'better-formats-role-block-weight');
?>
<div class="description">
  <?php print '<p><strong>' . t('Defaults only affect NEW content NOT existing content.') . '</strong></p>'; ?>
  <?php print '<p><strong>' . t('Place roles in order of precedence by dragging more important roles to the top.') . '</strong></p>'; ?>
</div>
<fieldset>
  <legend><strong><?php print t('Node defaults'); ?></strong></legend>
  <table id="node-format-defaults">
    <thead>
      <tr>
        <th><?php print t('Role'); ?></th>
        <th><?php print t('Default format'); ?></th>
        <th><?php print t('Weight'); ?></th>
      </tr>
    </thead>
    <tbody>
      <?php $row = 0; ?>
      <?php foreach ($node_default_rows as $rid => $data): ?>
      <tr class="draggable <?php print $row % 2 ? 'odd' : 'even'; ?>">
        <td class=""><?php print $data->role; ?></td>
        <td><?php print $data->format_select; ?></td>
        <td><?php print $data->weight_select; ?></td>
      </tr>
      <?php $row++; ?>
      <?php endforeach; ?>
    </tbody>
  </table>
</fieldset>

<fieldset>
  <legend><strong><?php print t('Comment defaults'); ?></strong></legend>
  <table id="comment-format-defaults">
    <thead>
      <tr>
        <th><?php print t('Role'); ?></th>
        <th><?php print t('Default format'); ?></th>
        <th><?php print t('Weight'); ?></th>
      </tr>
    </thead>
    <tbody>
      <?php $row = 0; ?>
      <?php foreach ($comment_default_rows as $rid => $data): ?>
      <tr class="draggable <?php print $row % 2 ? 'odd' : 'even'; ?>">
        <td class=""><?php print $data->role; ?></td>
        <td><?php print $data->format_select; ?></td>
        <td><?php print $data->weight_select; ?></td>
      </tr>
      <?php $row++; ?>
      <?php endforeach; ?>
    </tbody>
  </table>
</fieldset>

<?php if ($_GET['q'] === 'admin/settings/filters/defaults'): ?>
<fieldset>
  <legend><strong><?php print t('Block defaults'); ?></strong></legend>
  <?php if (isset($block_default_rows)): ?>
    <table id="block-format-defaults">
      <thead>
        <tr>
          <th><?php print t('Role'); ?></th>
          <th><?php print t('Default format'); ?></th>
          <th><?php print t('Weight'); ?></th>
        </tr>
      </thead>
      <tbody>
        <?php $row = 0; ?>
        <?php foreach ($block_default_rows as $rid => $data): ?>
        <tr class="draggable <?php print $row % 2 ? 'odd' : 'even'; ?>">
          <td class=""><?php print $data->role; ?></td>
          <td><?php print $data->format_select; ?></td>
          <td><?php print $data->weight_select; ?></td>
        </tr>
        <?php $row++; ?>
        <?php endforeach; ?>
      </tbody>
    </table>
  <?php endif; ?>

</fieldset>
<?php endif; ?>

<div class="description">
  <?php print '<p>' . t('* Only formats that a role has permission to use are shown in the default format drop downs.') . '</p>'; ?>
</div>

<?php print $form_submit; ?>

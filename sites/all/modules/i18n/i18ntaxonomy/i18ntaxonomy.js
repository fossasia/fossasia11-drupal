Drupal.behaviors.i18ntaxonomy = function (context) {
  if (Drupal.settings && Drupal.settings.i18ntaxonomy_vocabulary_form) {
    $('form#taxonomy-form-vocabulary input.form-radio', context).filter('[name=i18nmode]').click(function () {
      var languageSelect = $('form#taxonomy-form-vocabulary select#edit-language', context);
      if ($(this).val() == Drupal.settings.i18ntaxonomy_vocabulary_form.I18N_TAXONOMY_LANGUAGE) {
        // Make sure language form is enabled when I18N_TAXONOMY_LANGUAGE is clicked
        languageSelect.removeAttr("disabled");
      }
      else {
        // Make sure language form is disabled otherwise and set to blank.
        languageSelect.val("");
        languageSelect.attr("disabled", "disabled");
      }
    });
  }
};

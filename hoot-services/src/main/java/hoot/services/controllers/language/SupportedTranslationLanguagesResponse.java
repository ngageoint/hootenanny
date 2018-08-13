
package hoot.services.controllers.language;

import hoot.services.language.SupportedLanguage;

public class SupportedTranslationLanguagesResponse 
{
  private SupportedLanguage[] supportedLanguages;

  public SupportedTranslationLanguagesResponse() {}
  public SupportedTranslationLanguagesResponse(SupportedLanguage[] langs) 
  {
    this.supportedLanguages = langs;
  }

  public SupportedLanguage[] getSupportedLanguages() { return supportedLanguages; }
  public void setSupportedLanguages(SupportedLanguage[] langs) { this.supportedLanguages = langs; }
}

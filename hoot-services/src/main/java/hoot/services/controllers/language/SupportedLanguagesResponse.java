
package hoot.services.controllers.language;

import hoot.services.language.SupportedLanguage;

public class SupportedLanguagesResponse 
{
  private SupportedLanguage[] supportedLanguages;

  public SupportedLanguagesResponse() {}
  public SupportedLanguagesResponse(SupportedLanguage[] langs) 
  {
    this.supportedLanguages = langs;
  }

  public SupportedLanguage[] getSupportedLanguages() { return supportedLanguages; }
  public void setSupportedLanguages(SupportedLanguage[] langs) { this.supportedLanguages = langs; }
}

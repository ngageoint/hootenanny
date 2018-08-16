
package hoot.services.controllers.language;

import hoot.services.language.SupportedLanguage;

public class SupportedLanguagesResponse 
{
  private SupportedLanguage[] languages;

  public SupportedLanguagesResponse() {}
  public SupportedLanguagesResponse(SupportedLanguage[] langs) 
  {
    this.languages = langs;
  }

  public SupportedLanguage[] getLanguages() { return languages; }
  public void setLanguages(SupportedLanguage[] langs) { this.languages = langs; }
}

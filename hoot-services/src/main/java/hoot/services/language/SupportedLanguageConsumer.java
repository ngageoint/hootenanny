
package hoot.services.language;

public interface SupportedLanguageConsumer 
{
  SupportedLanguage[] getSupportedLanguages();

  boolean isLanguageAvailable(String langCode);

  String getLanguageName(String langCode);
}

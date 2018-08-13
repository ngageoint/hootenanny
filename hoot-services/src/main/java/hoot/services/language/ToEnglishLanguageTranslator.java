
package hoot.services.language;

public interface ToEnglishLanguageTranslator 
{
  String translate(String sourceLangCode, String text) throws Exception;
}

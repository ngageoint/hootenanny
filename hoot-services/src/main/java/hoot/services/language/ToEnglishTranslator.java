
package hoot.services.language;

public interface ToEnglishTranslator 
{
  String translate(String sourceLangCode, String text) throws Exception;
}

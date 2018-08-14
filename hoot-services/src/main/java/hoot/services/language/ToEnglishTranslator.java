
package hoot.services.language;

public interface ToEnglishTranslator 
{
  void setConfig(Object config);

  String translate(StringList sourceLangCodes, String text) throws Exception;

  String getDetectedLanguage();
}

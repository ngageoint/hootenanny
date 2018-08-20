
package hoot.services.language;

public interface ToEnglishTranslator 
{
  void setConfig(Object config);

  String translate(String[] sourceLangCodes, String text) throws Exception;
  String translate(String sourceLangCode, String text) throws Exception;

  String getUrl();
  String getDescription();
}

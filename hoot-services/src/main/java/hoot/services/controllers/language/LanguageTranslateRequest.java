
package hoot.services.controllers.language;

public class LanguageTranslateRequest 
{
  private String[] detectors;
  private String translator;
  private String[] sourceLangCodes;
  private String text;
  private boolean detectedLanguageOverridesSpecifiedSourceLanguages = false;
  private boolean performExhaustiveTranslationSearchWithNoDetection = false;

  public LanguageTranslateRequest() {}

  public String[] getDetectors() { return detectors; }
  public void setDetectors(String[] detectors) { this.detectors = detectors; }

  public String getTranslator() { return translator; }
  public void setTranslator(String translator) { this.translator = translator; }

  public String[] getSourceLangCodes() { return sourceLangCodes; }
  public void setSourceLangCodes(String[] sourceLangCodes) { this.sourceLangCodes = sourceLangCodes; }

  public String getText() { return text; }
  public void setText(String text) { this.text = text; }

  public boolean getDetectedLanguageOverridesSpecifiedSourceLanguages() { return detectedLanguageOverridesSpecifiedSourceLanguages; }
  public void setDetectedLanguageOverridesSpecifiedSourceLanguages(boolean override) 
  { this.detectedLanguageOverridesSpecifiedSourceLanguages = override; }

  public boolean getPerformExhaustiveTranslationSearchWithNoDetection() { return performExhaustiveTranslationSearchWithNoDetection; }
  public void setPerformExhaustiveTranslationSearchWithNoDetection(boolean performExhaustiveSearch) 
  { this.performExhaustiveTranslationSearchWithNoDetection = performExhaustiveSearch; }
}

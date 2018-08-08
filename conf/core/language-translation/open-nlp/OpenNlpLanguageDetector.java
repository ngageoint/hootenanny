//package hoot.core.language;

import opennlp.tools.langdetect.LanguageDetectorModel;
import opennlp.tools.langdetect.LanguageDetector;
import opennlp.tools.langdetect.LanguageDetectorME;
import opennlp.tools.langdetect.Language;

import java.io.IOException;
import java.io.File;

public class OpenNlpLanguageDetector 
{
  //TODO: may want to consider running this as service, so as not to incur model load time with every call
  public static void main(String args[]) throws IOException
  {    
    System.out.println(
      (new LanguageDetectorME(new LanguageDetectorModel(new File(args[0])))).predictLanguage(args[1]).getLang());
  }
}

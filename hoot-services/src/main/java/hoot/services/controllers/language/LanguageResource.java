/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

package hoot.services.controllers.language;

import java.io.IOException;
import java.io.UnsupportedEncodingException;

import java.util.List;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Set;
import java.util.HashSet;

import java.net.URLEncoder;
import java.net.URLDecoder;

import org.xml.sax.SAXException;

import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;
import javax.ws.rs.Consumes;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import org.springframework.stereotype.Controller;

import hoot.services.language.LanguageDetectorFactory;
import hoot.services.language.ToEnglishTranslatorFactory;
import hoot.services.language.ToEnglishTranslator;
import hoot.services.language.LanguageDetector;
import hoot.services.language.LanguageDetectionConsumer;
import hoot.services.language.SupportedLanguage;
import hoot.services.language.SupportedLanguageConsumer;
import hoot.services.language.LanguageAppInfo;
import hoot.services.language.LanguageApp;

import org.apache.commons.lang3.reflect.MethodUtils;
import org.apache.commons.lang3.StringUtils;

import javax.annotation.PreDestroy;

/**
 * Web endpoint for language translation and detection

   * For text returned URL encoded the replace of '+' is a bit of a hack, but not sure how better to handle it yet.
   * Not doing any extensive request param validation yet
 */
@Controller
@Path("")
public class LanguageResource 
{
  private static final Logger logger = LoggerFactory.getLogger(LanguageResource.class);

  public LanguageResource() throws Exception
  {
    try
    {
      //The Joshua init can take a long time, so let's do it here vs having it happen the very first time a translation is made.
      logger.info("Initializing Joshua...");
      MethodUtils.invokeStaticMethod(Class.forName("hoot.services.language.JoshuaLanguageTranslator"), "getInstance", null);
    }
    catch (Exception e)
    {
      throw new Exception("Error initializing Joshua. error: " + e.getMessage());
    }
  }

  /**
   * This occurs just before the web server shuts down.
   */
  @PreDestroy
  public void preDestroy() throws Exception
  {
    try
    {
      logger.info("Closing Joshua...");
      Object joshuaTranslator = 
        MethodUtils.invokeStaticMethod(Class.forName("hoot.services.language.JoshuaLanguageTranslator"), "getInstance", null);
      MethodUtils.invokeMethod(joshuaTranslator, "close");
    }
    catch (Exception e)
    {
      throw new Exception("Error closing Joshua. error: " + e.getMessage());
    }
  }

  /**
   * Returns the available language detectors
   *
   * curl localhost:8080/hoot-services/language/detectors
   *
   * {
      "apps":[
      {
         "name":"TikaLanguageDetector",
         "description":"The language detection portion of a library which detects and extracts metadata and text from many different file types",
         "url":"https://tika.apache.org/"
      },
      {
         "name":"OpenNlpLanguageDetector",
         "description":"The language detector portion of a machine learning based toolkit for the processing of natural language text",
         "url":"https://opennlp.apache.org/"
      }
     ]
    }
   */
  @GET
  @Path("/detectors")
  @Produces(MediaType.APPLICATION_JSON)
  public LanguageAppsResponse getDetectors()
  {
    try
    {
      Set<String> detectorClassNames = LanguageDetectorFactory.getSimpleClassNames();
      List<LanguageApp> apps = new ArrayList<LanguageApp>();
      for (String detectorClassName : detectorClassNames)
      {
        apps.add(languageEntityToApp(detectorClassName));
      }
      return new LanguageAppsResponse(apps.toArray(new LanguageApp[]{}));
    }
    catch (Exception e)
    {
      throw new WebApplicationException(
        e, 
        Response.status(Status.INTERNAL_SERVER_ERROR)
         .entity("Error retrieving available language detector information.  Error: " + e.getMessage())
         .build());
    }
  }

  /**
   * Returns the available language translators
   *
   * curl localhost:8080/hoot-services/language/translators
   *
   * {
   "apps":[
      {
         "name":"JoshuaLanguageTranslator",
         "description":"A statistical machine translation decoder for phrase-based, hierarchical, and syntax-based machine translation",
         "url":"https://cwiki.apache.org/confluence/display/JOSHUA"
      },
      {
         "name":"HootLanguageTranslator",
         "description":"A custom translator that combines language detection into its workflow.  See the Hootenanny User Documentation for more details.",
         "url":"N/A"
      }
     ]
    }
   */
  @GET
  @Path("/translators")
  @Produces(MediaType.APPLICATION_JSON)
  public LanguageAppsResponse getTranslators()
  {
    try
    {
      Set<String> translatorClassNames = ToEnglishTranslatorFactory.getSimpleClassNames();
      List<LanguageApp> apps = new ArrayList<LanguageApp>();
      for (String translatorClassName : translatorClassNames)
      {
        apps.add(languageEntityToApp(translatorClassName));
      }
      return new LanguageAppsResponse(apps.toArray(new LanguageApp[]{}));
    }
    catch (Exception e)
    {
      throw new WebApplicationException(
        e, 
        Response.status(Status.INTERNAL_SERVER_ERROR)
         .entity("Error retrieving available language translator information.  Error: " + e.getMessage())
         .build());
    }
  }

  /**
   * Returns information about detectable languages given a set of language detectors.  

     * If no language detectors are specified, then all available language detectors are used to create the detectable language 
     information.  
     * Language names are returned URL encoded and in UTF-8.

     curl -X POST -H "Content-Type: application/json" -d '{}' localhost:8080/hoot-services/language/detectable
     curl -X POST -H "Content-Type: application/json" -d '{ "apps": ["TikaLanguageDetector"]}' localhost:8080/hoot-services/language/detectable

     {  
      "languages":[  
      {  
         "name":"Danish",
         "available":true,
         "iso6391code":"da",
         "iso6392code":"dan"
      },
      {  
         "name":"German",
         "available":true,
         "iso6391code":"de",
         "iso6392code":"deu"
      },
     ...
     ]
    }
   */
  @POST
  @Path("/detectable")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public SupportedLanguagesResponse getDetectableLangs(SupportedLanguagesRequest request)
  {
    try
    {
      Set<String> detectorClassNames = new HashSet<String>();
      if (request.getApps() != null && request.getApps().length > 0)
      {  
        detectorClassNames = new HashSet<String>(Arrays.asList(request.getApps()));
      }
      if (detectorClassNames.isEmpty())
      {
        detectorClassNames = LanguageDetectorFactory.getSimpleClassNames();
      }
      logger.trace("Listing detectable languages for apps: " + String.join(",", detectorClassNames.toArray(new String[]{})) + "..."); 
      return new SupportedLanguagesResponse(getAllAppSupportedLangs(detectorClassNames));
    }
    catch (Exception e)
    {
      Status status;
      if (e instanceof IllegalArgumentException)
      {
        status = Status.BAD_REQUEST;
      }
      else
      {
        status = Status.INTERNAL_SERVER_ERROR;
      } 
      throw new WebApplicationException(
        e, 
        Response.status(status)
          .entity("Error retrieving detectable language information for apps: " + String.join(",", request.getApps()) + ".  Error: " + 
                  e.getMessage())
          .build());
    }
  }

  /**
   * Returns information about translatable languages given a set of language translators.  

     * If no language translators are specified, then all available language translators are used to create the translatable 
     language information.  
     * Language names are returned URL encoded and in UTF-8.

     curl -X POST -H "Content-Type: application/json" -d '{}' localhost:8080/hoot-services/language/translatable
     curl -X POST -H "Content-Type: application/json" -d '{ "apps": ["JoshuaLanguageTranslator"]}' 
       localhost:8080/hoot-services/language/translatable

     {  
      "languages":[  
      {  
         "name":"Danish",
         "available":false,
         "iso6391code":"da",
         "iso6392code":"dan"
      },
      {  
         "name":"German",
         "available":true,
         "iso6391code":"de",
         "iso6392code":"deu"
      },
     ...
     ]
    }
   */
  @POST
  @Path("/translatable")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public SupportedLanguagesResponse getTranslatableLangs(SupportedLanguagesRequest request)
  {
    try
    {
      Set<String> translatorClassNames = new HashSet<String>();
      if (request.getApps() != null && request.getApps().length > 0)
      {  
        translatorClassNames = new HashSet<String>(Arrays.asList(request.getApps()));
      }
      if (translatorClassNames.isEmpty())
      {
        translatorClassNames = ToEnglishTranslatorFactory.getSimpleClassNames();
      }
      logger.trace("Listing translatable languages for apps: " + String.join(",", translatorClassNames.toArray(new String[]{})) + "...");
      return new SupportedLanguagesResponse(getAllAppSupportedLangs(translatorClassNames));
    }
    catch (Exception e)
    {
      Status status;
      if (e instanceof IllegalArgumentException)
      {
        status = Status.BAD_REQUEST;
      }
      else
      {
        status = Status.INTERNAL_SERVER_ERROR;
      } 
      throw new WebApplicationException(
        e, 
        Response.status(status)
          .entity("Error retrieving detectable language information for apps: " + String.join(",", request.getApps()) + ".  Error: " + 
                  e.getMessage())
          .build());
    }
  }

  /**
   * Detects a spoken language given text and a set of language detectors.  

     * If no language detectors are specified, then as many language detectors as needed are used to perform the detection.  
     * The text sent in for translation should be URL encoded and in UTF-8.  
     * Language names are returned URL encoded and in UTF-8.

     curl -X POST -H "Content-Type: application/json" -d '{ "text": "wie alt bist du" }' localhost:8080/hoot-services/language/detect
     curl -X POST -H "Content-Type: application/json" -d '{ "detectors": ["TikaLanguageDetector"], "text": "wie%20alt%20bist%20du" }' 
       localhost:8080/hoot-services/language/detect

     {
       "detectorUsed":"TikaLanguageDetector",
       "detectedLangCode":"de",
       "sourceText":"wie%20alt%20bist%20du",
       "detectedLang":"German"
     }
   */
  @POST
  @Path("/detect")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public Response detectLanguage(LanguageDetectRequest request)
  {
    String requestText = null;
    try
    {
      requestText = URLDecoder.decode(request.getText(), "UTF-8");
      logger.debug("Detecting language for text: " + StringUtils.left(requestText, 25) + "...");
      String detectedLangCode = "";
      String detectingDetector = "";
      String detectedLangName = "";
      Set<String> detectorClassNames = new HashSet<String>();
      if (request.getDetectors() != null && request.getDetectors().length > 0)
      {  
        detectorClassNames = new HashSet<String>(Arrays.asList(request.getDetectors()));
      }
      if (detectorClassNames.isEmpty())
      {
        detectorClassNames = LanguageDetectorFactory.getSimpleClassNames();
      }

      for (String detectorClassName : detectorClassNames)
      {
        if (detectedLangCode.isEmpty())
        {
          LanguageDetector detector = LanguageDetectorFactory.create(detectorClassName);
          detectingDetector = detectorClassName;
          detectedLangCode = detector.detect(requestText);
          if (!detectedLangCode.isEmpty())
          {
            detectedLangName = ((SupportedLanguageConsumer)detector).getLanguageName(detectedLangCode);
          }
        }
      }

      JSONObject entity = new JSONObject();
      entity.put("sourceText", requestText);
      entity.put("detectedLangCode", detectedLangCode);
      if (!detectedLangCode.isEmpty())
      {
        entity.put("detectedLang", URLEncoder.encode(detectedLangName, "UTF-8").replace("+", "%20"));
        entity.put("detectorUsed", detectingDetector);
      } 
      return Response.ok(entity.toJSONString()).build();
    }
    catch (Exception e)
    {
      Status status;
      if (e instanceof IllegalArgumentException)
      {
        status = Status.BAD_REQUEST;
      }
      else
      {
        status = Status.INTERNAL_SERVER_ERROR;
      } 
      throw new WebApplicationException(
        e, 
        Response.status(Status.INTERNAL_SERVER_ERROR)
          .entity(
            "Error detecting language with detector(s): " + String.join(",", request.getDetectors()) + "; error: " + e.getMessage() + 
            " text: " + request.getText())
          .build());
    }
  }

  /**
   * Translates a single line of text given a specified language translator and one or more source languages.  

     * When using JoshuaLanguageTranslator, an associated translation service must be running for each specified source language.
       See the Install Guide for more details.
     * In place of language codes, some translators support specifying "detect", which will cause the translator to attempt to 
     detect the language before translating (e.g. HootLanguageTranslator).  
     * The text sent in for translation should be URL encoded and in UTF-8.  
     * The translated text is returned URL encoded and in UTF-8.  
     * If the specified translator does not support detection, then any request parameters related to detection will be ignored.

     curl -X POST -H "Content-Type: application/json" -d '{ "translator": "JoshuaLanguageTranslator", "sourceLangCodes": ["de"], 
       "text": "wie%20alt%20bist%20du" }' localhost:8080/hoot-services/language/translate

     {
       "translatedText":"How%20old%20are%20you",
       "performExhaustiveTranslationSearchWithNoDetection":false,
       "sourceLangCodes":[
         "de"
       ],
       "translator":"JoshuaLanguageTranslator",
       "sourceText":"wie%20alt%20bist%20du",
       "detectedLanguageOverridesSpecifiedSourceLanguages":false
     }
   */
  @POST
  @Path("/translate")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public Response translate(LanguageTranslateRequest request) 
  {
    return translate(request, false);
  }

  /**
   * Translates multiple lines of text in batch given a specified language translator and one or more source languages.  

     * Only a single source language is supported.
     * When using JoshuaLanguageTranslator, an associated translation service must be the specified source language.
       See the Install Guide for more details.
     * No translator may be used that is integrated with language detection when translating in batch (e.g. HootLanguageTranslator).  
     * The text sent in for translation should have one line for each piece of text to be translated with all of text URL encoded 
       and in UTF-8.  
     * The translated text is returned translated line by line, URL encoded, and in UTF-8.  
     * An error will be thrown if a translator is specified that supports language detection.

     curl -X POST -H "Content-Type: application/json" -d '{ "translator": "JoshuaLanguageTranslator", "sourceLangCodes": ["de"], 
       "text": "wie%20alt%20bist%20du\nwie%20heissen%20sie" }' localhost:8080/hoot-services/language/translateBatch

     {
       "translatedText":"How%20old%20are%20you%0AWhat%27s%20your%20name%0A",
       "performExhaustiveTranslationSearchWithNoDetection":false,
       "sourceLangCodes":[
         "de"
       ],
       "translator":"JoshuaLanguageTranslator",
       "sourceText":"wie%20alt%20bist%20du\nwie%20heissen%20sie",
       "detectedLanguageOverridesSpecifiedSourceLanguages":false
     }
   */
  @POST
  @Path("/translateBatch")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public Response translateBatch(LanguageTranslateRequest request) 
  {
    return translate(request, true);
  }

  private Response translate(LanguageTranslateRequest request, boolean batch)
  {
    String textToTranslate = null;
    String responseStr = null;
    boolean badRequest = false;
    try
    {
      //could make this check more robust if many translators are added
      if (batch && request.getTranslator().toLowerCase().equals("hootlanguagetranslator"))
      {
        badRequest = true;
        throw new Exception("HootLanguageTranslator may not be used in batch translation.");
      }
      if (batch && request.getSourceLangCodes().length > 1)
      {
        badRequest = true;
        throw new Exception("Batch translation only supports one source language.");
      }

      textToTranslate = URLDecoder.decode(request.getText(), "UTF-8");
      if (!batch)
      {
        //This isn't batch translation, so remove all newlines and translate this as a single piece of text.
        textToTranslate = textToTranslate.replaceAll("\n", "");
      }
      logger.debug(
        "Translating language for text: " + StringUtils.left(textToTranslate, 25) + " and source languages: " + 
        String.join(",", request.getSourceLangCodes()) + "...");

      ToEnglishTranslator translator = ToEnglishTranslatorFactory.create(request.getTranslator());
      translator.setConfig(request);
      String translatedText = translator.translate(request.getSourceLangCodes(), textToTranslate);
      responseStr = toTranslateResponse(request, translatedText, translator);
    }
    catch (Exception e)
    {
      Status status;
      if (badRequest || e instanceof IllegalArgumentException)
      {
        status = Status.BAD_REQUEST;
      }
      else
      {
        status = Status.INTERNAL_SERVER_ERROR;
      } 
      throw new WebApplicationException(
        e, 
        Response.status(status)
          .entity(
            "Error translating with translator: " + request.getTranslator() + " to language (s): " + 
            String.join(",", request.getSourceLangCodes()) + ".  Error: " + e.getMessage() + "; text: " + textToTranslate)
          .build());
    }
    
    return Response.ok(responseStr).build();
  }

  private LanguageApp languageEntityToApp(String appName) throws Exception
  {
    LanguageAppInfo appInfo = null;
    try
    {
      appInfo = (LanguageAppInfo)ToEnglishTranslatorFactory.create(appName);
    }
    catch (Exception e)
    {
      appInfo = (LanguageAppInfo)LanguageDetectorFactory.create(appName);
    }
    assert(appInfo != null);

    LanguageApp app = new LanguageApp();
    app.setName(appName);
    app.setDescription(appInfo.getDescription());
    app.setUrl(appInfo.getUrl());
    return app;
  }

  private SupportedLanguage[] getAllAppSupportedLangs(Set<String> apps) throws Exception
  {
    Set<String> parsedLangCodes = new HashSet<String>();
    List<SupportedLanguage> supportedLangs = new ArrayList<SupportedLanguage>();
    for (String appName : apps)
    {
      logger.trace("appName: " + appName);
      SupportedLanguageConsumer langConsumer = null;
      try
      {
        langConsumer = (SupportedLanguageConsumer)ToEnglishTranslatorFactory.create(appName);
      }
      catch (Exception e)
      {
        langConsumer = (SupportedLanguageConsumer)LanguageDetectorFactory.create(appName);
      }
      assert(langConsumer != null);

      SupportedLanguage[] consumerSupportedLangs = langConsumer.getSupportedLanguages();
      logger.trace("consumerSupportedLangs size: " + consumerSupportedLangs.length);
      for (int i = 0; i < consumerSupportedLangs.length; i++)
      {
        SupportedLanguage lang = consumerSupportedLangs[i];
        logger.trace("lang code: " + lang.getIso6391code());
        if (!parsedLangCodes.contains(lang.getIso6391code()))
        {
          parsedLangCodes.add(lang.getIso6391code());
          lang.setAvailable(langConsumer.isLanguageAvailable(lang.getIso6391code()));
          lang.setName(URLEncoder.encode(lang.getName(), "UTF-8").replace("+", "%20"));
          supportedLangs.add(lang);
        }
      }
    }
    logger.trace("supportedLangs size: " + supportedLangs.size());
    return supportedLangs.toArray(new SupportedLanguage[]{});
  }

  private String toTranslateResponse(LanguageTranslateRequest request, String translatedText, ToEnglishTranslator translator) 
    throws UnsupportedEncodingException
  {
    JSONObject entity = new JSONObject();
    entity.put("sourceText", request.getText());
    JSONArray sourceLangCodes = new JSONArray();
    for (int i = 0; i < request.getSourceLangCodes().length; i++)
    {
      sourceLangCodes.add(request.getSourceLangCodes()[i]);
    }
    entity.put("sourceLangCodes", sourceLangCodes);
    entity.put("translatedText", URLEncoder.encode(translatedText, "UTF-8").replace("+", "%20"));
    entity.put("translator", request.getTranslator());
    if (translator instanceof LanguageDetectionConsumer)
    {
      LanguageDetectionConsumer detectionConsumer = (LanguageDetectionConsumer)translator;
      if (detectionConsumer.getDetectedLangCode() != null && !detectionConsumer.getDetectedLangCode().isEmpty())
      {
        entity.put("detectedLangCode", detectionConsumer.getDetectedLangCode());
        entity.put("detectedLang", ((SupportedLanguageConsumer)translator).getLanguageName(detectionConsumer.getDetectedLangCode()));
        entity.put("detectorUsed", detectionConsumer.getDetectorUsed());
        entity.put(
          "detectedLangAvailableForTranslation", 
          ((SupportedLanguageConsumer)translator).isLanguageAvailable(detectionConsumer.getDetectedLangCode()));
      }
    }
    entity.put("detectedLanguageOverridesSpecifiedSourceLanguages", request.getDetectedLanguageOverridesSpecifiedSourceLanguages());
    entity.put("performExhaustiveTranslationSearchWithNoDetection", request.getPerformExhaustiveTranslationSearchWithNoDetection());
    return entity.toJSONString();
  }
}

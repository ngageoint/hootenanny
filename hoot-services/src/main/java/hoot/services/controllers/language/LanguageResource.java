
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

import org.apache.commons.lang3.reflect.MethodUtils;
import org.apache.commons.lang3.StringUtils;

/*
 * 
 */
@Controller
@Path("")
public class LanguageResource 
{
  private static final Logger logger = LoggerFactory.getLogger(LanguageResource.class);

  public LanguageResource() throws Exception
  {
    //The Joshua init can take a long time, so let's do it here vs having it happen the first time a translation is made.
    MethodUtils.invokeStaticMethod(Class.forName("hoot.services.language.JoshuaLanguageTranslator"), "getInstance", null);
  }

  @POST
  @Path("/detectable")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public SupportedLanguagesResponse getDetectableLangs(SupportedLanguagesRequest request)
  {
    try
    {
      List<String> detectors = new ArrayList<String>();
      if (request.getApps() != null && request.getApps().length > 0)
      {  
        detectors = Arrays.asList(request.getApps());
      }
      if (detectors.isEmpty())
      {
        //TODO: add these with reflection
        detectors.add("TikaLanguageDetector");
        detectors.add("OpenNlpLanguageDetector");
      }
      logger.error("Listing detectable languages for apps: " + String.join(",", detectors.toArray(new String[]{})) + "..."); 
      return new SupportedLanguagesResponse(getAllAppSupportedLangs(detectors));
    }
    catch (Exception e)
    {
      throw new WebApplicationException(
        e, 
        Response.status(Status.INTERNAL_SERVER_ERROR).entity("Error retrieving detectable language information.  Error: " + e.getMessage())
          .build());
    }
  }

  @POST
  @Path("/translatable")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public SupportedLanguagesResponse getTranslatableLangs(SupportedLanguagesRequest request)
  {
    try
    {
      List<String> translators = new ArrayList<String>();
      if (request.getApps() != null && request.getApps().length > 0)
      {  
        translators = Arrays.asList(request.getApps());
      }
      if (translators.isEmpty())
      {
        //TODO: add these with reflection
        translators.add("JoshuaLanguageTranslator");
        translators.add("HootLanguageTranslator");
      }
      logger.error("Listing translatable languages for apps: " + String.join(",", translators.toArray(new String[]{})) + "...");
      return new SupportedLanguagesResponse(getAllAppSupportedLangs(translators));
    }
    catch (Exception e)
    {
      throw new WebApplicationException(
        e, 
        Response.status(Status.INTERNAL_SERVER_ERROR).entity("Error retrieving translatable language information.  Error: " + e.getMessage())
          .build());
    }
  }

  private SupportedLanguage[] getAllAppSupportedLangs(List<String> apps) throws Exception
  {
    Set<String> parsedLangCodes = new HashSet<String>();
    List<SupportedLanguage> supportedLangs = new ArrayList<SupportedLanguage>();
    for (String appName : apps)
    {
      logger.error("appName: " + appName);
      SupportedLanguageConsumer langConsumer = null;
      try
      {
        ToEnglishTranslator translator = ToEnglishTranslatorFactory.create(appName);
        langConsumer = (SupportedLanguageConsumer)translator;
      }
      catch (Exception e)
      {
        LanguageDetector detector = LanguageDetectorFactory.create(appName);
        langConsumer = (SupportedLanguageConsumer)detector;
      }
      assert(langConsumer != null);

      SupportedLanguage[] consumerSupportedLangs = langConsumer.getSupportedLanguages();
      logger.error("consumerSupportedLangs size: " + consumerSupportedLangs.length);
      for (int i = 0; i < consumerSupportedLangs.length; i++)
      {
        SupportedLanguage lang = consumerSupportedLangs[i];
        logger.trace("lang code: " + lang.getIso6391code());
        if (!parsedLangCodes.contains(lang.getIso6391code()))
        {
          parsedLangCodes.add(lang.getIso6391code());
          lang.setAvailable(langConsumer.isLanguageAvailable(lang.getIso6391code()));
          supportedLangs.add(lang);
        }
      }
    }
    logger.error("supportedLangs size: " + supportedLangs.size());
    return supportedLangs.toArray(new SupportedLanguage[]{});
  }

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
      logger.error("Detecting language for text: " + StringUtils.left(requestText, 25) + "...");
      String detectedLangCode = "";
      String detectingDetector = "";
      String detectedLangName = "";
      List<String> detectors = new ArrayList<String>();
      if (request.getDetectors() != null && request.getDetectors().length > 0)
      {  
        detectors = Arrays.asList(request.getDetectors());
      }
      if (detectors.isEmpty())
      {
        //TODO: add these with reflection
        detectors.add("TikaLanguageDetector");
        detectors.add("OpenNlpLanguageDetector");
      }

      for (String detectorName : detectors)
      {
        if (detectedLangCode.isEmpty())
        {
          LanguageDetector detector = LanguageDetectorFactory.create(detectorName);
          detectingDetector = detectorName;
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
        entity.put("detectedLang", detectedLangName);
        entity.put("detectorUsed", detectingDetector);
      } 
      return Response.ok(entity.toJSONString()).build();
    }
    catch (Exception e)
    {
      throw new WebApplicationException(
        e, 
        Response.status(Status.INTERNAL_SERVER_ERROR)
          .entity(
            "Error detecting language with detector(s): " + String.join(",", request.getDetectors()) + "; error: " + e.getMessage() + 
            " text: " + request.getText())
          .build());
    }
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
    //TODO: replace is a hack
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

  @POST
  @Path("/translate")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public Response translate(LanguageTranslateRequest request) 
  {
    String textToTranslate = null;
    String responseStr = null;
    try
    {
      textToTranslate = URLDecoder.decode(request.getText(), "UTF-8");
      logger.error(
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
      //TODO: handle more exceptions
      if (e.getMessage().startsWith("No language translator available") || 
          e.getMessage().startsWith("Requested unsupported translation language"))
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
            String.join(",", request.getSourceLangCodes()) + ".  Error: " + 
            e.getMessage() + "; text: " + textToTranslate)
          .build());
    }
    
    return Response.ok(responseStr).build();
  }
}

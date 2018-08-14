
package hoot.services.controllers.language;

import java.io.IOException;
import java.util.List;
import java.util.Arrays;

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
import hoot.services.language.SupportedLanguages;

import org.apache.commons.lang3.reflect.MethodUtils;

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
    //The Joshua init can take a long time, so let's do it here vs having it happen the first time a translation is made (I think).
    MethodUtils.invokeStaticMethod(Class.forName("hoot.services.language.JoshuaLanguageTranslator"), "getInstance", null);
  }

  @GET
  @Path("/supportedLanguages")
  @Produces(MediaType.APPLICATION_JSON)
  public SupportedLanguagesResponse getSupportedLangs()
  {
    try
    {
      return new SupportedLanguagesResponse(SupportedLanguages.getInstance().getSupportedLanguages());
    }
    catch (Exception e)
    {
      throw new WebApplicationException(
        e, 
        Response.status(Status.INTERNAL_SERVER_ERROR).entity("Error retrieving supported language information.  Error: " + e.getMessage())
          .build());
    }
  }

  @POST
  @Path("/detect")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public Response detectLanguage(LanguageDetectRequest request)
  {
    try
    {
      logger.error(String.join(",", request.getDetectors()));
      logger.error(request.getText());

      String detectedLang = "";
      String detectingDetector = "";
      List<String> detectors = Arrays.asList(request.getDetectors());
      if (detectors.isEmpty())
      {
        //TODO: add these with reflection
        detectors.add("TikaLanguageDetector");
        detectors.add("OpenNlpLanguageDetector");
      }

      for (String detector : detectors)
      {
        if (detectedLang.isEmpty())
        {
          detectedLang = LanguageDetectorFactory.create(detector).detect(request.getText());
        }
      }

      JSONObject entity = new JSONObject();
      entity.put("sourceText", request.getText());
      entity.put("detectedLang", detectedLang);
      entity.put("detectingDetector", detectingDetector);
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

  @POST
  @Path("/translate")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public Response translate(LanguageTranslateRequest request) 
  {
    logger.error("text: " + request.getText());
    logger.error("detectors: " + String.join(",", request.getDetectors());
    logger.error("sourceLangCodes: " + String.join(",", request.getSourceLangCodes());
    logger.error("translator: " + request.getTranslator());
    logger.error("detectedLanguageOverridesSpecifiedSourceLanguages: " + request.getDetectedLanguageOverridesSpecifiedSourceLanguages());
    logger.error("performExhaustiveTranslationSearchWithNoDetection: " + request.getPerformExhaustiveTranslationSearchWithNoDetection());

    if (!SupportedLanguages.getInstance().isSupportedLanguage(request.getSourceLangCode().toLowerCase()))
    {
      throw new WebApplicationException(
        Response.status(Status.BAD_REQUEST).entity("Requested unsupported translation language: " + request.getSourceLangCode()).build());
    }

    String translatedText = "";
    try
    {
      ToEnglishTranslator translator = ToEnglishTranslatorFactory.create(request.getTranslator());
      translator.setConfig(request);
      translatedText = translator.translate(request.getSourceLangCode(), request.getText());
    }
    catch (Exception e)
    {
      Status status;
      if (e.getMessage().startsWith("No language translator available"))
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
            "Error translating with translator: " + request.getTranslator() + " to language: " + request.getSourceLangCode() + ".  Error: " + 
            e.getMessage() + "; text: " + request.getText())
          .build());
    }
    
    JSONObject entity = new JSONObject();
    entity.put("sourceText", request.getText());
    entity.put("sourceLangCodes", request.getSourceLangCodes());
    entity.put("translatedText", translatedText);
    entity.put("translator", request.getTranslator());
    if (!translator.getDetectedLanguage().isEmpty())
    {
      entity.put("detectedLanguage", translator.getDetectedLanguage());
    }
    entity.put("detectedLanguageOverridesSpecifiedSourceLanguages", request.getDetectedLanguageOverridesSpecifiedSourceLanguages());
    entity.put("performExhaustiveTranslationSearchWithNoDetection", request.getPerformExhaustiveTranslationSearchWithNoDetections());
    return Response.ok(entity.toJSONString()).build();
  }
}

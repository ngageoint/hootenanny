
package hoot.services.controllers.language;

import opennlp.tools.langdetect.LanguageDetectorModel;
import opennlp.tools.langdetect.LanguageDetectorME;
import opennlp.tools.langdetect.Language;

import java.io.IOException;
import java.io.File;
import java.io.InputStream;
import java.util.List;
import java.util.Arrays;

import org.apache.tika.langdetect.OptimaizeLangDetector;
import org.apache.tika.language.detect.LanguageResult;
import org.apache.tika.exception.TikaException;

import org.xml.sax.SAXException;

import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.inject.Singleton;
import javax.ws.rs.DefaultValue;
import javax.ws.rs.Consumes;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;

/**
 * http://tika.apache.org/
 * http://opennlp.apache.org/

@article{post2015joshua,
    Author = {Post, Matt and Cao, Yuan and Kumar, Gaurav},
    Journal = {The Prague Bulletin of Mathematical Linguistics},
    Title = {Joshua 6: A phrase-based and hierarchical statistical machine translation system},
    Year = {2015} }
 */
@Controller
@Path("")
@Singleton  //TODO: need to research this more
public class LanguageResource 
{
  private static final Logger logger = LoggerFactory.getLogger(LanguageResource.class);

  private org.apache.tika.language.detect.LanguageDetector tikaLangDetector = new OptimaizeLangDetector();

  private LanguageDetectorME opennlpLangDetector = null;
  //TODO: move to config
  private static final String OPEN_NLP_MODEL = "language-translation/langdetect-183.bin";

  public LanguageResource() throws RuntimeException, IOException
  {
    //fairly expensive initialization

    try
    {
      logger.debug("loading tika model...");
      tikaLangDetector.loadModels();
    }
    catch (IOException ioe) 
    {
      throw new RuntimeException("Error reading Tika model as resource stream", ioe);
    }
    
    InputStream modelStrm = null;
    try 
    {
      logger.debug("loading opennlp model...");
      modelStrm = LanguageResource.class.getClassLoader().getResourceAsStream(OPEN_NLP_MODEL); 
      opennlpLangDetector = new LanguageDetectorME(new LanguageDetectorModel(modelStrm));
    }
    catch (IOException ioe) 
    {
      if (modelStrm != null)
      {
        modelStrm.close();
      }
      throw new RuntimeException("Error reading OpenNLP model as resource stream", ioe);
    }
  }

  @POST
  @Path("/detect")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public Response detectLanguage(LanguageDetectRequest request) throws IOException, Exception
  {
    logger.debug(String.join(",", request.getApps()));
    logger.debug(request.getText());

    String detectedLang = "";
    String detectingApp;
    List<String> apps = Arrays.asList(request.getApps());
    if (apps.isEmpty())
    {
      apps.add("tika");
      apps.add("opennlp");
    }

    //TODO: use factory with detectors
    for (String app : request.getApps())
    {
      if (detectedLang.isEmpty())
      {
        logger.debug("detecting with " + app + "...");
        if (app.equalsIgnoreCase("tika"))
        {
          detectedLang = tikaLangDetector.detect(request.getText()).getLanguage();
          detectingApp = "tika";
          logger.debug("Tika detected: " + detectedLang);
        }
        else if (app.equalsIgnoreCase("tika"))
        {
          detectedLang = opennlpLangDetector.predictLanguage(request.getText()).getLang();
          detectingApp = "opennlp";
          logger.debug("OpenNLP detected: " + detectedLang);
        }
      }
    }
    
    //tika is better but supports fewer languages
    /*if (apps.isEmpty() || apps.contains("tika"))
    {
      logger.debug("detecting with tika...");
      detectedLang = tikaLangDetector.detect(request.getText()).getLanguage();
      detectingApp = "tika";
      logger.debug("Tika detected: " + detectedLang);
    }
    if (detectedLang.isEmpty() && (apps.isEmpty() || apps.contains("opennlp")))
    {
      logger.debug("detecting with opennlp...");
      detectedLang = opennlpLangDetector.predictLanguage(request.getText()).getLang();
      detectingApp = "opennlp";
      logger.debug("OpenNLP detected: " + detectedLang);
    }*/

    JSONObject entity = new JSONObject();
    entity.put("sourceText", request.getText());
    entity.put("detectedLang", detectedLang);
    entity.put("detectingApp", detectingApp);
    return Response.ok(entity.toJSONString()).build();
  }

  /*@POST
  @Path("/translate")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public Response translate(LanguageTranslateRequest request) 
  {
    String translatedText = "";

    //TODO: finish

    JSONObject entity = new JSONObject();
    entity.put("sourceLangCode", sourceLangCode); //TODO: return country name
    entity.put("sourceText", text);
    entity.put("translatedText", translatedText);
    return Response.ok(entity.toJSONString()).build();
  }

  @POST
  @Path("/translateWithDetect")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public Response translate(LanguageTranslateWithDetectRequest request) 
  {
    String translatedText = "";

    //TODO: finish

    JSONObject entity = new JSONObject();
    entity.put("sourceLangCode", sourceLangCode); //TODO: return country name
    entity.put("sourceText", text);
    entity.put("translatedText", translatedText);
    return Response.ok(entity.toJSONString()).build();
  }*/
}

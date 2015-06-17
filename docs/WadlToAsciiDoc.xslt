<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:ns2="http://wadl.dev.java.net/2009/02" 
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:exsl="http://exslt.org/common"
  extension-element-prefixes="exsl">
  <xsl:output method="text" indent="no"/>
  <xsl:strip-space elements="*"/>
  <xsl:preserve-space elements="pre"/>
  
  <xsl:template match="text()">
    <xsl:value-of select="normalize-space(.)"/>
  </xsl:template>
 
  <xsl:template match="/">
    <xsl:apply-templates/>
    <xsl:text>&#xA;</xsl:text>
  </xsl:template>
 
  <xsl:template match="comment()">
  </xsl:template>

  <!-- Global variables --> 
  <xsl:variable name="base" select="ns2:application/ns2:resources/@base"/>

  <!-- top level header query to parse title -->
  <xsl:template match="ns2:application/ns2:doc">
    <xsl:call-template name="titleHandler"/>
  </xsl:template>

  <!-- titleHandler: Process the title -->
  <xsl:template name="titleHandler">
    <xsl:if test="@title">
    <!--   <xsl:text>&#xA;= </xsl:text>
      <xsl:apply-templates select="@title"/> -->
      <xsl:text>&#xA;&#xA;</xsl:text>
    </xsl:if>
  </xsl:template> 

  <!-- body level query to parse resources -->
  <xsl:template match="ns2:application/ns2:resources">
    <xsl:call-template name="resourcesHandler"/>
  </xsl:template>

  <!-- resourcesHandler: Process the resources -->
  <xsl:template name="resourcesHandler">
    <xsl:if test="./ns2:resource">
      <xsl:apply-templates select="./ns2:resource"/>
    </xsl:if>
  </xsl:template> 

  <!-- resourceHandler: Process the resource tags at level 1 and 2-->
  <xsl:template match="ns2:resource">
    <xsl:if test="./ns2:method">
      <xsl:apply-templates select="./ns2:method"/>
    </xsl:if>
    <xsl:if test="./ns2:resource/ns2:method">
      <xsl:apply-templates select="./ns2:resource/ns2:method"/>
    </xsl:if>
  </xsl:template> 

  <!-- methodHandler: Process the method tags -->
  <xsl:template match="ns2:method">
    <!-- Extract comment data -->
    <xsl:variable name="cdata" select="ns2:doc"/>
    <xsl:variable name="parsedXml_">
      <xsl:call-template name="parseXml">
        <xsl:with-param name="text" select="$cdata"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="parsedXml" select="exsl:node-set($parsedXml_)"/>
    <xsl:call-template name="xmlCommentHandler">
      <xsl:with-param name="xmlText" select="$parsedXml"/>
    </xsl:call-template>

    <xsl:choose>
      <xsl:when test="$parsedXml/NAME">

        <!-- Miscellaneous section parsed from rest of WADL -->
        <xsl:text>&#xA;*Miscellaneous:* </xsl:text>
        <xsl:text>&#xA;&#xA;</xsl:text>
        <xsl:text>_ID:_ </xsl:text>
        <xsl:value-of select="@id"/>
        <xsl:text>&#xA;&#xA;</xsl:text>
        <xsl:text>_Request Parameters:_ </xsl:text>
        <xsl:text>&#xA;</xsl:text>
        <xsl:apply-templates select="./ns2:request"/>
        <xsl:text>&#xA;</xsl:text>
        <xsl:text>_Response Parameters:_ </xsl:text>
        <xsl:text>&#xA;</xsl:text>
        <xsl:apply-templates select="./ns2:response"/>
        <xsl:text>&#xA;</xsl:text>

      </xsl:when>
      <xsl:otherwise>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- Handler for request sections -->
  <xsl:template match="ns2:request">
    <xsl:text>&#xA;----</xsl:text>
    <xsl:text>&#xA;</xsl:text>

    <!-- Check for representation section -->
    <xsl:if test="./ns2:representation">
      <xsl:text>Content Type = </xsl:text>
      <xsl:value-of select="./ns2:representation/@mediaType"/>
      <xsl:text>&#xA;&#xA;</xsl:text>
    </xsl:if>

    <!-- Check for param sections -->
    <xsl:apply-templates select="./ns2:param"/>

    <xsl:text>----</xsl:text>
    <xsl:text>&#xA;</xsl:text>
  </xsl:template>

  <!-- Handler for param sections -->
  <xsl:template match="ns2:param">
    <xsl:text>Param: {name = </xsl:text>
    <xsl:value-of select="@name"/>
    <xsl:text>, style = </xsl:text>
    <xsl:value-of select="@style"/>
    <xsl:text>, type = </xsl:text>
    <xsl:value-of select="@type"/>
    <xsl:text>}</xsl:text>
    <xsl:text>&#xA;</xsl:text>
  </xsl:template>

  <!-- Handler for response section -->
  <xsl:template match="ns2:response">
    <xsl:text>&#xA;----</xsl:text>
    <xsl:text>&#xA;</xsl:text>

    <!-- Check for representation section -->
    <xsl:if test="./ns2:representation">
      <xsl:text>Content Type = </xsl:text>
      <xsl:value-of select="./ns2:representation/@mediaType"/>
      <xsl:text>&#xA;&#xA;</xsl:text>
    </xsl:if>

    <xsl:text>----</xsl:text>
    <xsl:text>&#xA;</xsl:text>
  </xsl:template>


  <!-- xmlCommentHandler: Parses through the CDATA xml code to get section title, description, etc. -->
  <xsl:template name="xmlCommentHandler">
    <xsl:param name="xmlText"/>

    <!-- Extract section "name" -->
    <xsl:choose>
      <xsl:when test="$xmlText/NAME">
        <xsl:text>&#xA;&#xA;== </xsl:text>
        <xsl:value-of select="$xmlText/NAME"/>
        <xsl:text>&#xA;&#xA;</xsl:text>

        <!-- Extract section "description" -->
        <xsl:choose>
          <xsl:when test="$xmlText/DESCRIPTION">
            <xsl:value-of select="normalize-space($xmlText/DESCRIPTION)"/>
            <xsl:text>&#xA;&#xA;</xsl:text>
          </xsl:when>
          <xsl:otherwise>
            <xsl:text>!!!Section DESCRIPTION needed from WADL generator!!! &#xA; </xsl:text>
            <xsl:text>&#xA;&#xA;</xsl:text>
          </xsl:otherwise>
        </xsl:choose>

        <!-- Extract section "parameters" -->
        <xsl:text>*Parameters* </xsl:text>
        <xsl:text>&#xA;&#xA;</xsl:text>
        <xsl:choose>
          <xsl:when test="$xmlText/PARAMETERS">
            <xsl:text>----</xsl:text>
            <xsl:text>&#xA;</xsl:text>

            <xsl:choose>
              <xsl:when test="count($xmlText/PARAMETERS/*)=0">
                <xsl:text>N/A</xsl:text>
              </xsl:when>
              <xsl:otherwise>
                <xsl:for-each select="$xmlText/PARAMETERS/*">
                  <xsl:value-of select="local-name()"/>
                  <xsl:text> = </xsl:text>
                  <xsl:value-of select="normalize-space(.)"/>
                  <xsl:text>&#xA;</xsl:text>
                </xsl:for-each>
              </xsl:otherwise>
            </xsl:choose>

            <xsl:text>&#xA;</xsl:text>
            <xsl:text>----</xsl:text>
            <xsl:text>&#xA;</xsl:text>
          </xsl:when>
          <xsl:otherwise>
            <xsl:text>!!!Section PARAMETERS needed from WADL generator!!! &#xA; </xsl:text>
            <xsl:text>&#xA;&#xA;</xsl:text>
          </xsl:otherwise>
        </xsl:choose>

        <!-- Extract section "Output" -->
        <xsl:text>*Output* </xsl:text>
        <xsl:text>&#xA;&#xA;</xsl:text>
        <xsl:choose>
          <xsl:when test="$xmlText/OUTPUT">
            <xsl:text>----</xsl:text>
            <xsl:text>&#xA;</xsl:text>
            <xsl:value-of select="normalize-space($xmlText/OUTPUT)"/>
            <xsl:text>&#xA;&#xA;</xsl:text>
            <xsl:text>----</xsl:text>
            <xsl:text>&#xA;</xsl:text>
          </xsl:when>
          <xsl:otherwise>
            <xsl:text>!!!Section OUTPUT needed from WADL generator!!! &#xA; </xsl:text>
            <xsl:text>&#xA;&#xA;</xsl:text>
          </xsl:otherwise>
        </xsl:choose>

        <!-- Output the URL -->
        <xsl:text>*URL:* </xsl:text>
        <xsl:text>&#xA;&#xA;</xsl:text>
        <xsl:text>----</xsl:text>
        <xsl:text>&#xA;</xsl:text>
        <xsl:value-of select="$base"/>
        <xsl:if test="../../ns2:resource">
          <xsl:value-of select="../../@path"/>
        </xsl:if>
        <xsl:value-of select="../@path"/>
        <xsl:text>&#xA;&#xA;</xsl:text>
        <xsl:text>----</xsl:text>
        <xsl:text>&#xA;</xsl:text>

        <!-- Output the Request Type -->
        <xsl:text>*Request Type:* </xsl:text>
        <xsl:value-of select="./@name"/>
        <xsl:text>&#xA;&#xA;</xsl:text>

        <!-- Extract section "Example" -->
        <xsl:text>*Example* </xsl:text>
        <xsl:text>&#xA;&#xA;</xsl:text>
        <xsl:choose>
          <xsl:when test="not($xmlText/EXAMPLE)">
            <xsl:text>!!!Section EXAMPLE needed from WADL generator!!! &#xA; </xsl:text>
            <xsl:text>&#xA;&#xA;</xsl:text>
          </xsl:when>
          <xsl:otherwise>
            <!-- Extract section "Example Input" -->
            <xsl:text>_Input:_ </xsl:text>
            <xsl:text>&#xA;&#xA;</xsl:text>
            <xsl:choose>
              <xsl:when test="$xmlText/EXAMPLE/INPUT">
                <xsl:text>----</xsl:text>
                <xsl:text>&#xA;</xsl:text>

                <xsl:choose>
                  <xsl:when test="string-length(normalize-space($xmlText/EXAMPLE/INPUT))=0">
                    <xsl:text>N/A</xsl:text>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="$xmlText/EXAMPLE/INPUT"/>
                  </xsl:otherwise>
                </xsl:choose>

                <xsl:text>&#xA;</xsl:text>
                <xsl:text>----</xsl:text>
                <xsl:text>&#xA;</xsl:text>
              </xsl:when>
              <xsl:otherwise>
                <xsl:text>!!!Section EXAMPLE/INPUT needed from WADL generator!!! &#xA; </xsl:text>
                <xsl:text>&#xA;&#xA;</xsl:text>
              </xsl:otherwise>
            </xsl:choose>
            <!-- Extract section "Example Output" -->
            <xsl:text>_Output:_ </xsl:text>
            <xsl:text>&#xA;&#xA;</xsl:text>
            <xsl:choose>
              <xsl:when test="$xmlText/EXAMPLE/OUTPUT">
                <xsl:text>----</xsl:text>
                <xsl:text>&#xA;</xsl:text>
                <xsl:value-of select="$xmlText/EXAMPLE/OUTPUT"/>
                <xsl:text>&#xA;</xsl:text>
                <xsl:text>----</xsl:text>
                <xsl:text>&#xA;</xsl:text>
              </xsl:when>
              <xsl:otherwise>
                <xsl:text>!!!Section EXAMPLE/OUTPUT needed from WADL generator!!! &#xA; </xsl:text>
                <xsl:text>&#xA;&#xA;</xsl:text>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:otherwise>
        </xsl:choose>

      </xsl:when> <!-- end of section name -->

      <xsl:otherwise>
        <!-- <xsl:text>!!!Section NAME needed from WADL generator!!! &#xA; </xsl:text>
        <xsl:text>&#xA;&#xA;</xsl:text> -->
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>
       
  <!-- PARSE XML is a set of tiered templates to parse the inner contents of a CDATA 
       comment block that contains XML -->
  <xsl:template name="parseXml">
    <xsl:param name="text"/>
    <xsl:choose>
      <xsl:when test="contains($text, '&gt;')">
        <xsl:variable name="topLevelTag">
          <xsl:call-template name="getTopLevelTag">
            <xsl:with-param name="text" select="$text"/>
          </xsl:call-template>
        </xsl:variable>
        <xsl:variable name="openingTag">
          <xsl:value-of select="$topLevelTag"/>
        </xsl:variable>
        <xsl:variable name="tagName">
          <xsl:call-template name="getTopLevelTagName">
            <xsl:with-param name="text" select="$text"/>
          </xsl:call-template>
        </xsl:variable>
        <xsl:variable name="closingTag">
          <xsl:value-of select="concat('&lt;/',$tagName,'&gt;')"/>
        </xsl:variable>
        <xsl:variable name="firstNode">
          <xsl:if test="not(contains($topLevelTag,'/&gt;'))">
            <xsl:value-of select="substring-before(substring-after($text,$openingTag),$closingTag)"/>        
          </xsl:if>
        </xsl:variable>
        <xsl:variable name="afterFirstNode">
          <xsl:choose>
            <xsl:when test="not(contains($topLevelTag,'/&gt;'))">
              <xsl:value-of select="substring-after($text,concat($firstNode,$closingTag))"/>        
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="substring-after($text,$topLevelTag)"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>
        <xsl:element name="{$tagName}">
          <xsl:call-template name="createAttributes">
            <xsl:with-param name="text" select="$topLevelTag"/>
          </xsl:call-template>
          <xsl:call-template name="parseXml">
            <xsl:with-param name="text" select="$firstNode"/>
          </xsl:call-template>
        </xsl:element>
        <xsl:call-template name="parseXml">
          <xsl:with-param name="text" select="$afterFirstNode"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$text"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- getTopLevelTagName parse the top (root) layer inside the CDATA -->
  <xsl:template name="getTopLevelTagName">
    <xsl:param name="text"/>
    <xsl:choose>
      <xsl:when test="contains($text, '&gt;')">
        <xsl:variable name="tagWithAttributesWithoutEnd">
          <xsl:value-of select="substring-before($text, '&gt;')"/>
        </xsl:variable>
        <xsl:variable name="tagWithAttributesWithoutBegining">
          <xsl:value-of select="substring-after($tagWithAttributesWithoutEnd, '&lt;')"/>
        </xsl:variable>
        <xsl:variable name="tagName">
          <xsl:choose>
            <xsl:when test="contains($tagWithAttributesWithoutBegining,' ')">
              <xsl:value-of
                select="substring-before($tagWithAttributesWithoutBegining, ' ')"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$tagWithAttributesWithoutBegining"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>
        <xsl:value-of select="$tagName"/>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- getTopLevelTag gets the tag data -->
  <xsl:template name="getTopLevelTag">
    <xsl:param name="text"/>
    <xsl:choose>
      <xsl:when test="contains($text, '&gt;')">
        <xsl:variable name="tagWithAttributesWithoutEnd">
          <xsl:value-of select="substring-before($text, '&gt;')"/>
        </xsl:variable>
        <xsl:value-of select="concat($tagWithAttributesWithoutEnd,'&gt;')"/>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- createAttributes creates the attirbutes for the nodes -->
  <xsl:template name="createAttributes">
    <xsl:param name="text"/>
    <xsl:choose>
      <xsl:when test="contains($text, '=&quot;')">
        <xsl:variable name="attributeName">
          <xsl:value-of select="substring-before(substring-after($text,' '),'=&quot;')"/>
        </xsl:variable>
        <xsl:message>
          <xsl:value-of select="$text"/>
        </xsl:message>
        <xsl:variable name="attributeValue">
          <xsl:value-of select="substring-before(substring-after($text,concat($attributeName,'=&quot;')),'&quot;')"/>
        </xsl:variable>
        <xsl:attribute name="{$attributeName}">
          <xsl:value-of select="$attributeValue"/>
        </xsl:attribute>
        <xsl:call-template name="createAttributes">
          <xsl:with-param name="text" select="substring-after($text,concat($attributeName,'=&quot;',$attributeValue,'&quot;'))"/>
        </xsl:call-template>
      </xsl:when>
    </xsl:choose>        
  </xsl:template>

</xsl:stylesheet>

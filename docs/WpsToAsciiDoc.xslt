<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:wps="http://www.deegree.org/processes/java"
  xmlns:exsl="http://exslt.org/common"
  extension-element-prefixes="exsl"
  version="1.0">

  <xsl:output method="text" indent="no"/>
  <xsl:strip-space elements="*"/>
  <xsl:preserve-space elements="pre"/>

  <xsl:template match="text()">
    <xsl:value-of select="normalize-space(.)"/>
  </xsl:template>

  <xsl:template match="comment()">
  </xsl:template>

  <!-- Title extraction -->
  <xsl:template match="wps:ProcessDefinition">
    <xsl:call-template name="titleHandler"/>
    <xsl:apply-templates select="./wps:Abstract"/>
  </xsl:template>

  <xsl:template name="titleHandler">
    <xsl:text>&#xA;&#xA;</xsl:text>
    <xsl:text>== </xsl:text>
    <xsl:value-of select="wps:Title"/>
    <xsl:text>&#xA;&#xA;</xsl:text>
  </xsl:template>

  <!-- Process Abstract section which contains CDATA embedded with XML content -->
  <xsl:template match="wps:Abstract">
    <!-- Extract comment data -->
    <xsl:variable name="cdata" select="."/>
    <xsl:variable name="parsedXml_">
      <xsl:call-template name="parseXml">
        <xsl:with-param name="text" select="$cdata"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="parsedXml" select="exsl:node-set($parsedXml_)"/>
    <xsl:call-template name="xmlCommentHandler1">
      <xsl:with-param name="xmlText" select="$parsedXml"/>
    </xsl:call-template>
    <xsl:apply-templates select="../wps:InputParameters"/>
    <xsl:apply-templates select="../wps:OutputParameters"/>
    <xsl:call-template name="xmlCommentHandler2">
      <xsl:with-param name="xmlText" select="$parsedXml"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="wps:InputParameters">
    <xsl:text>*Parameters* </xsl:text>
    <xsl:text>&#xA;&#xA;</xsl:text>
    <xsl:text>----</xsl:text>
    <xsl:text>&#xA;</xsl:text>
    <xsl:for-each select="./wps:LiteralInput">
      <xsl:value-of select="./wps:Title"/>
      <xsl:text> = </xsl:text>
      <xsl:value-of select="./wps:Abstract"/>
      <xsl:text>&#xA;</xsl:text>
    </xsl:for-each>
    <xsl:text>----</xsl:text>
    <xsl:text>&#xA;</xsl:text>
  </xsl:template>

  <xsl:template match="wps:OutputParameters">
    <xsl:text>*Output* </xsl:text>
    <xsl:text>&#xA;&#xA;</xsl:text>
    <xsl:text>----</xsl:text>
    <xsl:text>&#xA;</xsl:text>
      <xsl:choose>
        <xsl:when test="./wps:LiteralOutput">
          <xsl:value-of select="./wps:LiteralOutput/wps:Title"/>
        </xsl:when>
        <xsl:when test="./wps:ComplexOutput">
          <xsl:value-of select="./wps:ComplexOutput/wps:Title"/>
        </xsl:when>
      </xsl:choose>
      <xsl:text>&#xA;</xsl:text>
    <xsl:text>----</xsl:text>
    <xsl:text>&#xA;</xsl:text>
  </xsl:template>


<!--    <xsl:template match="/wps:ProcessDefinition/wps:Identifier">
    </xsl:template>
    <xsl:template match="/wps:ProcessDefinition/wps:JavaClass">
    </xsl:template>
-->
    <xsl:template match="/wps:InputParameters/wps:LiteralInput/wps:Title">
      <xsl:text>&#xA;</xsl:text>
      <xsl:text>&#xA;</xsl:text>
      <xsl:text>== </xsl:text>
      <xsl:value-of select="text()"/>
      <xsl:text>&#xA;</xsl:text>
    </xsl:template>

  <!-- xmlCommentHandler 1: Parses through the CDATA xml code to get section title, description, etc. -->
  <xsl:template name="xmlCommentHandler1">
    <xsl:param name="xmlText"/>

    <!-- Extract section "description" -->
    <xsl:choose>
      <xsl:when test="$xmlText/DESCRIPTION">
        <xsl:value-of select="normalize-space($xmlText/DESCRIPTION)"/>
        <xsl:text>&#xA;&#xA;</xsl:text>
      </xsl:when>
      <xsl:otherwise>
        <xsl:text>!!!Section DESCRIPTION needed from WPS generator!!! &#xA; </xsl:text>
        <xsl:text>&#xA;&#xA;</xsl:text>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- xmlCommentHandler 2: Parses through the CDATA xml code to get section title, description, etc. -->
  <xsl:template name="xmlCommentHandler2">
    <xsl:param name="xmlText"/>

    <!-- Extract section "Example" -->
    <xsl:choose>
      <xsl:when test="not($xmlText/EXAMPLE)">
        <xsl:text>!!!Section EXAMPLE needed from WPS generator!!! &#xA; </xsl:text>
        <xsl:text>&#xA;&#xA;</xsl:text>
      </xsl:when>
      <xsl:otherwise>

        <!-- Extract section "URL" -->
        <xsl:choose>
          <xsl:when test="$xmlText/EXAMPLE/URL">
            <xsl:text>*URL:* </xsl:text>
            <xsl:value-of select="normalize-space($xmlText/EXAMPLE/URL)"/>
            <xsl:text>&#xA;&#xA;</xsl:text>
          </xsl:when>
          <xsl:otherwise>
            <xsl:text>!!!Section URL needed from WPS generator!!! &#xA; </xsl:text>
            <xsl:text>&#xA;&#xA;</xsl:text>
          </xsl:otherwise>
        </xsl:choose>

        <!-- Extract section "REQUEST_TYPE" -->
        <xsl:choose>
          <xsl:when test="$xmlText/EXAMPLE/REQUEST_TYPE">
            <xsl:text>*Request Type:* </xsl:text>
            <xsl:value-of select="normalize-space($xmlText/EXAMPLE/REQUEST_TYPE)"/>
            <xsl:text>&#xA;&#xA;</xsl:text>
          </xsl:when>
          <xsl:otherwise>
            <xsl:text>!!!Section REQUEST_TYPE needed from WPS generator!!! &#xA; </xsl:text>
            <xsl:text>&#xA;&#xA;</xsl:text>
          </xsl:otherwise>
        </xsl:choose>

        <xsl:text>*Example* </xsl:text>
        <xsl:text>&#xA;&#xA;</xsl:text>

        <!-- Extract section "Example Input" -->
        <xsl:text>_Input:_ </xsl:text>
        <xsl:text>&#xA;&#xA;</xsl:text>
        <xsl:choose>
          <xsl:when test="$xmlText/EXAMPLE/INPUT">
            <xsl:text>----</xsl:text>
            <xsl:text>&#xA;</xsl:text>
            <xsl:value-of select="$xmlText/EXAMPLE/INPUT"/>
            <xsl:text>&#xA;</xsl:text>
            <xsl:text>----</xsl:text>
            <xsl:text>&#xA;</xsl:text>
          </xsl:when>
          <xsl:otherwise>
            <xsl:text>!!!Section EXAMPLE/INPUT needed from WPS generator!!! &#xA; </xsl:text>
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
            <xsl:text>!!!Section EXAMPLE/OUTPUT needed from WPS generator!!! &#xA; </xsl:text>
            <xsl:text>&#xA;&#xA;</xsl:text>
          </xsl:otherwise>
        </xsl:choose>
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

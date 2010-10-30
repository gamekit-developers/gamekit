<?xml version="1.0"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="html"/>
  <xsl:template match="config">
    <html>
      <body>
        <xsl:apply-templates select="section"/>
      </body>
    </html>
  </xsl:template>

  <xsl:template match="section">
    <br/>
    <table border="1" width="800">
      <caption align="center" style="background-color=#4F559F;color=#DF9F9F;">
        <h1>
          <xsl:value-of select="@name"/>
        </h1>
      </caption>
      <xsl:apply-templates select="group"/>
    </table>
    <br/>
  </xsl:template>

  <xsl:template match="group">
    <xsl:variable name="total" select="count(entry)"/>
    
    <xsl:for-each select="entry"> 
      <tr>
        
        <xsl:if test="position()=1">       
          <td width="20%" align="center" style="background-color=#9F9FBF;color=#4F4F6F">
        
            <xsl:attribute name="rowspan">
              <xsl:value-of select="$total"/>
            </xsl:attribute>
        
            <xsl:value-of select="../@name"/>
          </td>
        </xsl:if>
                
        <td width="40%">
          <xsl:value-of select="@name"/>
        </td>
        
        <td>
          <xsl:value-of select="@value"/>
          <xsl:text>&#160;</xsl:text>
        </td>
        
      </tr>
    </xsl:for-each>

  </xsl:template>
  
</xsl:stylesheet>

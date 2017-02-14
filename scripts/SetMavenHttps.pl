#!/usr/bin/perl -w

# This script will add an alternate download mirror for https to the maven settings file if its not alredy there
use strict;
use warnings; 
use XML::LibXML;
use XML::LibXML::XPathContext;

# parse file
my $parser = XML::LibXML->new;
$parser->keep_blanks(0);
my $doc = $parser->parse_file('/usr/share/maven/conf/settings.xml');

# register namespace
my $xc = XML::LibXML::XPathContext->new($doc);
$xc->registerNs('x', 'http://maven.apache.org/SETTINGS/1.0.0');

# look for exitsing entry
my ($repoNode) = $xc->findnodes("//x:mirror/
       x:id[text( )='central-secure']/../
       x:url[text( )='https://repo.maven.apache.org/maven2']/../
       x:mirrorOf[text( )='central']/
       ancestor::x:mirror");

if (not defined($repoNode)) {
  # no existing node so create one and add it

  # find first instance of 'mirrors' element
  my ($node) = $xc->findnodes("//x:mirrors");
  
  # create new sub 'mirror' elements
  my $id_element = $doc->createElement('id');
  $id_element->appendText('central-secure');
  my $url_element = $doc->createElement('url');
  $url_element->appendText('https://repo.maven.apache.org/maven2');
  my $of_element = $doc->createElement('mirrorOf');
  $of_element->appendText('central');

  # create new 'mirror' element and add sub elements
  my $mirror_element = $doc->createElement('mirror');
  $mirror_element->appendChild($id_element);
  $mirror_element->appendChild($url_element);
  $mirror_element->appendChild($of_element);

  # add 'mirror' element and write to file
  $node->appendChild($mirror_element);
  $doc->toFile('/usr/share/maven/conf/settings.xml', 2);
}

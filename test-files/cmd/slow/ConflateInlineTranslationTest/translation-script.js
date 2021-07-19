// IMPORT
// translateToOsm - takes 'attrs' and returns OSM 'tags'
function translateToOsm(attrs, layerName, geometryType)
{
  var tags = {};
  if (attrs.f_code == 'AP030') tags.highway='road';
  if (attrs.src) tags.source = attrs.src;
  switch (attrs.cls)
  {
    case undefined:
    break;
    case 'road':
      tags.surface = 'paved';
      break;
    case 'dirt_road':
      tags.surface = 'unpaved';
      break;
  }
  return tags;
}


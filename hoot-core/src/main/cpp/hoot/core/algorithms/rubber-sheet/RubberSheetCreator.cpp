#include "RubberSheetCreator.h"

// Hoot
#include <hoot/core/algorithms/rubber-sheet/RubberSheet.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/util/FileUtils.h>

namespace pt = boost::property_tree;

namespace hoot
{

void RubberSheetCreator::create(const QString& tiepoints, const QString& input, const QString& input2, const QString& transform2To1,
                                const QString& transform1To2, const bool ref) const
{
  LOG_STATUS("Creating alignment transform for  ..." << FileUtils::toLogFormat(tiepoints, 25) << "...");

  OsmMapPtr map = std::make_shared<OsmMap>();
  IoUtils::loadMap(map, input, false, Status::Unknown1);
  IoUtils::loadMap(map, input2, false, Status::Unknown2);

  std::shared_ptr<OGRSpatialReference> prepojection = map->getProjection();

  MapProjector::projectToPlanar(map);

  std::shared_ptr<OGRSpatialReference> projection = map->getProjection();

  std::vector<RubberSheet::Tie> ties;

  //  Load the tie-points file and convert it to a tie-point vector
  boost::property_tree::ptree json;
  std::ifstream infile(tiepoints.toStdString());
  if (!infile.is_open())
    throw HootException("Unable to read JSON file: " + tiepoints);

  std::vector<geos::geom::Coordinate> coords;

  try
  {
    pt::read_json(infile, json);
    //  Parse the JSON tie-point tree
    for (const auto& points_array : json.get_child("tie-points"))
    {
//      geos::geom::Coordinate coords[2];
//      int coord_idx = 0;
      for (const auto& coords_array : points_array.second)
      {
        //  Latitude (0), Longitude (1)
        double lat_lon[2];
        int index = 0;
        for (const auto& coord : coords_array.second)
        {
          lat_lon[index] = coord.second.get_value<double>();
          index++;
        }
        coords.emplace_back(lat_lon[1], lat_lon[0]);
        //  Convert the array to a coordinate
//        coords[coord_idx].x = lat_lon[1];
//        coords[coord_idx].y = lat_lon[0];
//        coord_idx++;
      }
//      ties.emplace_back(coords[0], coords[1]);
    }
    //  Reproject the tie-points to planar
    std::vector<geos::geom::Coordinate> post = MapProjector::project(coords, prepojection, projection);
    //  Create the tie-point vector for the rubber sheet
    for (size_t i = 0; i < post.size(); i += 2)
      ties.emplace_back(post[i], post[i + 1]);
  }
  catch (const pt::json_parser::json_parser_error& e)
  {
    QString reason = QString::fromStdString(e.message());
    QString line = QString::number(e.line());

    throw HootException(QString("Error parsing JSON: %1 (line %2)").arg(reason, line));
  }
  catch (const std::exception& e)
  {
    QString reason = e.what();
    throw HootException("Error parsing JSON " + reason);
  }

  RubberSheet rs;
  rs.setReference(ref);
  rs.setProjection(projection);
  rs.createTransform(ties);

  QFile fp2to1(transform2To1);
  if (fp2to1.open(QIODevice::WriteOnly) == false)
    throw HootException(QString("Error opening %1 for writing").arg(transform2To1));
  rs.writeTransform2to1(fp2to1);
  // if --ref is specified then this will just write an identity transform
  if (transform1To2 != "")
  {
    QFile fp1to2(transform1To2);
    if (fp1to2.open(QIODevice::WriteOnly) == false)
      throw HootException(QString("Error opening %1 for writing").arg(transform1To2));
    rs.writeTransform1to2(fp1to2);
  }
}

}

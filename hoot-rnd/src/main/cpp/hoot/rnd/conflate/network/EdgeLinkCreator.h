#ifndef EDGELINKCREATOR_H
#define EDGELINKCREATOR_H

namespace hoot
{

/**
 * This class links two pairs of edges to show that there is a route between them. Directionality
 * isn't taken into consideration. This concept is a bit more precise than simply saying two
 * vertices match. This class discovers all the links and puts them into a link data structure.
 *
 * To make the terminology a little more precise, the term link will refer to a link from one
 * pair of edges to another pair of edges.
 */
class EdgeLinkCreator
{
public:
  EdgeLinkCreator();
};

}

#endif // EDGELINKCREATOR_H

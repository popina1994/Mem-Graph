#pragma once

#include "Label.h"
#include "Vertex.h"
#include "Edge.h"

class GraphStorage
{
public: 
	void CreateVertex();
	void CreateEdge(const Vertex& v1, const Vertex& v2);
	void AddLabel(const Vertex& a, const Label& l);
	void RemoveLabel(const Vertex& a, const Label& l);
	void ShortestPath(const Vertex& vSrc, const Vertex& vDst, const Label& lab) const;
};


#pragma once
#include "Vertex.h"

namespace MemGraph
{
	class Edge
	{
		Vertex::VERTEX_ID m_vId;

	public:
		Edge(const Vertex::VERTEX_ID& vId): m_vId(vId){}
		const Vertex::VERTEX_ID& getVertexId(void) const
		{
			return m_vId;
		}
		// Weight
	};
}

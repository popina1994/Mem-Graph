#pragma once

#include "Type.h"

namespace MemGraph
{
	/**
	* @brief Stores the end to which the edge is pointed to.
	*/
	class Edge
	{
		Type::VERTEX_ID m_vId;

	public:
		Edge(const Type::VERTEX_ID& vId): m_vId(vId){}
		
		/**
		* @brief Return the vertex id.
		*/
		const Type::VERTEX_ID& GetVertexId(void)
		{
			return m_vId;
		}
	};
}
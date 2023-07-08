#pragma once

#include "Label.h"
#include "Vertex.h"
#include "Edge.h"
#include "NoVertexException.h"

namespace MemGraph
{

	class GraphStorage
	{
		std::vector<std::shared_ptr<Vertex>> m_vVertices;
	public:
		enum class ShortPathSearches
		{
			DFS, BFS
		};
		/**
		* Creates a vertex in Graph Store where identifier is returned.
		*/
		Vertex::VERTEX_ID CreateVertex()
		{
			std::shared_ptr<Vertex> pVert = std::make_shared<Vertex>();
			m_vVertices.push_back(pVert);
			return m_vVertices.size() - 1;
		}

		void CreateEdge(const Vertex::VERTEX_ID& v1Id, const Vertex::VERTEX_ID& v2Id)
		{
			if (v1Id >= m_vVertices.size())
			{
				throw NoVertexException(v1Id);
			}
			if (v2Id >= m_vVertices.size())
			{
				throw NoVertexException(v2Id);
			}
			m_vVertices[v1Id]->AddEdge(v2Id);
		}

		void AddLabel(const Vertex::VERTEX_ID& vId, const Label& label)
		{
			if (vId >= m_vVertices.size())
			{
				throw NoVertexException(vId);
			}
			m_vVertices[vId]->AddLabel(label);
		}
		void RemoveLabel(const Vertex::VERTEX_ID& vId, const Label& label)
		{
			if (vId >= m_vVertices.size())
			{
				throw NoVertexException(vId);
			}
			m_vVertices[vId]->RemoveLabel(label);
		}

		bool ExistsVertex(const Vertex::VERTEX_ID& vId) const
		{
			return vId < m_vVertices.size();
		}

		bool VertexHasLabel(const Vertex::VERTEX_ID& vId, const Label& label) const
		{
			if (!ExistsVertex(vId))
			{
				return false;
			}
			return m_vVertices[vId]->HasLabel(label);
		}

		bool ExistsEdge(const Vertex::VERTEX_ID& vId1, const Vertex::VERTEX_ID& vId2) const
		{
			if (!ExistsVertex(vId1) || !ExistsVertex(vId2))
			{
				return false;
			}
			return m_vVertices[vId1]->HasEdge(vId2);

		}

		std::vector<Vertex::VERTEX_ID> ShortestPath(const Vertex::VERTEX_ID& vId1, const Vertex::VERTEX_ID& vId2, const Label& lab, const ShortPathSearches& sps) const
		{
			if (sps == ShortPathSearches::BFS)
			{
				// TODO: BFS
			}
			else if (sps == ShortPathSearches::DFS)
			{
				// TODO: DFS
			}
		}

		friend std::ostream& operator<<(std::ostream& out, const GraphStorage& gs)
		{
		
			//for (const auto& gs.)
		}
	};


}

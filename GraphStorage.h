#pragma once

#include "Label.h"
#include "Vertex.h"
#include "Edge.h"
#include "NoVertexException.h"
#include <list>
#include <iostream>

namespace MemGraph
{
	/***
	* @brief Class that stores graphs. 
	*/
	class GraphStorage
	{
		std::vector<std::unique_ptr<Vertex>> m_vVertices;
	public:
		enum class ShortPathSearches
		{
			BFS
		};

		/**
		* @brief Returns true if the vertex @par vId exists. 
		*/
		bool ExistsVertex(const Vertex::VERTEX_ID& vId) const
		{
			return vId < m_vVertices.size();
		}

		/**
		* @brief Returns true if the vertex @par vId has the label @par label, otherwise false.
		*/
		bool VertexHasLabel(const Vertex::VERTEX_ID& vId, const Label& label) const
		{
			if (!ExistsVertex(vId))
			{
				return false;
			}
			return m_vVertices[vId]->HasLabel(label);
		}

		/**
		* @brief Returns true if there is an edge with (@par vId1, @par vId2).
		*/
		bool ExistsEdge(const Vertex::VERTEX_ID& vId1, const Vertex::VERTEX_ID& vId2) const
		{
			if (!ExistsVertex(vId1) || !ExistsVertex(vId2))
			{
				return false;
			}
			return m_vVertices[vId1]->HasEdge(vId2);
		}

		/**
		* @brief Creates a vertex in Graph Store where the vertex id is returned.
		* @note Potential exception: std::bad_alloc. 
		*/
		Vertex::VERTEX_ID CreateVertex()
		{
			std::unique_ptr<Vertex> pVert = std::make_unique<Vertex>();
			m_vVertices.emplace_back(std::move(pVert));
			return m_vVertices.size() - 1;
		}

		/**
		* @brief Creates an edge in Graph Store.
		* @note Potential exception: std::bad_alloc and MemGraph::NoVertexException.  
		*/
		void CreateEdge(const Vertex::VERTEX_ID& v1Id, const Vertex::VERTEX_ID& v2Id)
		{
			if (!ExistsVertex(v1Id))
			{
				throw NoVertexException(v1Id);
			}
			if (!ExistsVertex(v2Id))
			{
				throw NoVertexException(v2Id);
			}
			m_vVertices[v1Id]->AddToEdge(v2Id);
			m_vVertices[v2Id]->AddFromEdge(v1Id);
		}

		/**
		* @brief Adds the label @par label to the existing vertex in the graph store.
		* @note Potential exception: std::bad_alloc and MemGraph::NoVertexException. 
		*/
		void AddLabel(const Vertex::VERTEX_ID& vId, const Label& label)
		{
			if (!ExistsVertex(vId))
			{
				throw NoVertexException(vId);
			}
			m_vVertices[vId]->AddLabel(label);
		}

		/**
		* @brief Removes the label @par label from the existing vertex in the graph store.
		* @note Potential exception: std::bad_alloc, MemGraph::NoVertexException and MemGraph::NoLabelException.
		*/
		void RemoveLabel(const Vertex::VERTEX_ID& vId, const Label& label)
		{
			if (!ExistsVertex(vId))
			{
				throw NoVertexException(vId);
			}
			m_vVertices[vId]->RemoveLabel(label);
		}


		/**
		* @brief Returns the shortest path between two vertices @par vertIdStart and @par vertIdEnd such that vertices on the way contain @par lab. 
		* @note Potential exception: std::bad_alloc, MemGraph::NoVertexException.
		*/
		std::vector<Vertex::VERTEX_ID> ShortestPath(const Vertex::VERTEX_ID& vertIdStrt, const Vertex::VERTEX_ID& vertIdEnd, 
			const Label& label, 
			const ShortPathSearches& sps = ShortPathSearches::BFS) const
		{
			std::vector<uint32_t> vVisitStep(m_vVertices.size(), -1);
			std::vector<Vertex::VERTEX_ID> vPath;
			uint32_t pathLength = 0;
			if (!ExistsVertex(vertIdStrt))
			{
				throw NoVertexException(vertIdStrt);
			}
			if (!ExistsVertex(vertIdEnd))
			{
				throw NoVertexException(vertIdEnd);
			}

			if (sps == ShortPathSearches::BFS)
			{
				// Apply BFS to index the shortest distance of each node from the starting node. 
				std::list<Vertex::VERTEX_ID> lVisit;
				lVisit.push_back(vertIdStrt);
				vVisitStep[vertIdStrt] = 0;

				while (!lVisit.empty())
				{
					Vertex::VERTEX_ID curVertId = lVisit.front();
					lVisit.pop_front();

					if (curVertId == vertIdEnd)
					{
						pathLength  = vVisitStep[vertIdEnd];
						break;
					}

					uint32_t curStep = vVisitStep[curVertId];
					const auto& curVertAdjEdgs = m_vVertices[curVertId]->GetToAdjVertices();

					for (const Vertex::VERTEX_ID& nextVertId:  curVertAdjEdgs)
					{
						// Visited
						if (!m_vVertices[nextVertId]->HasLabel(label))
						{
							continue;
						}
						if (vVisitStep[nextVertId] != -1)
						{
							continue;
						}
						lVisit.push_back(nextVertId);
						vVisitStep[nextVertId] = curStep + 1; 
					}
				}
				// Trace back the path.
			}
			vPath.resize(pathLength + 1);
			Vertex::VERTEX_ID curVertId = vertIdEnd;
			uint32_t curStep = vVisitStep[curVertId];
			for (int cnt = 0; cnt < pathLength; cnt++)
			{
				vPath[cnt] = curVertId;
				for (const auto& vertBefId : m_vVertices[curVertId]->GetFromAdjVertices())
				{
					if (vVisitStep[vertBefId] == curStep - 1)
					{
						curVertId = vertBefId;
						curStep--;
						break;
					}
				}
			}
			vPath.back() = vertIdStrt;
			std::reverse(vPath.begin(), vPath.end());

			return vPath;
		}

		friend std::ostream& operator<<(std::ostream& out, const GraphStorage& gs)
		{
		
			//for (const auto& gs.)
		}
	};


}

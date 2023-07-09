#pragma once

#include "Label.h"
#include "Vertex.h"
#include "Edge.h"
#include "NoVertexException.h"
#include <list>
#include <queue>
#include <iostream>
#include <chrono>

namespace MemGraph
{
	/**
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
		bool ExistsVertex(const Type::VERTEX_ID& vId) const
		{
			return vId < m_vVertices.size();
		}

		/**
		* @brief Returns true if the vertex @par vId has the label @par label, otherwise false.
		*/
		bool VertexHasLabel(const Type::VERTEX_ID& vId, const Label& label) const
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
		bool ExistsEdge(const Type::VERTEX_ID& vId1, const Type::VERTEX_ID& vId2) const
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
		Type::VERTEX_ID CreateVertex()
		{
			std::unique_ptr<Vertex> pVert = std::make_unique<Vertex>();
			m_vVertices.emplace_back(std::move(pVert));

			return m_vVertices.size() - 1;
		}

		/**
		* @brief Creates an edge in Graph Store.
		* @note Potential exception: std::bad_alloc and MemGraph::NoVertexException.  
		*/
		void CreateEdge(const Type::VERTEX_ID& v1Id, const Type::VERTEX_ID& v2Id)
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
		}

		/**
		* @brief Adds the label @par label to the existing vertex in the graph store.
		* @note Potential exception: std::bad_alloc and MemGraph::NoVertexException. 
		*/
		void AddLabel(const Type::VERTEX_ID& vId, const Label& label)
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
		void RemoveLabel(const Type::VERTEX_ID& vId, const Label& label)
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
		std::vector<Type::VERTEX_ID> ShortestPath(
			const Type::VERTEX_ID& vertIdStrt, 
			const Type::VERTEX_ID& vertIdEnd, 
			const Label& label, 
			const ShortPathSearches& shortestPathSearch = ShortPathSearches::BFS) const
		{
			std::vector<uint32_t> vVisitStep(m_vVertices.size(), 0);
			std::vector<Type::VERTEX_ID> vPath;
			std::vector<Type::VERTEX_ID> qVisit(m_vVertices.size(), -1);
			std::vector<Type::VERTEX_ID> vOrigVertex(m_vVertices.size(), -1);
			uint32_t curIdx = 0;
			uint32_t pathLength = 0;
			bool thereIsPath = false;
			
			if (!ExistsVertex(vertIdStrt))
			{
				throw NoVertexException(vertIdStrt);
			}
			if (!ExistsVertex(vertIdEnd))
			{
				throw NoVertexException(vertIdEnd);
			}

			if (shortestPathSearch == ShortPathSearches::BFS)
			{
				// Apply BFS to index the shortest distance of each node from the starting node. 
				std::queue<Type::VERTEX_ID> qVisit;
				qVisit.push(vertIdStrt);

				while (!qVisit.empty())
				{
					Type::VERTEX_ID curVertId = qVisit.front();
					qVisit.pop();
					uint32_t curStep = vVisitStep[curVertId];

					if ((curVertId == vertIdEnd) && (curStep != 0))
					{
						pathLength = vVisitStep[vertIdEnd];
						thereIsPath = true;
						break;
					}
					const auto& curVertAdjEdgs = m_vVertices[curVertId]->GetToAdjVertices();

					for (const auto& pEdge : curVertAdjEdgs)
					{
						auto nextVertId = pEdge->GetVertexId();
						if (m_vVertices[nextVertId]->HasLabel(label) && (vVisitStep[nextVertId] == 0))
						{
							qVisit.push(nextVertId);
							vVisitStep[nextVertId] = curStep + 1;
							vOrigVertex[nextVertId] = curVertId;
						}
					}
				}
				// Trace back the path.

				if (!thereIsPath)
				{
					return {};
				}

				vPath.resize(pathLength + 1);
				Type::VERTEX_ID curVertId = vertIdEnd;
				uint32_t curStep = vVisitStep[curVertId];
				for (int cnt = 0; cnt < pathLength; cnt++)
				{
					vPath[pathLength - cnt] = curVertId;
					curVertId = vOrigVertex[curVertId];
				}
				vPath[0] = vertIdStrt;
			}

			return vPath;
		}
	};


}

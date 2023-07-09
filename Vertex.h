#pragma once

#include <vector>
#include <string> 
#include <unordered_set>
#include <memory>
#include "NoLabelException.h"
#include "Label.h"
#include "Type.h"
#include "Edge.h"

namespace MemGraph
{
	/**
	* @brief Class that stores information about vertices in the graph storage: labels and the adjcent vertices.
	* Depending on the neeeds, additional data structures can be added to speed up the search. 
	*/
	class Vertex
	{
	private:
		std::unordered_set<Label> m_sLabels;
		std::vector<std::unique_ptr<Edge>> m_vToAdjVertices;
	public:
		
		/**
		* @brief Addds label @par label to the vertex. 
		* @note It can throw bad_alloc in the case there is not enough memory. 
		*/
		void AddLabel(const Label& label)
		{
			m_sLabels.insert(label);
		}

		/**
		* @brief Removes label @par label from the vertex.
		* @note It can throw NoLabelException the label does not exist.
		*/
		void RemoveLabel(const Label& label)
		{
			if (m_sLabels.find(label) == m_sLabels.end())
			{
				throw NoLabelException(label);
			}
			m_sLabels.erase(label);
		}

		/**
		* @brief Adds a directed vertex from the current vertex to the vertex with vId.
		* @note It can throw bad_alloc in the case there is not enough memory.
		*/
		void AddToEdge(const Type::VERTEX_ID& vId)
		{
			std::unique_ptr<Edge> pEdge = std::make_unique<Edge>(vId);
			m_vToAdjVertices.push_back(std::move(pEdge));
		}


		/**
		* @brief Returns if the label @par label exists.
		*/
		bool HasLabel(const Label& label) const
		{
			return m_sLabels.find(label) != m_sLabels.end();
		}

		/**
		* @brief Returns if the directed edge from the current vertex to @par vId exists.
		*/
		bool HasEdge(const Type::VERTEX_ID& vId) const
		{
			for (const auto& pEdge: m_vToAdjVertices)
			{
				if (pEdge->GetVertexId() == vId)
				{
					return true;
				}
			}
		}

			
		/**
		* @brief Gets all edges for which this vertex is pointing from.
		*/
		const std::vector<std::unique_ptr<Edge>>& GetToAdjVertices(void) const
		{
			return m_vToAdjVertices;
		}
		
	};
}

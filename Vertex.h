#pragma once

#include <vector>
#include <string> 
#include <unordered_set>
#include <memory>
#include "NoLabelException.h"
#include "Label.h"

namespace MemGraph
{
	class Vertex
	{
	public:
		typedef uint32_t VERTEX_ID;
	private:
		std::unordered_set<Label> m_sLabels;
		std::vector<VERTEX_ID> m_vToAdjVertices;
	public:

		void AddLabel(const Label& label)
		{
			m_sLabels.insert(label);
		}

		void RemoveLabel(const Label& label)
		{
			if (m_sLabels.find(label) == m_sLabels.end())
			{
				throw NoLabelException(label);
			}
			m_sLabels.erase(label);
		}

		void AddToEdge(const VERTEX_ID& vId)
		{
			//m_vToAdjVertices.insert(vId);
			m_vToAdjVertices.push_back(vId);
		}

		bool HasLabel(const Label& label) const
		{
			return m_sLabels.find(label) != m_sLabels.end();
		}

		bool HasEdge(const VERTEX_ID& vId) const
		{
			return std::find(m_vToAdjVertices.begin(), m_vToAdjVertices.end(), vId) != m_vToAdjVertices.end();
		}



		const std::vector<VERTEX_ID>& GetToAdjVertices(void) const
		{
			return m_vToAdjVertices;
		}
		
	};
}

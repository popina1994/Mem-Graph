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
		std::unordered_set<VERTEX_ID> m_vAdjVertices;
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

		void AddEdge(const VERTEX_ID& vId)
		{
			m_vAdjVertices.insert(vId);
		}

		bool HasLabel(const Label& label) const
		{
			return m_sLabels.find(label) != m_sLabels.end();
		}

		bool HasEdge(const VERTEX_ID& vId) const
		{
			return m_vAdjVertices.find(vId) != m_vAdjVertices.end();
		}

		const std::unordered_set<VERTEX_ID>& GetAdjVertices(void) const
		{
			return m_vAdjVertices;
		}
		
	};
}

#pragma once

#include <exception>
#include "Vertex.h"
#include "Label.h"

namespace MemGraph
{
	class NoLabelException : public std::exception
	{
		std::string m_errorMessage;
	public:
		NoLabelException(const Label& label)
		{
			m_errorMessage = "Label with" + label.getLabelName() + "id does not exist";
		}
		virtual const char* what() const override
		{

			return m_errorMessage.c_str();
		}

		virtual ~NoLabelException()
		{
		}
	};
}
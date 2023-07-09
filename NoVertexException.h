#include <exception>
#include "Vertex.h"

namespace MemGraph
{
	class NoVertexException : public std::exception
	{

		std::string m_errorMessage;
	public:
		NoVertexException(const Type::VERTEX_ID& vertId)
		{
			m_errorMessage = "Vertex with" + std::to_string(vertId) + "id does not exist";
		}
		virtual const char* what() const override
		{

			return m_errorMessage.c_str();
		}

		virtual ~NoVertexException()
		{

		}
	};
}
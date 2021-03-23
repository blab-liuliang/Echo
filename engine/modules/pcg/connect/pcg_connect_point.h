#pragma once

#include "engine/modules/pcg/data/pcg_data.h"

namespace Echo
{
	class PCGNode;
	class PCGConnect;
	class PCGConnectPoint
	{
		friend class PCGNode;

	public:
		// Type
		enum Type
		{
			Input,
			Output
		};

	public:
		PCGConnectPoint(PCGNode* owner, const String& supportTypes);
		PCGConnectPoint(PCGNode* owner, PCGDataPtr data);
		~PCGConnectPoint();

		// Owner
		PCGNode* getOwner() { return m_owner; }

		// Data type
		String getDataType();

		// Data
		PCGDataPtr getData();
		void setData(PCGDataPtr InData) { m_data = InData; }

	public:
		// Connect
		void addConnect(PCGConnect* InConnect);
		void removeConnect(PCGConnect* InConnect);

		// Depend
		PCGConnectPoint* getDependEndPoint();

	protected:
		PCGNode*						m_owner = nullptr;
		Type							m_type;
		String							m_supportTypes;
		PCGDataPtr						m_data;
		std::vector<class PCGConnect*>	m_connects;
	};
}
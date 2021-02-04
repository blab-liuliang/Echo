#include "shader_node_degree_to_radian.h"
#include "shader_node_operation_rules.h"

#ifdef ECHO_EDITOR_MODE

namespace Echo
{
    ShaderNodeDegreeToRadian::ShaderNodeDegreeToRadian()
        : ShaderNode()
    {
        m_inputDataTypes = 
        {
            {"any", "degree"},
        };

        m_inputs.resize(m_inputDataTypes.size());

        m_outputs.resize(1);
        m_outputs[0] = std::make_shared<DataInvalid>(this);
        m_outputs[0]->setVariableName(getVariableName());
    }

	void ShaderNodeDegreeToRadian::bindMethods()
	{

	}

    void ShaderNodeDegreeToRadian::setInData(std::shared_ptr<NodeData> nodeData, QtNodes::PortIndex portIndex)
    {
        m_inputs[portIndex] = std::dynamic_pointer_cast<ShaderData>(nodeData);
        if (m_inputs[0])
        {
            m_outputs[0] = OperationRules::instance().NewShaderData( DataAny::getInternalData(m_inputs[0])->type().id, this);
            m_outputs[0]->setVariableName(getVariableName());
        }
        else
        {
			m_outputs[0] = std::make_shared<DataInvalid>(this);
			m_outputs[0]->setVariableName(getVariableName());
        }

		Q_EMIT dataUpdated(0);
    }

    bool ShaderNodeDegreeToRadian::generateCode(Echo::ShaderCompiler& compiler)
    {
        if (m_inputs[0])
        {
            compiler.addCode(Echo::StringUtil::Format("\t%s %s = radians(%s);\n",
                m_outputs[0]->type().id.c_str(),
                m_outputs[0]->getVariableName().c_str(),
                DataAny::getInternalData(m_inputs[0])->getVariableName().c_str()));
        }

        return true;
    }
}

#endif
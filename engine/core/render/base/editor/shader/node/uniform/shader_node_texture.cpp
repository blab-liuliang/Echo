#include "shader_node_texture.h"
#include "engine/core/util/StringUtil.h"
#include "engine/core/render/base/editor/shader/data/shader_data_texture.h"

namespace Echo
{
#ifdef ECHO_EDITOR_MODE
	ShaderNodeTexture::ShaderNodeTexture()
		: ShaderNode()
	{
		m_toolButton = new QToolButton();
		m_toolButton->setFixedSize(75, 75);

		m_inputDataTypes =
		{
			{"vec2", "uv"},
		};

		m_inputs.resize(m_inputDataTypes.size());

		m_outputs.resize(3);
		m_outputs[0] = std::make_shared<DataSampler2D>(this, "tex");
		m_outputs[1] = std::make_shared<DataVector3>(this, "rgb");
		m_outputs[2] = std::make_shared<DataFloat>(this, "a");

		updateOutputDataVariableName();
	}

	ShaderNodeTexture::~ShaderNodeTexture()
	{
	}

	void ShaderNodeTexture::bindMethods()
	{
		CLASS_BIND_METHOD(ShaderNodeTexture, setVariableName, DEF_METHOD("setVariableName"));
		CLASS_BIND_METHOD(ShaderNodeTexture, getVariableName, DEF_METHOD("getVariableName"));
		CLASS_BIND_METHOD(ShaderNodeTexture, setType, DEF_METHOD("setType"));
		CLASS_BIND_METHOD(ShaderNodeTexture, getType, DEF_METHOD("getType"));
		CLASS_BIND_METHOD(ShaderNodeTexture, setAtla, DEF_METHOD("setAtla"));
		CLASS_BIND_METHOD(ShaderNodeTexture, isAtla, DEF_METHOD("isAtla"));
		CLASS_BIND_METHOD(ShaderNodeTexture, setTexture, DEF_METHOD("setTexture"));
		CLASS_BIND_METHOD(ShaderNodeTexture, getTexture, DEF_METHOD("getTexture"));

		CLASS_REGISTER_PROPERTY(ShaderNodeTexture, "Name", Variant::Type::String, "getVariableName", "setVariableName");
		CLASS_REGISTER_PROPERTY(ShaderNodeTexture, "Type", Variant::Type::StringOption, "getType", "setType");
		CLASS_REGISTER_PROPERTY(ShaderNodeTexture, "Atla", Variant::Type::Bool, "isAtla", "setAtla");
		CLASS_REGISTER_PROPERTY(ShaderNodeTexture, "Texture", Variant::Type::Bool, "getTexture", "setTexture");
	}

	void ShaderNodeTexture::updateOutputDataVariableName()
	{
		Echo::String variableName = getDefaultVariableName();

		m_outputs[0]->setVariableName(variableName.c_str());
		m_outputs[1]->setVariableName(Echo::StringUtil::Format("%s_Color.rgb", variableName.c_str()));
		m_outputs[2]->setVariableName(Echo::StringUtil::Format("%s_Color.a", variableName.c_str()));
	}

	void ShaderNodeTexture::setInData(std::shared_ptr<NodeData> nodeData, int portIndex)
	{
		m_inputs[portIndex] = std::dynamic_pointer_cast<ShaderData>(nodeData);

		Q_EMIT dataUpdated(0);
		Q_EMIT dataUpdated(1);
		Q_EMIT dataUpdated(2);
	}

	bool ShaderNodeTexture::getDefaultValue(Echo::StringArray& uniformNames, Echo::VariantArray& uniformValues)
	{
		uniformNames.emplace_back("Uniforms." + getVariableName());
		uniformValues.emplace_back(Echo::ResourcePath(m_texture.getPath(), m_isAtla ? ".png|.atla" : ".png"));

		if (m_isAtla)
		{
			uniformNames.emplace_back("Uniforms." + getVariableName() + "Viewport");
			uniformValues.emplace_back(Echo::Color(0.f, 0.f, 1.f, 1.f));
		}

		return true;
	}

	bool ShaderNodeTexture::generateCode(ShaderCompiler& compiler)
	{
		updateOutputDataVariableName();

		compiler.addMacro("ENABLE_VERTEX_UV0");

		compiler.addTextureUniform(getVariableName());

		Echo::String uvConvertCode;
		if (isAtla())
		{
			compiler.addUniform("vec4", Echo::StringUtil::Format("%sViewport", getVariableName().c_str()));
			uvConvertCode = Echo::StringUtil::Format(" * fs_ubo.%sViewport.zw + fs_ubo.%sViewport.xy", getVariableName().c_str(), getVariableName().c_str());
		}

		if (isAtla())
		{
			compiler.addUniform("vec4", Echo::StringUtil::Format("%sViewport", getVariableName().c_str()));
			uvConvertCode = Echo::StringUtil::Format(" * fs_ubo.%sViewport.zw + fs_ubo.%sViewport.xy", getVariableName().c_str(), getVariableName().c_str());
		}

		if (m_inputs[0])
		{
			compiler.addCode(Echo::StringUtil::Format("\tvec4 %s_Color = texture( %s, %s %s);\n", getVariableName().c_str(), getVariableName().c_str(), dynamic_cast<ShaderData*>(m_inputs[0].get())->getVariableName().c_str(), uvConvertCode.c_str()));
		}
		else
		{
			compiler.addCode(Echo::StringUtil::Format("\tvec4 %s_Color = texture( %s, v_UV %s);\n", getVariableName().c_str(), getVariableName().c_str(), uvConvertCode.c_str()));
		}

		if (m_type.getValue() == "General")
		{
			compiler.addCode(Echo::StringUtil::Format("\t%s_Color.rgb = SRgbToLinear(%s_Color.rgb);\n", getVariableName().c_str(), getVariableName().c_str()));
		}
		else
		{
			compiler.addMacro("ENABLE_VERTEX_NORMAL");

			compiler.addCode(Echo::StringUtil::Format("\t%s_Color.rgb = _NormalMapFun(%s_Color.rgb);\n", getVariableName().c_str(), getVariableName().c_str()));
		}

		return true;
	}

#endif
}


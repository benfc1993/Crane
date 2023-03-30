#pragma once

#include "Crane/Core/Base.h"
#include "Crane/ImGui/ImGuiTheme.h"

namespace Crane {
	class ThemeSerialiser
	{
	public:
		ThemeSerialiser(ImGuiTheme& theme);

		void Serialise(const std::string filePath, const std::string name = "Default");

		bool Deserialise(const std::string filePath);

	private:
		ImGuiTheme& m_Theme;
	};
}
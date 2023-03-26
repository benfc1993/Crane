#pragma once 

#include "Crane/Core/Base.h"

namespace Crane {


	struct EditorSettingsData
	{
		bool ShowColiders = true;

		EditorSettingsData() = default;
	};


	class EditorSettings
	{
	public:
		EditorSettings();
		~EditorSettings();
		static void Serialise();
		static bool Deserialise();

		static EditorSettingsData* Get();

	};
}
#pragma once

#include <filewatch.h>

namespace Crane {

	enum FileType
	{
		None = 0,
		Scene = 1,
		Prefab = 2,
		Texture = 3
	};

	struct FileEvent
	{
		FileType Type;
		filewatch::Event Action;
		void* Data;
	};

	class FileSystem
	{
	public:
		FileSystem();
	private:
		void OnFileEvent(const std::string& path, const filewatch::Event change_type);
	private:
		filewatch::FileWatch<std::string> m_FileWatcher;
		std::vector<FileEvent> m_eventQueue;
	};
}
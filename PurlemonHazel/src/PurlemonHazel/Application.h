#pragma once
#include"Core.h"

namespace PurlemonHazel {

	class PH_API Application
	{
	public:
		Application();
		virtual ~Application(); // ����������������ͬʱ��������

		void Run();
	};

	// ���ڿͻ��˱�����
	Application* CreateApplication();
}


#include "LyraPCH.h"
#include "LyraEditor.h"
//程序入口
#include "core/entrypoint.h"
//_____________________________________

class  Lyra : public Aquarius::Application
{
public:
	Lyra()
		:Application("Lyra Editor")
	{
		PushOverLay(new Aquarius::LyraEditor());
		
	}

	~Lyra()
	{
	}

private:
};


Aquarius::Application* Aquarius::CreateApplication()
{
	return new Lyra();
}
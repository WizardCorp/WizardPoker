/**
	chat main file (entry point)
**/

// WizardPoker headers
#include "chat/Terminal/TerminalChat.hpp"
#include "chat/Gui/GuiChat.hpp"
// std-C++ headers
#include <cassert>
#include <iostream>
#include <memory>

#ifdef __linux__
extern "C"
{
# include <X11/Xlib.h>
}
#else
#endif

int main(int argc, char **argv)
{
#ifdef __linux__
	XInitThreads();
#else
#endif
	assert(argc == 7);
	std::unique_ptr<AbstractChat> chatManager;
	if(std::string(argv[6]) == "gui")
		chatManager.reset(new GuiChat(argv));
	else
		chatManager.reset(new TerminalChat(argv));
	chatManager->start();

	return EXIT_SUCCESS;
}

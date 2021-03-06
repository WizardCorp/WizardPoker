/// Client main file (entry point).

// std-C++ headers
#include <iostream>
// WizardPoker headers
#include "client/Terminal/TerminalApplication.hpp"
#include "client/Gui/GuiApplication.hpp"
#ifdef __linux__
extern "C"
{
# include <X11/Xlib.h>
}
#else
#endif

/// Print the usage of this program in the standard format.
static void printUsage(const std::string& programName, std::ostream& os = std::cout)
{
	os << "Usage: " << programName << " [-h] [--no-gui]" << std::endl;
}

/// Print the help when --help or -h is given as command line option.
static void printHelp(const std::string& programName)
{
	printUsage(programName);
	std::cout << "\n\
Fantasy multiplayer cards game\n\
\n\
Optional arguments:\n\
	-h, --help  show this help message and exit\n\
	--no-gui    use the terminal version rather than the GUI\n\
";
}

static int error(const std::string& programName, const std::string& message)
{
	std::cerr << programName << ": error: " << message << std::endl;
	printUsage(programName, std::cerr);
	return 1;
}

int main(int argc, char* argv[])
{
#ifdef __linux__
	XInitThreads();
#else
#endif
	int returnValue{0};
	if(argc > 2)
		returnValue = error(argv[0], "too many arguments");
	else if(argc == 2)
	{
		const std::string arg{argv[1]};
		if(arg == "--no-gui")
			returnValue = TerminalApplication().play();
		else if(arg == "-h" or arg == "--help")
			printHelp(argv[0]);
		else
			returnValue = error(argv[0], "unrecognized argument: " + arg);
	}
	else
		returnValue = GuiApplication().play();
	return returnValue;
}

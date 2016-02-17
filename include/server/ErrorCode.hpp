#ifndef _ERROR_CODE_SERVER_HPP_
#define _ERROR_CODE_SERVER_HPP_

// std-C++ headers
#include <cstdlib>
// WizardPoker headers
#include <common/ErrorCode.hpp>

/// enum used to define the return status of the server program
enum
{
	/// return code in case of non-free listening port
	UNABLE_TO_LISTEN,
};

#endif // _ERROR_CODE_SERVER_HPP_

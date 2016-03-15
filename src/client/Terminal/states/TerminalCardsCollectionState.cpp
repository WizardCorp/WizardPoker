// std-C++ headers
#include <iostream>
// WizardPoker headers
#include "client/states/TerminalCardsCollectionState.hpp"

TerminalCardsCollectionState::TerminalCardsCollectionState(StateStack& stateStack, Client& client):
	AbstractState(stateStack, client)
{
	addAction("Back to main menu", &TerminalCardsCollectionState::backMainMenu);
	try
	{
		_cardsCollection = _client.getCardsCollection();
	}
	catch(const std::runtime_error& e)
	{
		std::cout << "Error: " << e.what() << "\n";
		std::cout << "Default card collection loaded.\n";
	}
}

void TerminalCardsCollectionState::display()
{
	std::cout << "Here are your cards:\n";
	for(const auto& card : _cardsCollection)
		// The title of the card should be displayed rather than its ID
		std::cout << "* " << card << "\n";
	std::cout << std::string(40, '*') << "\n";

	// Display the actions
	AbstractState::display();
}

void TerminalCardsCollectionState::backMainMenu()
{
	stackPop();
}

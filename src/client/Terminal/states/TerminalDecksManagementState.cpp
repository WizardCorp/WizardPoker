// std-C++ headers
#include <iostream>
#include <cassert>
// WizardPoker headers
#include "common/CardData.hpp"
#include "client/sockets/Client.hpp"
#include "client/Terminal/states/TerminalDecksManagementState.hpp"

TerminalDecksManagementState::TerminalDecksManagementState(Context& context):
	AbstractState(context),
	TerminalAbstractState(context),
	AbstractDecksManagementState(context)
{
	addAction("Back to main menu", &TerminalDecksManagementState::backMainMenu);
	addAction("Display a deck", &TerminalDecksManagementState::displayDeck);
	addAction("Edit a deck", &TerminalDecksManagementState::editDeck);
	addAction("Create a deck", &TerminalDecksManagementState::createDeck);
	addAction("Delete a deck", &TerminalDecksManagementState::deleteDeck);
}

void TerminalDecksManagementState::display()
{
	displaySeparator("Decks");

	int i{1};
	for(const auto& deck : _decks)
		displayNumberedEntry(deck.getName(), i++);

	// Display the actions
	TerminalAbstractState::display();
}

void TerminalDecksManagementState::displayDeck()
{
	if(_decks.empty())
	{
		std::cout << "There is no deck to display!\n";
		return;
	}
	//get index of deck to display
	std::cout << "Choose a deck to display: ";
	const std::size_t index{askForNumber(1, _decks.size()+1) - 1};

	displaySeparator(_decks[index].getName(), '~'); // display the deck's name
	std::size_t i=0;
	for(const auto& cardId : _decks[index])  // display the deck's content
		displayCardWithIndex(cardId, i++);

	waitForEnter();
}

void TerminalDecksManagementState::editDeck()
{
	if(_decks.empty())
	{
		std::cout << "There is no deck to edit!\n";
		return;
	}
	std::cout << "Choose a deck to edit: ";
	std::size_t deckIndex;
	try
	{
		//ask for a deck index
		deckIndex = askForNumber(1, _decks.size()+1) - 1;
	}
	catch(const std::logic_error& e)
	{
		std::cout << "Wrong deck index!\n";
		return;
	}
	// N + 1/2 loop (stop as soon as the user enters 0).
	// askForReplacedCard calls must be in a try block, so it must be
	// inside the loop (we cannot write `statement; while(condition) statement;`).
	while(true)
	{
		try
		{
			//ask for the index of the card to replace
			std::size_t replacedIndex = askForReplacedCard(deckIndex);
			if(replacedIndex == 0)
				break;
			// Indices are displayed 1 greater than their actual values
			// (for the card 0, the index 1 is shown to the user)
			--replacedIndex;

			// Ask for the replacing card put it in the deck
			_decks[deckIndex].changeCard(replacedIndex, askForReplacingCard(deckIndex));
		}
		catch(const std::out_of_range& e)
		{
			std::cout << "Wrong card index!\n";
		}
		catch(const std::logic_error& e)
		{
			std::cout << "Error: " << e.what() << "\n";
		}
	};
	try
	{
		_context.client->handleDeckEditing(_decks[deckIndex]);
	}
	catch(const std::runtime_error& e)
	{
		std::cout << "Error: " << e.what() << "\n";
	}
}
std::size_t TerminalDecksManagementState::askForReplacedCard(std::size_t deckIndex)
{
	// display the deck's name and content
	displaySeparator(_decks[deckIndex].getName(), '~');
	std::size_t index=1;
	for(const auto& cardId : _decks[deckIndex])
		displayCardWithIndex(cardId, index++);

	// ask for a card index
	std::cout << "Choose a card to be replaced (0 to quit): ";
	return askForNumber(0, Deck::size + 1);
}

CardId TerminalDecksManagementState::askForReplacingCard(std::size_t deckIndex)
{
	displaySeparator("Available cards for replacement", '~');
	auto cardsCollectionToShow(getCardsCollectionWithoutDeck(_decks[deckIndex]));

	// display cards with their index
	std::size_t i=1;
	for(const auto& cardId : cardsCollectionToShow)
		displayCardWithIndex(cardId, i++);

	// ask for a card index
	std::cout << "Choose a card to put in your deck: ";
	std::size_t chosenIndex;
	try
	{
		//ask for an index within the card collection
		chosenIndex = askForNumber(1, cardsCollectionToShow.size()+1) -1;
	}
	catch(const std::logic_error& e)
	{
		throw std::logic_error("Not a valid index!");
	}

	CardId replacingCard = cardsCollectionToShow.at(chosenIndex);

	// Check if user HAS this card
	if(not _cardsCollection.contains(replacingCard))
		throw std::logic_error("You do not have this card!");
	// Check if deck does not contain this card twice
	if(std::count(_decks.at(deckIndex).begin(), _decks.at(deckIndex).end(), replacingCard) > 1)
		throw std::logic_error(std::to_string(replacingCard) + " is already two times in your deck, this is the maximum allowed.");

	return replacingCard;
}

void TerminalDecksManagementState::createDeck()
{
	std::cout << "Choose a name for your new deck: ";
	std::string input;
	std::getline(std::cin, input);
	_decks.emplace_back(input);
	try
	{
		_context.client->handleDeckCreation(_decks.back());
	}
	catch(const std::runtime_error& e)
	{
		std::cout << "Error: " << e.what() << "\n";
	}
}

void TerminalDecksManagementState::deleteDeck()
{
	if(_decks.empty())
	{
		std::cout << "There is no deck to delete!\n";
		return;
	}
	try
	{
		std::cout << "Choose a deck to delete: ";
		const std::size_t input{askForNumber(1, _decks.size() + 1) - 1};
		_context.client->handleDeckDeletion(_decks[input].getName());
		_decks.erase(_decks.begin() + input);
	}
	catch(const std::logic_error& e)
	{
		std::cout << "Wrong input!\n";
	}
	catch(const std::runtime_error& e)
	{
		std::cout << "Error: " << e.what() << "\n";
	}
}

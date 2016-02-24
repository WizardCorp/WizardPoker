// std-C++ headers
#include <iostream>
#include <cstdlib>
#include <cassert>
// SFML headers
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/SocketSelector.hpp>
// WizardPoker headers
#include "common/constants.hpp"
#include "client/ErrorCode.hpp"
#include "client/sockets/Client.hpp"
#include "common/sockets/TransferType.hpp"
#include "common/Terminal.hpp"
#include "common/ini/IniFile.hpp"
#include "client/states/GameState.hpp"
#include "client/NonBlockingInput.hpp"

GameState::GameState(StateStack& stateStack, Client& client):
	AbstractState(stateStack, client),
	_playing(true)
{
	addAction("Quit", &GameState::quit);
	std::cout << "Your game is about to start!\n";
	_client.waitTillReadyToPlay();
	initListening();
	sf::Packet packet;
	_client.getGameSocket().receive(packet);
	TransferType type;
	packet >> type;
	if(type != TransferType::GAME_STARTING)
		throw std::runtime_error("Wrong signal received: " + std::to_string(static_cast<sf::Uint32>(type)));
	packet >> type;
	begin();
	if(type == TransferType::GAME_PLAYER_ENTER_TURN)
	{
		_myTurn.store(true);
		startTurn();
	}
	else if(type == TransferType::GAME_PLAYER_LEAVE_TURN)
		_myTurn.store(false);
	else
		throw std::runtime_error("Wrong turn information: " + std::to_string(static_cast<sf::Uint32>(type)));
}

void GameState::display()
{
	std::cout << "Here are your options:\n";
	// Display the actions
	AbstractState::display();
}

void GameState::begin()
{
	addAction("Use card from hand", &GameState::useCard);
	addAction("Attack with a creature", &GameState::attackWithCreature);
	addAction("End your turn", &GameState::endTurn);
	/**/
}

void GameState::startTurn()
{
	display();
	std::cout << "It is now your turn, type something\n";
	while(_myTurn.load())
	{
		if(_nonBlockingInput.waitForData(0.1))
		{
			std::string command{_nonBlockingInput.receiveStdinData()};
			std::cout << "input is: '" + command + "'" << std::endl;
		}
	}
	/**/
}

void GameState::updateData(int pickedCard)
{
	++_nbrTurn;
	setEnergy(_nbrTurn);
	
	if(_remainCards > 0)
	{
		_inHand.push_back(pickedCard);
		--_remainCards; // Player took a card from his deck
	};
}

void GameState::changeHealth(int health)
{
	if(_selfHealth+health >= MAX_HEALTH)
		_selfHealth = MAX_HEALTH;
	else if (_selfHealth+health <= 0)
		_selfHealth = 0;
	else
		_selfHealth += health;
}

void GameState::changeOppoHealth(int health)
{
	if(_oppoHealth+health >= MAX_HEALTH)
		_oppoHealth = MAX_HEALTH;
	else if (_oppoHealth+health <= 0)
		_oppoHealth = 0;
	else
		_oppoHealth += health;
}

void GameState::changeEnergy(int energy)
{
	if(_energy+energy >= MAX_ENERGY)
		_energy = MAX_ENERGY;
	else if (_energy+energy <= 0)
		_energy = 0;
	else
		_energy += energy;
}


//PRIVATE METHODS

std::size_t GameState::selectHand()
{
	std::size_t res = 0;
	if(not _inHand.empty())
	{
		for(std::size_t i = 0; i < _inHand.size(); ++i)
			std::cout << i+1 << ". " << _inHand[i] << "\n";
		std::cout << "Enter the index of the card : ";
		std::cin >> res;
	}
	else
		std::cout << "You have no card in your hand";
	return res;
}
	
std::size_t GameState::selectBoard()
{
	std::size_t res = 0;
	if(not _onBoard.empty())
	{
		for(std::size_t i = 0; i < _onBoard.size(); ++i)
			std::cout << i+1 << ". " << _onBoard[i] << "\n";
		std::cout << "Enter the index of the card : ";
		std::cin >> res;
	}
	else
		std::cout << "You have not card on the board";
		
	return res;
}
		
std::size_t GameState::selectOppo()
{
	std::size_t res = 0;
	if(not _oppoBoard.empty())
	{
		for(std::size_t i = 0; i < _oppoBoard.size(); ++i)
			std::cout << i+1 << ". " << _oppoBoard[i] << "\n";
		std::cout << "Enter the index of the card : ";
		std::cin >> res;
	}
	else
		std::cout << "Your opponent has no card on the board";
		
	return res;
}

void GameState::setEnergy(unsigned energy)
{
	if(energy >= MAX_ENERGY)
		_energy = MAX_ENERGY;
	else
		_energy = energy;
}

void GameState::useCard()
{
	if (not _myTurn)
	{
		std::cout << "You must wait your turn!\n";
		return;
	}
	else
	{
		std::size_t cardIndex;
		std::cout << "Which card would you like to use?\n";
		cardIndex = selectHand();
		if(cardIndex > 0)	///If hand is not empty
			putOnBoard(cardIndex-1);
	}
}

void GameState::putOnBoard(std::size_t cardIndex)
{
	int card = _inHand[cardIndex];			 ///Save the card ID
	_inHand.erase(_inHand.begin()+cardIndex);///Remove it from the hand
	_onBoard.push_back(card);				 ///Put it on the board
}

void GameState::attackWithCreature()
{
	if (not _myTurn)
	{
		std::cout << "You must wait your turn!\n";
		return;
	}
	else
	{
		std::size_t selfCardIndex, oppoCardIndex;
		std::cout << "Which creature would you like to attack with?\n";
		selfCardIndex = selectBoard();

		std::cout << "Which opponent's creature would you like to attack?\n";
		oppoCardIndex = selectOppo();
		
		///If there's cards on board
		if(selfCardIndex > 0 && oppoCardIndex > 0)
		{
			//TODO send the attack
			//Note that the indexes are vector's indexes +1
			//0 indicate there's no card in the vector.
		};
	};
}

void GameState::applyOppoEffect()
/* In the case applies on a opponent's creature*/
{
	if(not _myTurn)
	{
		std::cout << "You must wait your turn!\n";
		return;
	}
	else
	{
		std::size_t oppoCardIndex;
		std::cout << 
		"On which opponent's creature would you like to apply the effect?\n";
		oppoCardIndex = selectOppo();
		
		if(oppoCardIndex > 0)
		{
			//TODO send the effect
			//Note that the indexes are vector's indexes +1
			//0 indicate there's no card in the vector.
		};
	};
}

void GameState::applySelfEffect()
{
	if(not _myTurn)
	{
		std::cout << "You must wait your turn!\n";
		return;
	}
	else
	{
		size_t selfCardIndex;
		std::cout << 
		"On which of your creature would you like to apply the effect?\n";
		selfCardIndex = selectBoard();
		
		if (selfCardIndex > 0)
		{
			//TODO send the effect
			//Note that the indexes are vector's indexes +1
			//0 indicate there's no card in the vector.
		};
	};
}

void GameState::endTurn()
{
	if (not _myTurn)
	{
		std::cout << "You must wait your turn!\n";
		return;
	}
	else
	{
		_myTurn = false;
	};
}

void GameState::quit()
{
	_playing.store(true);
	_listeningThread.join();
	stackPop();
}


//////////////// special data receival

void GameState::initListening()
{
	_listeningThread = std::thread(&GameState::inputListening, this);
}

void GameState::inputListening()
{
	sf::TcpSocket& listeningSocket{_client.getGameListeningSocket()};
	_client.waitTillReadyToPlay();
	sf::Packet receivedPacket;
	TransferType type;
	sf::SocketSelector selector;
	selector.add(listeningSocket);
	while(_playing.load())
	{
		if(!selector.wait(SOCKET_TIME_SLEEP))
			continue;
		assert(selector.isReady(listeningSocket));
		auto receiveStatus{listeningSocket.receive(receivedPacket)};
		if(receiveStatus == sf::Socket::Disconnected)
			;  // \TODO: handle disconnection
		else if(receiveStatus == sf::Socket::Error)
			;  // \TODO: handle error
		receivedPacket >> type;
		if(type == TransferType::GAME_OVER)
			_playing.store(false);
		else if(type == TransferType::GAME_PLAYER_ENTER_TURN)
			std::cout << "starting turn\n";  // perform turn changing here
		else if(type == TransferType::GAME_PLAYER_LEAVE_TURN)
			std::cout << "ending turn\n";  // perform turn changing here
		else
			std::cerr << "Unknown message received: " << static_cast<sf::Uint32>(type) << "; ignore." << std::endl;
	}
}

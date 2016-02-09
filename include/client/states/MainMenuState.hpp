#ifndef _MAIN_MENU_STATE_CLIENT_HPP
#define _MAIN_MENU_STATE_CLIENT_HPP

#include "client/AbstractState.hpp"
#include <vector>
#include <utility>
#include <string>
#include <functional>

// Forward declarations
class StateStack;

/// Cannot be more explicit.
class MainMenuState : public AbstractState
{
    public:
        /// Constructor.
        MainMenuState(StateStack& stateStack);

        /// The display function.
        /// It must do all things related to drawing or printing stuff on the screen.
        virtual void display() override;

        /// The event handling function.
        virtual void handleEvent() override;

    private:
        /// Helper method for adding an entry in the menu.
        /// \param actionName The name of the action.
        /// \param method A pointer to member that will be called when the
        /// action is triggered.
        void addAction(const std::string& actionName, void (MainMenuState::*method)());

        void findGame();
        void manageDecks();
		void seeCollection();
		void goToChat();
		void seeLadder();
		void addFriend();
        void logOut();

        std::vector<std::pair<std::string, std::function<void()>>> _actions;///< All actions doable in the menu.
};

#endif// _MAIN_MENU_STATE_CLIENT_HPP


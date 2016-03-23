#ifndef _GUI_MAIN_MENU_STATE_CLIENT_HPP
#define _GUI_MAIN_MENU_STATE_CLIENT_HPP

// External headers
#include <TGUI/TGUI.hpp>
// WizardPoker headers
#include "client/Gui/GuiAbstractState.hpp"
#include "client/states/AbstractMainMenuState.hpp"

/// Main menu in GUI Interface
class GuiMainMenuState: public GuiAbstractState, public AbstractMainMenuState
{
	public:
		/// Constructor
		GuiMainMenuState(Context& context);

	private:
		////////// Attributes
		const std::vector<ButtonData<GuiMainMenuState>> _buttons;

		tgui::Label::Ptr _menuLabel;

		tgui::VerticalLayout::Ptr _layout;

		////////// Methods
		void findGame();
		void manageDecks();
		void seeCards();
		void manageFriends();
		void seeLadder();
};

#endif  // _GUI_MAIN_MENU_STATE_CLIENT_HPP

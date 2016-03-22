#ifndef _GUI_LADDER_STATE_CLIENT_HPP
#define _GUI_LADDER_STATE_CLIENT_HPP

// External headers
#include <TGUI/HorizontalLayout.hpp>
#include <TGUI/VerticalLayout.hpp>

// WizardPoker headers
#include "common/Database.hpp"  // Ladder
#include "client/Gui/GuiAbstractState.hpp"
#include "client/states/AbstractLadderState.hpp"


/// Final class for the ladder with GUI.
class GuiLadderState : public GuiAbstractState, public AbstractLadderState
{
	public:
		/// Constructor.
		GuiLadderState(Context& context);

		/// Destructor. Deletes all widgets of this state.
		~GuiLadderState();

		/// Method called when another state is pushed on this one.
		/// By default, does nothing.
		/// Hides all the widgets.
		virtual void onPush() override;

		/// Method called when this state become the TOS again (e.g. this method
		/// is called on the HomeState instance when we log out from the main
		/// menu state).
		/// Shows all the widgets again.
		virtual void onPop() override;

	private:
		const std::vector<ButtonData<GuiLadderState>> _buttons;

		/// A structure that contains all widgets needed for displaying an entry
		/// of the ladder in GUI.
		struct GuiLadderEntry
		{
			/// Constructor.
			GuiLadderEntry();

			/// The rank field (from 1 to _ladder.size()).
			tgui::Label::Ptr rankLabel;

			/// The name of the player.
			tgui::Label::Ptr playerNameLabel;

			/// The number of games the player won.
			tgui::Label::Ptr wonGamesLabel;

			/// The number of games the player played.
			tgui::Label::Ptr playedGamesLabel;

			/// The layoud to ties all labels together.
			tgui::HorizontalLayout::Ptr layout;
		};

		/// The list of all lines of the ladder that will be displayed.
		std::array<GuiLadderEntry, std::tuple_size<decltype(_ladder)>::value> _guiLadder;

		/// The first line of the ladder (the top line with the names of the
		/// columns).
		GuiLadderEntry _ladderHeader;

		tgui::VerticalLayout::Ptr _ladderLayout;
		tgui::Label::Ptr _titleLabel;
};

#endif// _GUI_LADDER_STATE_CLIENT_HPP

#ifndef SPELL_GUI_CLIENT_HPP
#define SPELL_GUI_CLIENT_HPP

// WizardPoker headers
#include "client/Gui/CardGui.hpp"

class SpellGui: public CardGui
{
public:
	/// Constructor
	SpellGui(const std::string& name, const std::string& description, int cost);

private:
	/// Path of the image of the front side for a creature.
	static constexpr char FRONT_IMAGE_PATH[] = "../resources/client/spellFront.png";

};

#endif  // SPELL_GUI_CLIENT_HPP

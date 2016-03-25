#ifndef CARD_GUI_CLIENT_HPP
#define CARD_GUI_CLIENT_HPP

// std-C++ headers
#include <string>
// External headers
#include <SFML/Graphics.hpp>

/// TODO: handle line wraps in the description field.
/// Graphical representation of a card. It behaves like other SFML graphicals
/// classes, plus some getters/setters for the card interface.
class CardGui : public sf::Drawable, public sf::Transformable
{
public:
	/// Constructor.
	CardGui(const std::string& name, const std::string& description, int cost);

	/// Destructor.
	virtual ~CardGui() = default;

	/// Sets the cost of the card.
	void setCost(int cost);

	/// Sets which side of the card is drawn when the card is displayed.
	/// \param true to show the front (with the image, title, ...), false to
	/// show the back.
	void setShownSide(bool showFront);

	/// Returns the size of a card. Since all cards have the same size, this
	/// method is static.
	/// \return the size of a card.
	static sf::Vector2f getSize();

	/// Draws the object to a render target.
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

protected:
	/// The global size of the card, in pixels.
	static const sf::Vector2f SIZE;

	/// The main shape that contains the texture.
	sf::RectangleShape _picture;

	/// The texture displayed in front of the card.
	sf::Texture _pictureTexture;

	/// Ease of use method. It configures the card with the right font, position
	/// , text size and color.
	void setupText(sf::Text& text, const std::string& string, const sf::Vector2f& position) const;

private:
	/// Description graphical text.
	sf::Text _descriptionText;

	/// Name graphical text.
	sf::Text _nameText;

	/// Cost graphical text.
	sf::Text _costText;

	/// Text font.
	sf::Font _font;

	/// Position of the name relatively to the card.
	static const sf::Vector2f NAME_POSITION;

	/// Position of the cost relatively to the card.
	static const sf::Vector2f COST_POSITION;

	/// Position of the description relatively to the card.
	static const sf::Vector2f DESCRIPTION_POSITION;

	/// Character size.
	static constexpr std::size_t CHAR_SIZE = 20;

	/// Description character size.
	static constexpr std::size_t DESCRIPTION_CHAR_SIZE = 12;

	/// Path to the font.
	static constexpr char FONT_PATH[] = "../resources/client/FreeSans.otf";

	/// Path to the image of the back side of a card.
	static constexpr char BACK_IMAGE_PATH[] = "../resources/client/back.png";

	/// True when showing the front (with the image, title, ...), false when
	/// showing the back.
	bool _showFront;

	/// Shape that will contains the image of the back side.
	sf::RectangleShape _backView;

	/// Texture of the back side.
	sf::Texture _backTexture;
};



#endif  // CARD_GUI_CLIENT_HPP

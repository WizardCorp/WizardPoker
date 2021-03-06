#ifndef _DECK_COMMON_HPP
#define _DECK_COMMON_HPP

#include <array>
#include <string>
#include "common/Identifiers.hpp"

/// A deck is a set of cards.
/// All decks have the same amount of card, defined by Deck::size.
/// There is a special deck, the default deck
/// The default deck contains exactly Deck::size unique cards that match
/// the Deck::size cards all players start with.
class Deck
{
	public:
		static constexpr std::size_t size{20};
		typedef std::array<CardId, size>::iterator Iterator;
		typedef std::array<CardId, size>::const_iterator ConstIterator;

		/// Copy constructor.
		Deck(const Deck& other) = default;

		/// Destructor.
		virtual ~Deck() = default;

		/// Default constructor.
		/// Creates a default deck. The default deck is the set of the \a size
		/// first cards (with the cards ordered by their ID).
		/// Without the default value for name, this would impossible to
		/// construct a `std::vector<Deck>` because std::vector requires that its
		/// elements are default-constructible. This is not the case if name
		/// has no default value.
		explicit Deck(const std::string& name = ""); // should take Deck id

		/// Constructor.
		/// Creates a deck from an array of cards.
		Deck(const std::string& name, const std::array<CardId, size>& cards);

		CardId getCard(std::size_t index) const;

		std::size_t getIndex(CardId card) const;

		/// Replaces the card at \a index in the deck by \a card.
		/// \param searchWholeDeck True to check the whole deck for count <= 2
		/// and false to check from begin to index.
		void changeCard(std::size_t index, CardId card, bool searchWholeDeck=true);

		const std::string& getName() const;

		void setName(const std::string& newName);

		Iterator begin();

		Iterator end();

		ConstIterator begin() const;

		ConstIterator end() const;

		ConstIterator cbegin() const;

		ConstIterator cend() const;

	private:
		std::string _name;
		std::array<CardId, size> _cards;  ///< All the cards of the deck.
};

#endif  // _DECK_COMMON_HPP

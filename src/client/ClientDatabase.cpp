#include "client/ClientDatabase.hpp"

// WizardPocker
#include "client/ClientCreature.hpp"
#include "client/ClientSpell.hpp"
// std-C++
#include <cassert>

const char ClientDatabase::FILENAME[] = "../resources/client/database.db";
ClientDatabase::ClientDatabase(const std::string& filename) : Database(filename)
{
	for(size_t i = 0; i < _statements.size(); ++i)
		prepareStmt(_statements[i]);

	assert(sqliteThrowExcept(sqlite3_step(_countCardsStmt)) == SQLITE_ROW);
	_cardCount = sqlite3_column_int(_countCardsStmt, 0);

	assert(sqliteThrowExcept(sqlite3_step(_countCreaturesStmt)) == SQLITE_ROW);
	_creatureCount = sqlite3_column_int(_countCreaturesStmt, 0);

	assert(sqliteThrowExcept(sqlite3_step(_countSpellsStmt)) == SQLITE_ROW);
	_spellCount = sqlite3_column_int(_countSpellsStmt, 0);
}

Card ClientDatabase::getCard(CardId id)
{
	const CommonCardData* data(getCardData(id));

	if(data->isSpell())
		return ClientSpell(*static_cast<const ClientSpellData*>(data));
	else
		return ClientCreature(*static_cast<const ClientCreatureData*>(data));
}

const CommonCardData* ClientDatabase::getCardData(CardId id)
{
	auto found = _cards.find(id);

	if(found == _cards.end()) // not found: create and return
	{
		sqlite3_reset(_getCardStmt);
		sqliteThrowExcept(sqlite3_bind_int64(_getCardStmt, 1, id));

		assert(sqliteThrowExcept(sqlite3_step(_getCardStmt)) == SQLITE_ROW);

		CommonCardData * cardData;

		if(sqlite3_column_type(_getCardStmt, 4) == SQLITE_NULL)  // Spell
		{
			cardData = new ClientSpellData(
				id,
				reinterpret_cast<const char *>(sqlite3_column_text(_getCardStmt, 0)),  // name
				sqlite3_column_int(_getCardStmt, 1),  // cost
				reinterpret_cast<const char *>(sqlite3_column_text(_getCardStmt, 2))  // description
			);
		}
		else // Creature
		{
			cardData = new ClientCreatureData(
				id,
				reinterpret_cast<const char *>(sqlite3_column_text(_getCardStmt, 0)),  // name
				sqlite3_column_int(_getCardStmt, 1),  // cost
				reinterpret_cast<const char *>(sqlite3_column_text(_getCardStmt, 2)),  // description
				sqlite3_column_int(_getCardStmt, 3),  // attack
				sqlite3_column_int(_getCardStmt, 4),  // health
				sqlite3_column_int(_getCardStmt, 5),  // shield
				sqlite3_column_int(_getCardStmt, 6)  // shieldType
			);
		}

		return _cards.emplace(std::make_pair<>(
		                          id,
		                          std::unique_ptr<CommonCardData>(cardData)
		                      )).first->second.get();
	}
	else // card already created
	{
		return found->second.get();
	}
}

std::vector<CardId> ClientDatabase::getFirstCardIds(unsigned count)
{
	sqlite3_reset(_getFirstCardIdsStmt);
	sqliteThrowExcept(sqlite3_bind_int(_getFirstCardIdsStmt, 1, static_cast<int>(count)));

	std::vector<CardId> CardIds;

	while(sqliteThrowExcept(sqlite3_step(_getFirstCardIdsStmt)) == SQLITE_ROW)
	{
		CardIds.emplace_back(sqlite3_column_int64(_getFirstCardIdsStmt, 0));
	}

	return CardIds;
}

CardId ClientDatabase::getGreatestCardId()
{
	sqlite3_reset(_getGreatestCardIdStmt);

	assert(sqliteThrowExcept(sqlite3_step(_getGreatestCardIdStmt)) == SQLITE_ROW);
	return sqlite3_column_int(_getGreatestCardIdStmt, 0);
}

CardId ClientDatabase::countCards() const
{
	return _cardCount;
}

CardId ClientDatabase::countCreatures() const
{
	return _creatureCount;
}

CardId ClientDatabase::countSpells() const
{
	return _spellCount;
}

AchievementData ClientDatabase::getAchievementData(AchievementId id) const
{
	sqlite3_reset(_getAchievementDataStmt);
	sqliteThrowExcept(sqlite3_bind_int64(_getAchievementDataStmt, 1, id));

	assert(sqliteThrowExcept(sqlite3_step(_getAchievementDataStmt)) == SQLITE_ROW);

	return AchievementData
	{
		id,
		reinterpret_cast<const char *>(sqlite3_column_text(_getAchievementDataStmt, 0)), // name
		reinterpret_cast<const char *>(sqlite3_column_text(_getAchievementDataStmt, 1)), // description
		sqlite3_column_int(_getAchievementDataStmt, 2) // progressRequired
	};
}


ClientDatabase::~ClientDatabase()
{
	// TODO: move it to Database base class
	int errcode;

	for(size_t i = 0; i < _statements.size(); ++i)
		if((errcode = sqlite3_finalize(*_statements[i].statement())) != SQLITE_OK)
			std::cerr << "ERROR while finalizing statement "
			          << i + 1 << " of " << _statements.size()
			          << ": " << sqlite3_errstr(errcode)
			          << std::endl;

	/*for(_cards::const_iterator it = _cards.begin(); it != _cards.end(); ++it)
		delete *it;*/
}

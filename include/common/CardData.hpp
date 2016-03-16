#ifndef _CARD_DATA_HPP_
#define _CARD_DATA_HPP_

#include <string>
#include <vector>
#include "common/Identifiers.hpp"
#include "common/CardData.inc"

constexpr int UNLIMITED_TURNS=100;

///How to define an effect:
typedef std::vector<int> EffectParamsCollection;

struct EffectArgs
{
	EffectArgs(EffectParamsCollection effect): args(effect), index(0) {};
	EffectArgs(std::initializer_list<int> effect): args(effect), index(0) {};
	const EffectParamsCollection& args; // values that define the effect
	std::size_t index; // index indicating where values should be read
};
//In the following order:
//EFFECT SUBJECT [+ SUBJECT INDEX] if subject is identified by index (INDX at the end)
//EFFECT ID
//if effect is SET_CONSTRAINT:
//	CONSTRAINT ID
//	followed by VALUE, TURNS (or UNLIMITED_TURNS), CONSTRAINT CONDITIONS
//if effect is something else:
//	ALL ARGUMENTS (no more than 4)

///Creature struct
struct CreatureData
{
	std::string name;
	CostValue cost;
	AttackValue attack;
	HealthValue health;
	ShieldValue shield;
	ShieldType shieldType;
	std::vector<EffectParamsCollection> effects;
	std::string description;
};

///Spell struct
struct SpellData
{
	std::string name;
	CostValue cost;
	std::vector<EffectParamsCollection> effects;
	std::string description;
};

extern const CreatureData ALL_CREATURES[];

extern const SpellData ALL_SPELLS[];

extern const std::size_t nbSpells;

extern const std::size_t nbCreatures;

#endif  // _CARD_DATA_HPP

set(SOURCES
	"Deck.cpp"
	"CardsCollection.cpp"
	"CardData.cpp"
	"Card.cpp"
	"NotConnectedException.cpp"
	"UnableToConnectException.cpp"
	"Terminal.cpp"
	"ini/IniFile.cpp"
	"sockets/TransferType.cpp"
	"sockets/PacketOverload.cpp"
	"Database.cpp"
	# random
	"random/RandomInteger.cpp"
	"Ladder.cpp"
	)

add_library(${COMMON_NAME} ${SOURCES})

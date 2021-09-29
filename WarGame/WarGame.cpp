#include <iostream>
#include <list>
#include <map>

#include <string>

class Card
{
public:
    int cardValue;
    std::string* rawData;
    std::string cardSuit;

    /*static std::map<char, int> charToValue = {
        {'J', 11},
        {'Q', 12},
        {'K', 13},
        {'A', 14},
    };*/

    Card(std::string* str)
    {
        rawData = new std::string(*str);
        cardValue = Parse(rawData);
        cardSuit = str->length() == 3 ? rawData->at(2) : rawData->at(1);
    }

    static int Parse(const std::string* str)
    {
        char* p;
        strtol(str->c_str(), &p, 10);

        switch (*p)
        {
        case 'J': return 11;
        case 'Q': return 12;
        case 'K': return 13;
        case 'A': return 14;
        default: return std::stoi(*str);
        }
    }
};

class Deck
{
public:
    int GetCardsAmount() const
    {
        return _cards.size();
    }

    Card* GetNextCard()
    {
        const auto cardPointer = _cards.front();
        _cards.pop_front();
        return cardPointer;
    }

    void AddCard(Card card)
    {
        const auto buffer_card = new Card(card);
        _cards.push_back(buffer_card);
    }

    void AddCards(const std::list<Card*>* cards)
    {
        _cards.insert(_cards.end(), cards->begin(), cards->end());
    }

private:
    std::list<Card*> _cards;
};

class Player
{
public:
    Player(int cardAmount)
    {
        for (auto cardNumber = 0; cardNumber < cardAmount; cardNumber++)
        {
            std::string cardRawData;
            std::cin >> cardRawData;
            std::cin.ignore();

            Card card(&cardRawData);
            _deck.AddCard(card);
            //std::cout << *(card.rawData) << std::endl;
        }
    }

    bool HasCards()
    {
        return _deck.GetCardsAmount() > 0;
    }

    Card* NextCard()
    {
        const auto card = _deck.GetNextCard();
        _playedCards.push_back(card);
        return card;
    }

    std::list<Card*> ClaimPlayerCards()
    {
        auto buffer = _playedCards;
        _playedCards.clear();
        return buffer;
    }

    bool CanHandleWar() const
    {
        return _deck.GetCardsAmount() > 3;
    }

    void HandleWar()
    {
        for (auto cardNumber = 0; cardNumber < 3; cardNumber++)
        {
            _playedCards.push_back(_deck.GetNextCard());
        }
    }

    void AddCards(const std::list<Card*>* cards)
    {
        _deck.AddCards(cards);
    }

    static void HandleFightWon(Player* winner, Player* firstPlayer, Player* secondPlayer)
    {
        const auto bufferList = new std::list<Card*>();

        auto cardsBuffer = firstPlayer->ClaimPlayerCards();
        bufferList->insert(bufferList->end(), cardsBuffer.begin(), cardsBuffer.end());

        cardsBuffer.clear();
        cardsBuffer = secondPlayer->ClaimPlayerCards();
        bufferList->insert(bufferList->end(), cardsBuffer.begin(), cardsBuffer.end());

        winner->AddCards(bufferList);
    }

private:
    Deck _deck;
    std::list<Card*> _playedCards;
};

class WarCardGameController
{
public:
    Player* PlayerOne;
    Player* PlayerTwo;

    WarCardGameController(Player* playerOne, Player* playerTwo) : PlayerOne(playerOne), PlayerTwo(playerTwo)
    {
        GameLoop();
    }

    static bool HandleFight(Player* playerOne, Player* playerTwo)
    {
        auto firstPlayerCard = playerOne->NextCard();
        auto secondPlayerCard = playerTwo->NextCard();

        //std::cout << *firstPlayerCard->rawData << " vs " << *secondPlayerCard->rawData << std::endl;

        if (firstPlayerCard->cardValue == secondPlayerCard->cardValue)
        {
            if (!playerOne->CanHandleWar() || !playerTwo->CanHandleWar())
            {
                std::cout << "PAT";
                return false;
            }
            playerOne->HandleWar();
            playerTwo->HandleWar();

            return HandleFight(playerOne, playerTwo);
        }

        Player::HandleFightWon(firstPlayerCard->cardValue > secondPlayerCard->cardValue ? playerOne : playerTwo, playerOne, playerTwo);

        return true;
    }

private:
    int round = 0;

    void GameLoop()
    {
        while (PlayerOne->HasCards() && PlayerTwo->HasCards())
        {

            if(!HandleFight(PlayerOne, PlayerTwo))
            {
                return;
            }
            
            round++;  
        }

        int playerNumber = PlayerOne->HasCards() ? 1 : 2;
        std::cout << playerNumber << " " << round;
    }
};


int main(int argc, char* argv[])
{
    int cardsAmount; // the number of cards for player 1

    std::cin >> cardsAmount;
    std::cin.ignore();
    Player playerOne(cardsAmount);

    std::cin >> cardsAmount;
    Player playerTwo(cardsAmount);

    WarCardGameController game_controller(&playerOne, &playerTwo);


    return 0;
}

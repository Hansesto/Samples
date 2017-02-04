#include "gameboard.h"

GameBoard::GameBoard()
{
    initialize();
}

GameBoard::~GameBoard()
{

}

void GameBoard::initialize()
{
    //Asetetaan pelilaudan X-akseli
    gameBoard_.resize(GAMEBOARD_SIZE);
    for (int x = 0; x < GAMEBOARD_SIZE; ++x) {
        //Asetetaan pelilaudan Y-akseli
        gameBoard_.at(x).resize(GAMEBOARD_SIZE);
    }
}

int GameBoard::size()
{
    return GAMEBOARD_SIZE;
}

bool GameBoard::squareHasCharacter(int x, int y)
{
    //Jos lähtökoordinaateissa on ukko
    if (gameBoard_.at(x).at(y) != 0){
        return true;
    } else { //Jos lähtökoordinaateissa ei ole ukkoa
        return false;
    }
}

bool GameBoard::flyBullet(const std::string &type, std::pair<int, int> permStartCoords, QmlMessenger &qmlMsgr, std::string &direction, std::pair<int, int> startPos)
{
    int x = startPos.first;
    int y = startPos.second;

    if (direction == "left" && x >= 1) {

        //Jos seuraavassa ruudussa on targetti
        if (squareHasCharacter(x - 1, y)) {

            gameBoard_.at(x - 1).at(y)->loseHealth(5);
            qmlMsgr.splatter(std::make_pair(x - 1, y));

            //Jos ruudussa olleen kharakterin health <= 0
            if (gameBoard_.at(x - 1).at(y)->isDead()) {
                qmlMsgr.setDead(gameBoard_.at(x - 1).at(y)->type(), gameBoard_.at(x - 1).at(y)->name(), std::make_pair(x - 1, y));
                if (gameBoard_.at(x - 1).at(y)->type() == "teammate") {
                    //Poistetaan ukkeli loogiselta laudalta
                    gameBoard_.at(x - 1).at(y) = nullptr;
                    return false;
                }
                //Poistetaan ukkeli loogiselta laudalta
                gameBoard_.at(x - 1).at(y) = nullptr;
                return true;
            }
            //Osuma riippumatta oliko viimeinen tarvittava
            qmlMsgr.flyBullet(type, permStartCoords, std::make_pair(x - 1, y));
            if (gameBoard_.at(x - 1).at(y)->type() != "teammate") {
                return true;
            }

            return false;
        }
        else { //No hit (yet)

            //qmlMsgr.hideBullet(parkHallViewer);
            return flyBullet(type, permStartCoords, qmlMsgr, direction, std::make_pair(x - 1, y));
        }
    }
    else if (direction == "right" && x <= GAMEBOARD_SIZE - 2) {

        //Jos seuraavassa ruudussa on targetti
        if (squareHasCharacter(x + 1, y)) {

            gameBoard_.at(x + 1).at(y)->loseHealth(5);
            qmlMsgr.splatter(std::make_pair(x + 1, y));

            //Jos ruudussa olleen kharakterin health <= 0
            if (gameBoard_.at(x + 1).at(y)->isDead()) {
                qmlMsgr.setDead(gameBoard_.at(x + 1).at(y)->type(), gameBoard_.at(x + 1).at(y)->name(), std::make_pair(x + 1, y));
                if (gameBoard_.at(x + 1).at(y)->type() == "teammate") {
                    //Poistetaan ukkeli loogiselta laudalta
                    gameBoard_.at(x + 1).at(y) = nullptr;
                    return false;
                }

                //Poistetaan ukkeli loogiselta laudalta
                gameBoard_.at(x + 1).at(y) = nullptr;
                return true;
            }
            //Osuma riippumatta oliko viimeinen tarvittava
            qmlMsgr.flyBullet(type, permStartCoords, std::make_pair(x + 1, y));
            if (gameBoard_.at(x + 1).at(y)->type() != "teammate") {
                return true;
            }

            return false;
        }
        else { //No hit (yet)

            //qmlMsgr.hideBullet(parkHallViewer);
            return flyBullet(type, permStartCoords, qmlMsgr, direction, std::make_pair(x + 1, y));
        }
    }
    else if (direction == "up" && y >= 1) {

        //Jos seuraavassa ruudussa on targetti
        if (squareHasCharacter(x, y - 1)) {

            gameBoard_.at(x).at(y - 1)->loseHealth(5);
            qmlMsgr.splatter(std::make_pair(x, y - 1));

            //Jos ruudussa olleen kharakterin health <= 0
            if (gameBoard_.at(x).at(y - 1)->isDead()) {
                qmlMsgr.setDead(gameBoard_.at(x).at(y - 1)->type(), gameBoard_.at(x).at(y - 1)->name(), std::make_pair(x, y - 1));
                if (gameBoard_.at(x).at(y - 1)->type() == "teammate") {
                    //Poistetaan ukkeli loogiselta laudalta
                    gameBoard_.at(x).at(y - 1) = nullptr;
                    return false;
                }

                //Poistetaan ukkeli loogiselta laudalta
                gameBoard_.at(x).at(y - 1) = nullptr;
                return true;
            }
            //Osuma riippumatta oliko viimeinen tarvittava
            qmlMsgr.flyBullet(type, permStartCoords, std::make_pair(x, y - 1));
            if (gameBoard_.at(x).at(y - 1)->type() != "teammate") {
                return true;
            }

            return false;
        }
        else { //No hit (yet)

            //qmlMsgr.hideBullet(parkHallViewer);
            return flyBullet(type, permStartCoords, qmlMsgr, direction, std::make_pair(x, y - 1));
        }
    }
    else if (direction == "down" && y <= GAMEBOARD_SIZE - 2) {

        //Jos seuraavassa ruudussa on targetti
        if (squareHasCharacter(x, y + 1)) {

            gameBoard_.at(x).at(y + 1)->loseHealth(5);
            qmlMsgr.splatter(std::make_pair(x, y + 1));

            //Jos ruudussa olleen kharakterin health <= 0
            if (gameBoard_.at(x).at(y + 1)->isDead()) {
                qmlMsgr.setDead(gameBoard_.at(x).at(y + 1)->type(), gameBoard_.at(x).at(y + 1)->name(), std::make_pair(x, y + 1));
                if (gameBoard_.at(x).at(y + 1)->type() == "teammate") {
                    //Poistetaan ukkeli loogiselta laudalta
                    gameBoard_.at(x).at(y + 1) = nullptr;
                    return false;
                }

                //Poistetaan ukkeli loogiselta laudalta
                gameBoard_.at(x).at(y + 1) = nullptr;
                return true;
            }
            //Osuma riippumatta oliko viimeinen tarvittava
            qmlMsgr.flyBullet(type, permStartCoords, std::make_pair(x, y + 1));
            if (gameBoard_.at(x).at(y + 1)->type() != "teammate") {
                return true;
            }

            return false;
        }
        else { //No hit (yet)
            //qmlMsgr.hideBullet(parkHallViewer);
            return flyBullet(type, permStartCoords, qmlMsgr, direction, std::make_pair(x, y + 1));
        }
    }
    else { //Direction not yet defined OR bullet has reached an edge = NO HIT
        if (x != permStartCoords.first || y != permStartCoords.second) {
            qmlMsgr.flyBullet(type, permStartCoords, std::make_pair(x, y));
        }
        return false;
    }
    //No Hit
    return false;
}

void GameBoard::reset()
{
    for (int x = 0; x < GAMEBOARD_SIZE; ++x) {
        for (int y = 0; y < GAMEBOARD_SIZE; ++y) {
            gameBoard_.at(x).at(y) = nullptr;
        }
    }
}

std::pair<int, int> GameBoard::moveCharacter(const string &direction, string name)
{
    //Apumuuttujat
    bool found = false;
    pair <int, int> coords;

    //Katsotaan että ukkeli varmasti löytyy laudalta (Etsitään nimeä vastaava hahmo)
    for (int x = 0; x < GAMEBOARD_SIZE; ++x) {
        for (int y = 0; y < GAMEBOARD_SIZE; ++y) {
            //Katsotaan onko ruudussa ukkelia
            if (squareHasCharacter(x, y)) {

                //Verrataan ruudussa olevan ukkelin nimeä parametrina annettuun nimeen
                if (gameBoard_.at(x).at(y)->correctName(name)) {

                    //Ukkeli löytyi pisteestä x, y
                    coords = std::make_pair (x, y);
                    found = true;
                    break;
                }
            }
        }
    }

    //Ukkeli on pelilaudalla
    if (found == true) {
        //Ukon siirto
        if (direction == "left") {
            //Katsotaan ensin onnistuuko siirto
            if (moveLeft(coords)) {
                return std::make_pair(coords.first - 1, coords.second);
            }
        } else if (direction == "right"){
            //Katsotaan ensin onnistuuko siirto
            if (moveRight(coords)) {
                return std::make_pair(coords.first + 1, coords.second);
            }
        } else if (direction == "up"){
            //Katsotaan ensin onnistuuko siirto
            if (moveUp(coords)) {
                return std::make_pair(coords.first, coords.second - 1);
            }
        }
        else { //direction == "down"
            //Katsotaan ensin onnistuuko siirto
            if (moveDown(coords)) {
                return std::make_pair(coords.first, coords.second + 1);
            }
        }
    }
    return std::make_pair(100, 100); //Error, move not possible
}

std::pair<int, int> GameBoard::findLaura()
{
    for (int x = 0; x < GAMEBOARD_SIZE; ++x) {
        for (int y = 0; y < GAMEBOARD_SIZE; ++y) {
            //Katsotaan onko ruudussa ukkelia
            if (squareHasCharacter(x, y)) {
                if (gameBoard_.at(x).at(y)->name() == "laura") {
                    return std::make_pair(x, y);
                }
            }
        }
    }
    return std::make_pair (100, 100); //ERROR, Laura not found
}

void GameBoard::insertCharacter(std::shared_ptr<GameCharacter> character, const int &x, const int &y)
{
    if (character->type() == "enemy") {
        gameBoard_.at(x).at(y) = character;
    }
    else if (character->type() == "teammate") {
        gameBoard_.at(x).at(y) = character;
    }
    else { //Character type == Laura
        gameBoard_.at(x).at(y) = character;
    }
}

bool GameBoard::moveLeft(pair <int, int> &coords)
{
    //Tarkastetaan ensin että laudan reuna ei rajoita liikkumista
    if (coords.first != 0) {
        //Katsotaan että kohderuudussa ei ole ketään
        if (! squareHasCharacter(coords.first - 1, coords.second)) {
            //Ukon siirto
            gameBoard_.at(coords.first - 1).at(coords.second) =
                    gameBoard_.at(coords.first).at(coords.second);
            //Lähtöruudun nollaus
            gameBoard_.at(coords.first).at(coords.second) = nullptr;

            return true;
        }
        return false;
    }
    return false;
}

bool GameBoard::moveRight(pair <int, int> &coords)
{
    //Tarkastetaan ensin että laudan reuna ei rajoita liikkumista
    if (coords.first != GAMEBOARD_SIZE - 1) {
        //Katsotaan että kohderuudussa ei ole ketään
        if (! squareHasCharacter(coords.first + 1, coords.second)) {
            //Ukon siirto
            gameBoard_.at(coords.first + 1).at(coords.second) =
                    gameBoard_.at(coords.first).at(coords.second);
            //Lähtöruudun nollaus
            gameBoard_.at(coords.first).at(coords.second) = nullptr;

            return true;
        }
        return false;
    }
    return false;
}

bool GameBoard::moveUp(pair <int, int> &coords)
{
    //Tarkastetaan ensin että laudan reuna ei rajoita liikkumista
    if (coords.second != 0) {
        //Katsotaan että kohderuudussa ei ole ketään
        if (! squareHasCharacter(coords.first, coords.second - 1)) {
            //Ukon siirto
            gameBoard_.at(coords.first).at(coords.second - 1) =
                    gameBoard_.at(coords.first).at(coords.second);
            //Lähtöruudun nollaus
            gameBoard_.at(coords.first).at(coords.second) = nullptr;

            return true;
        }
        return false;
    }
    return false;
}

bool GameBoard::moveDown(pair <int, int> &coords)
{
    //Tarkastetaan ensin että laudan reuna ei rajoita liikkumista
    if (coords.second != GAMEBOARD_SIZE - 1) {
        //Katsotaan että kohderuudussa ei ole ketään
        if (! squareHasCharacter(coords.first, coords.second + 1)) {
            //Ukon siirto
            gameBoard_.at(coords.first).at(coords.second + 1) =
                    gameBoard_.at(coords.first).at(coords.second);
            //Lähtöruudun nollaus
            gameBoard_.at(coords.first).at(coords.second) = nullptr;

            return true;
        }
        return false;
    }
    return false;
}

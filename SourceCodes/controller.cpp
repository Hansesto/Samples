#include "controller.h"


Controller::Controller(std::shared_ptr<QtQuick2ApplicationViewer> viewer,
                       std::shared_ptr<QtQuick2ApplicationViewer> menuViewer,
                       std::shared_ptr<QtQuick2ApplicationViewer> parkHallViewer){
    viewer_ = viewer;
    menuViewer_ = menuViewer;
    parkHallViewer_ = parkHallViewer;

    sources_ = {"qrc:city.jpg", "qrc:park.jpg", "qrc:parkhallback.jpg", "qrc:yard.jpg"};

    gameLogic_ = std::make_shared<GameLogic>(viewer_);

    QObject::connect(&*gameLogic_, SIGNAL(backToMap(QString)), this, SLOT(fromParkHallSlot(QString)));

}

void Controller::gameFileSlot(const QString &name)
{
    std::shared_ptr<GameFileHandler> gameFile = std::make_shared<GameFileHandler>();

    //Returns true if creates new game i.e. file with the same name already exists
    if (gameFile->createNewGame(name)) {
        std::cout << "DEBUG: New Game: " << name.toStdString() << " created" << std::endl; //DEBUG
    }
    //File already exists
    else {
        std::cout << "DEBUG: File: " << name.toStdString() << ", already exists" << std::endl; //DEBUG
    }
}

void Controller:: fromMapSlot(const QString &msg) {

    if (msg == "Open Menu") {
        std::cout << "Message: " << msg.toStdString() << std::endl; //DEBUG

        gameLogic_->updateMoney(parkHallViewer_, viewer_, menuViewer_);

        //Muutetaan menuviewerin koko aluksi
        menuViewer_->resize(viewer_->size());

        viewer_->hide();
        menuViewer_->showFullScreen();
    }
    else if (msg == "Open parkHall") {
        std::cout << "Message: " << msg.toStdString() << std::endl; //DEBUG

        gameLogic_->updateMoney(parkHallViewer_, viewer_, menuViewer_);

        //Muutetaan parkhallviewerin koko aluksi että lauta pystytään alustamaan
        parkHallViewer_->resize(viewer_->size());

        //Alustetaan lauta
        gameLogic_->initGameBoard(parkHallViewer_);

        std::string source = sources_[rand()%4];

        //QML-viewer objekti josta voidaan etsiä lapsiobjekteja
        QObject *viewerObject = dynamic_cast<QObject *>(parkHallViewer_->rootObject());

        //Haetaan parkhallvieweristä characteri
        QObject *character = viewerObject->findChild<QObject*>(QString::fromStdString("phbackground"));
        QObject *aiTimer = viewerObject->findChild<QObject*>(QString::fromStdString("aiTimer"));


        if (character && aiTimer) {
            aiTimer->setProperty("running", true);
            character->setProperty("source", QVariant::fromValue(QString::fromStdString(source)));
        } else if (!character) {
        }

        viewer_->hide();
        parkHallViewer_->showFullScreen();
    }
}

void Controller::fromParkHallSlot(const QString &msg)
{
    if (msg == "BackToMapFromParkHall") {
        std::cout << "Message: " << msg.toStdString() << std::endl; //DEBUG

        gameLogic_->updateMoney(parkHallViewer_, viewer_, menuViewer_);

        parkHallViewer_->hide();
        viewer_->showFullScreen();
    }else if (msg == "shoot") {
        gameLogic_->shoot(parkHallViewer_);
        gameLogic_->updateMoney(parkHallViewer_, viewer_, menuViewer_);
    }
    else if (msg == "AIShoot") {
            gameLogic_->AI(parkHallViewer_);
        }
    else { //Message = "direction:name"
        std::string message = msg.toStdString();
        std::string delimiter = ":";
        std::string direction = message.substr(0, message.find(delimiter));
        message.erase(0, message.find(delimiter) + delimiter.size());
        std::string name = message;
        gameLogic_->moveCharacter(parkHallViewer_, direction, name);

        gameLogic_->updateMoney(parkHallViewer_, viewer_, menuViewer_);
    }
}

void Controller::fromMenuSlot(const QString &msg)
{
    if (msg == "backToMapFromMenu") {
        std::cout << "Message: " << msg.toStdString() << std::endl; //DEBUG

        gameLogic_->updateMoney(parkHallViewer_, viewer_, menuViewer_);
        gameLogic_->updateParkHalls();

        menuViewer_->hide();
        viewer_->showFullScreen();

        return;
    }

    std::string message = msg.toStdString();
    std::string delimiter = ":";
    std::string type = message.substr(0, message.find(delimiter));
    message.erase(0, message.find(delimiter) + delimiter.size());
    std::string itemName = message;

    std::cout << "Type: " << type <<  ", item: " << itemName << std::endl; //DEBUG

    if (type == "weapon") {
        gameLogic_->setWeapon(menuViewer_, itemName);
        gameLogic_->updateMoney(parkHallViewer_, viewer_, menuViewer_);
    }

    else if (type == "armor") {
        gameLogic_->setArmor(menuViewer_, itemName);
        gameLogic_->updateMoney(parkHallViewer_, viewer_, menuViewer_);
    }
}

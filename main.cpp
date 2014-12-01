/****************************************************************************
****************************************************************************/

#include <QApplication>

#include "Animal_card_app.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Animal_card_app card_app;
    card_app.show();
    return app.exec();
}

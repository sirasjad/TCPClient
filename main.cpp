#include <QApplication>
#include "client.h"

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    TCPClient client;
    client.show();

    return a.exec();
}

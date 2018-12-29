#include "client.h"
#include "ui_client.h"

TCPClient::TCPClient(QWidget *parent) : QMainWindow(parent), ui(new Ui::TCPClient){
    ui->setupUi(this);
}

TCPClient::~TCPClient(){
    delete ui;
}

void TCPClient::addLog(QString text){
    QDateTime dateTime = dateTime.currentDateTime();
    QString dateTimeString = dateTime.toString("hh:mm:ss");
    ui->logBox->appendPlainText("[" + dateTimeString + "] " + text);
}

void TCPClient::on_connectButton_clicked(){
    if(!ui->ipBox->text().isEmpty() && !ui->portBox->text().isEmpty() && !ui->studBox->text().isEmpty()){
        std::string tempIP = ui->ipBox->text().toStdString();
        int tempPort = ui->portBox->text().toInt();
        std::string studNumber = ui->studBox->text().toStdString();

        if(!socket.getConnectionStatus()){
            try{
                socket.verifyStudent(studNumber);
                socket.makeConnection(tempIP, tempPort);
                addLog("Connected to " + ui->ipBox->text() + " on port " + ui->portBox->text());

                socket.requestPort();
                short newPort = socket.receivePort();
                addLog("Received a new port from server (" + QString::number(newPort) + ")");
                addLog("Disconnecting from current port (" + ui->portBox->text() + ")");
                socket.abortConnection();

                socket.makeConnection(tempIP, newPort);
                addLog("Re-connected to " + ui->ipBox->text() + " on port " + QString::number(newPort));
                ui->connectButton->setText("Disconnect");

                Server::MessageID response = socket.portResponse();
                while(response == Server::MessageID::PING){                            
                    addLog("Server is pinging the client, returning a pong in response");
                    socket.pongServer();
                    response = socket.portResponse();
                }
                if(response == Server::MessageID::QUIT) addLog("Connection aborted! Disconnecting from the server");
                socket.abortConnection();
                ui->connectButton->setText("Connect");
            }
            catch(Server::Errors e){
                switch(e){
                    case Server::INVALID_STUDNR: addLog("Invalid student number!"); break;
                    case Server::SOCKET_ERROR: addLog("Failed to create a socket!"); break;
                    case Server::INVALID_CONNECTION: addLog("Failed to connect to the server!"); break;
                    case Server::INVALID_PORTREQUEST: addLog("Failed to receive a new port from the server!"); break;
                    case Server::PING_ERROR: addLog("Server could not ping the client properly!"); break;
                    case Server::PONG_ERROR: addLog("Client could not pong the server properly!"); break;
                    case Server::INVALID_PORTRESPONSE: addLog("Invalid port response!"); break;
                }
            }
            catch(...){ addLog("Unknown error occured!"); }
        }
        else{
            socket.abortConnection();
            addLog("Disconnected from the server");
            ui->connectButton->setText("Connect");
        }
    }
    else addLog("Please enter the server IP address, port number and student number!");
}

void TCPClient::on_clearButton_clicked(){
    ui->logBox->clear();
}

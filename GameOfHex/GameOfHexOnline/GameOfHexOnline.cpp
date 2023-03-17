#include <iostream>
#include <regex>
#include <mutex>
#include <vector>
#include <queue>
#include <unordered_map>
#include "HexGameComponent.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <sstream>

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 1024
#define DEFAULT_PORT "8888" //Port目前固定,尚未加入讀檔

//message receive from client
//msg="msgType,playerName|"
//msg="0,playerName|" //enroll
//msg="1,playerName,(1,1)|" //


//message send to client
//msg="msgType,msg|"
//msg="-1,error|" //error msg
//msg="0,login sucess|" //login notification
//msg="1,game start|" //game start notification
//msg="2,your turn|" //turn notification
//msg="3,lose or win" //result notification

std::string PlayerError = "-1,error|";
std::string LoginSucess = "0,login sucess|";
std::string GameStartAsPlayer1 = "1,game start,player1|";
std::string GameStartAsPlayer2 = "1,game start,player2|";
std::string PlayerTurn = "2,your turn";
std::string playerWin = "3,you win|";
std::string playerLose = "3,you loose|";
std::string drawGame = "3,draw game|";

std::vector<std::string> splitStr(std::string str, std::string&& delim);

enum class PlayingStatus :short
{
    Waiting, Playing, Idle
};
class OnlinePlayer
{
public:
    SOCKET socket;
    std::string playerName;
    
    PlayingStatus status;
    Node::EnumNodePlayerOrderStatus CurrentOrder;

    OnlinePlayer(SOCKET socket, std::string playerName, PlayingStatus status = PlayingStatus::Waiting)
    {
        this->socket = socket;
        this->playerName = playerName;
        this->status = status;
    };

    void notifyPlayerLoginSucess()
    {
        send(socket, LoginSucess.c_str(), LoginSucess.size(), 0);
    }

    void notifyPlayerError()
    {
        send(socket, PlayerError.c_str(), PlayerError.size(), 0);
    }

    void notifyPlayerTurn(std::string& chessBoardData)
    {
        auto tempPlayerTurn = PlayerTurn;
        tempPlayerTurn.append(",");
        tempPlayerTurn.append(chessBoardData);
        tempPlayerTurn.append("|");
        send(socket, tempPlayerTurn.c_str(), tempPlayerTurn.size(), 0);
    }
    void notifyPlayerWin()
    {
        send(socket, playerWin.c_str(), playerWin.size(), 0);
    }
    void notifyPlayerLose()
    {
        send(socket, playerLose.c_str(), playerLose.size(), 0);
    }
    ~OnlinePlayer()
    {
       
    }

};
enum class ERoomStatus :short { idle, playing, end };

class GameHost;

//單一棋局
class GameRoom
{
private:
    int boardSize =5;
  
public:
    static int roomId_s;
    int roomId;
    OnlinePlayer *player1, *player2;   
    HexGraph hexGraph = HexGraph(boardSize);
    ERoomStatus RoomStatus;
    GameHost* gameHost;

    GameRoom(OnlinePlayer* player1, OnlinePlayer* player2,GameHost* gameHost)
    {
        roomId=++roomId_s;
        RoomStatus = ERoomStatus::idle;
        this->player1 = player1;
        this->player2 = player2;
        this->gameHost = gameHost;
    }
    //通知該局使用者遊戲開始
    void notifyUserGameStart()
    {
     
        send(player1->socket, GameStartAsPlayer1.c_str(), GameStartAsPlayer1.size(), 0);
        send(player2->socket, GameStartAsPlayer2.c_str(), GameStartAsPlayer2.size(), 0);
    }
    //對局開始 (呼叫1個執行緒處理1個棋局,可改用select或iocp改善降低server負擔)
    void runGameThd()
    {
        std::thread thd(&GameRoom::runGame,this);
        thd.detach();
    }
    //對局開始
    void runGame();
    //提供client端棋盤資訊格式
    std::string showBoardMoves(HexGraph& hexGraph)
    {
        std::string str;
        for (auto& nd : hexGraph.NodeList)
            str.append(std::to_string((int)nd->NodeStatus));
        return str;
    }

};
int GameRoom::roomId_s = 0;
//Server主要類別,掌控多個棋局
class GameHost
{
public:
    std::mutex* HostLock,*QLock;
    std::vector<GameRoom*> rooms;
    std::queue<OnlinePlayer*> playerQueue;
    std::unordered_map<std::string,OnlinePlayer*> players;
    GameHost()
    {
        HostLock = new std::mutex();
        QLock = new std::mutex();
    }
    //依玩家數量創建棋局(房間)
    void createRooms() //Match pair user to game room
    {
        int countToPair = 0;
        std::vector<GameRoom*> NewRoooms;
        HostLock->lock();
        while (playerQueue.size() > 0) //擷取等待中user配對入game room
        {
            if (playerQueue.size() > 1)
            {
                auto player1 = playerQueue.front();
                playerQueue.pop();
                auto player2 = playerQueue.front();
                playerQueue.pop();
                
                GameRoom* gameRoom = new GameRoom(player1, player2,this);
                NewRoooms.push_back(gameRoom);
            }
            if (playerQueue.size() == 1 && countToPair % 2 == 0)
                break;
        }
        HostLock->unlock();

        for (auto& room : NewRoooms)
        {
            room->notifyUserGameStart(); //通知player遊戲開始
        }
        HostLock->lock();
        for (auto& room : NewRoooms)
            rooms.push_back(room);
        NewRoooms.clear();
        HostLock->unlock();

    }
    //啟動配對好的房間棋局
    void ActivateIdleRoom()
    {
        for (auto& room : rooms)
            if (room->RoomStatus == ERoomStatus::idle)
            {
                room->RoomStatus = ERoomStatus::playing;
                room->runGameThd();
                //room->runGame();
            }
    }
    //刪除離線玩家
    void ClearPlayer(OnlinePlayer* player1, OnlinePlayer* player2)
    {
        HostLock->lock();
        players.erase(player1->playerName);
        players.erase(player2->playerName);
        delete player1;
        delete player2;
        HostLock->unlock();
    }
    //刪除結束的棋局
    void ClearEndRoom()
    {
        std::vector<GameRoom*> roomsToRemain;
        std::vector<GameRoom*> roomsToRemove;
        for (auto& room : rooms)
        {
            if (room->RoomStatus == ERoomStatus::end)
                roomsToRemove.push_back(room);
            else if (room->RoomStatus != ERoomStatus::end)
                roomsToRemain.push_back(room);
        }
        HostLock->lock();
        for (auto& room : roomsToRemove)
            delete room;
        roomsToRemove.clear();
        rooms.clear();
        rooms = roomsToRemain;
        HostLock->unlock();  
    }
    //新增玩家
    bool addUser(SOCKET socket, std::string userName)
    {
        try
        {
            if (players.find(userName) != players.end())
            {
                send(socket, PlayerError.c_str(), PlayerError.size(), 0); //已有相同名稱使用者,拒絕登錄
                return false;
            }
            else
            {
                players[userName] = new OnlinePlayer(socket, userName);
                playerQueue.push(players[userName]);
                players[userName]->notifyPlayerLoginSucess();
            }
        }
        catch (std::exception ex)
        {
            std::cout << "adduser error:\n";
            std::cout << ex.what() << std::endl;
            return false;
        }
        return true;
    }
    //Server主程序
    void run()
    {
        WSADATA wsaData;
        int iResult;

        SOCKET ListenSocket = INVALID_SOCKET;
        SOCKET ClientSocket = INVALID_SOCKET;

        struct addrinfo* result = NULL;
        struct addrinfo hints;

        int iSendResult;
        char recvbuf[DEFAULT_BUFLEN];
        int recvbuflen = DEFAULT_BUFLEN;

        // Initialize Winsock
        iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0) {
            printf("WSAStartup failed with error: %d\n", iResult);
           // return 1;
        }

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;

        // Resolve the server address and port
        iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result); //
        if (iResult != 0) {
            printf("getaddrinfo failed with error: %d\n", iResult);
            WSACleanup();
           
            //return 1;
        }

        // Create a SOCKET for the server to listen for client connections.
        ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (ListenSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            freeaddrinfo(result);
            WSACleanup();
            //return 1;
        }

        // Setup the TCP listening socket
        iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            printf("bind failed with error: %d\n", WSAGetLastError());
            freeaddrinfo(result);
            closesocket(ListenSocket);
            WSACleanup();
            //return 1;
        }

        freeaddrinfo(result);

        while (true)
        {
            ZeroMemory(recvbuf, recvbuflen);
            iResult = listen(ListenSocket, SOMAXCONN);
            if (iResult == SOCKET_ERROR) {
                printf("listen failed with error: %d\n", WSAGetLastError());
                closesocket(ListenSocket);
                WSACleanup();
                //return 1;
            }

            // Accept a client socket
            ClientSocket = accept(ListenSocket, NULL, NULL);
            if (ClientSocket == INVALID_SOCKET) {
                printf("accept failed with error: %d\n", WSAGetLastError());
                closesocket(ListenSocket);
                WSACleanup();
                //return 1;
            }

            iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
            if (iResult > 0)
            {
                std::cout << std::string(recvbuf) << std::endl;
                auto str = splitStr(recvbuf, "|")[0];
                auto name = splitStr(str.c_str(), ",")[1];
                if (addUser(ClientSocket, name))
                {
                    createRooms();
                    ActivateIdleRoom();
                    
                    displayServerStatus();
                }
            }
        }
        // No longer need server socket
        //closesocket(ListenSocket);

        // Receive until the peer shuts down the connection
       



        SOCKET client;
       
        WSACleanup();
    }

    void displayServerStatus()
    {
        system("cls");
        //Display room
        HostLock->lock(); //可改用read-write lock優化
        std::cout << "room id: player1,player2\r\n";
        for(auto &room:rooms)
            std::cout << room->roomId << ":" << room->player1->playerName << "," << room->player2->playerName << "\r\n";
        std::cout << "==============" << std::endl;
        //Display player
        std::cout << "playerList" << std::endl;
        for (auto& player : players)
            std::cout << player.first << std::endl;
        HostLock->unlock();
    }
};

void GameRoom::runGame()
{

    std::map< Node::EnumNodePlayerOrderStatus, OnlinePlayer*> playerRelation;
    playerRelation[Node::EnumNodePlayerOrderStatus::FirstPlayer] = player1;
    playerRelation[Node::EnumNodePlayerOrderStatus::SecondPlayer] = player2;
    std::string PlayerInput;
    HexGraph hexGraph = HexGraph(boardSize);

    char* buff = new char[1024];
    while (true)
    {
        ZeroMemory(buff, 1024);
        auto curPlayer = playerRelation[hexGraph.NextMovePlayer()];

        auto boardInfo=showBoardMoves(hexGraph);
        curPlayer->notifyPlayerTurn(boardInfo);
        int recvCode = recv(curPlayer->socket, buff, 1024, 0); //等待玩家輸入
       
        if (recvCode <= 0) //斷線
        {
            std::cout << "connection error during game play\r\n";
            break;
        }

        PlayerInput = std::string(buff);;
        PlayerInput = std::regex_replace(PlayerInput,
            std::regex("\\r\\n|\\r|\\n"),
            ""); //針對結尾有\r\n的資料處理 
        std::string msg;
        if (!hexGraph.place(PlayerInput, hexGraph.NextMovePlayer(), msg)) //確認棋子是否擺放正常
        {
            playerRelation[hexGraph.NextMovePlayer()]->notifyPlayerError(); //通知使用者異常
        }
        else
        {
           // std::cout << hexGraph.showArrangedPlayerNodes() << std::endl;; //Display the current board as the player sucessfully placed a valid move
            std::string currPlayer;
            if (hexGraph.JudgeWinner(*hexGraph.LastNodePlaced, currPlayer))  //If someone win the game, announce the winner and end the game
            {
                playerRelation[hexGraph.LastMovedPlayer()]->notifyPlayerWin();
                playerRelation[hexGraph.NextMovePlayer()]->notifyPlayerLose();
                break;
            }           
        }
    }
    closesocket(player1->socket);
    closesocket(player2->socket);   
    this->RoomStatus = ERoomStatus::end;
    gameHost->ClearPlayer(player1, player2); //清理賽局結束玩家
    gameHost->ClearEndRoom(); //清理賽局
}
std::vector<std::string> splitStr(std::string str, std::string&& delim)
{
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string tok;

    int index;    
    while ((index = str.find(delim)) != -1)
    {
        result.push_back(std::string(str.begin(), str.begin() + index));
        str.erase(str.begin(), str.begin() + index+1);
    }
    if (str.size() > 0)
        result.push_back(str);

    return result;
}


int main()
{
    GameHost gameHost;
    gameHost.run();  
    return 0;
}


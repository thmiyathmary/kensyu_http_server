#include <stdio.h> //printf(), fprintf(), perror()
#include <sys/socket.h> //socket(), bind(), accept(), listen()
#include <arpa/inet.h> // struct sockaddr_in, struct sockaddr, inet_ntoa()
#include <stdlib.h> //atoi(), exit(), EXIT_FAILURE, EXIT_SUCCESS
#include <string.h> //memset()
#include <unistd.h> //close()

#define QUEUELIMIT 5

int startUp(unsigned short);
void receiveRequest(int);
void makeHttp(char*);

int main(int argc, char* argv[]) {
    unsigned short servPort; //server port number
    int servSock; //server socket descripter
    // 引数が2個(1つは実行するファイル名のため、実質1つ)でなければ終了
    if ( argc != 2) {
        fprintf(stderr, "argument count mismatch error.\n");
        exit(EXIT_FAILURE);
    }
    // 2つ目の引数(ポート番号)が文字列から数字に変換できなければ終了
    if ((servPort = (unsigned short) atoi(argv[1])) == 0) {
        fprintf(stderr, "invalid port number.\n");
        exit(EXIT_FAILURE);
    }
    servSock = startUp(servPort);
    receiveRequest(servSock);

    return EXIT_SUCCESS;
}

int startUp(unsigned short servPort) {
    int servSock; //server socket descripter
    struct sockaddr_in servSockAddr; //server internet socket address

    // socket()が正常に実行されなかった場合終了
    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 ){
        perror("socket() failed.");
        exit(EXIT_FAILURE);
    }

    // servSockAddrの設定
    // sin_familyはプロトコルの種類の指定?
    // sin_addr.s_addrは受け入れるIPアドレス(INADDR_ANYだとなんでも受け入れる)
    // sin_portは受け入れるポート番号
    memset(&servSockAddr, 0, sizeof(servSockAddr));
    servSockAddr.sin_family      = AF_INET;
    servSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servSockAddr.sin_port        = htons(servPort);

    // サーバが利用するIPアドレスとポートを設定する
    // 正常に実行されなかった場合終了
    if (bind(servSock, (struct sockaddr *) &servSockAddr, sizeof(servSockAddr) ) < 0 ) {
        perror("bind() failed.");
        exit(EXIT_FAILURE);
    }

    // 外部からの通信を受け付ける
    // QUEUELIMITは接続待ちの数?
    // 正常に実行されなかった場合終了
    if (listen(servSock, QUEUELIMIT) < 0) {
        perror("listen() failed.");
        exit(EXIT_FAILURE);
    }
    return servSock;
}

void receiveRequest(int servSock) {
    int clitSock; //client socket descripter
    struct sockaddr_in clitSockAddr; //client internet socket address
    unsigned int clitLen; // client internet socket address length
    char buf[2048];

    while(1) {
        clitLen = sizeof(clitSockAddr);
        if ((clitSock = accept(servSock, (struct sockaddr *) &clitSockAddr, &clitLen)) < 0) {
            perror("accept() failed.");
            exit(EXIT_FAILURE);
        }

        printf("connected from %s.\n", inet_ntoa(clitSockAddr.sin_addr));
        makeHttp(buf);
        printf("%c",buf[0]);
        printf("%c",buf[1]);
        printf("%c",buf[2]);
        printf("%c",buf[3]);
        send(clitSock, buf, (int)strlen(buf), 0);

    //   close(clitSock);
    }
}

 void makeHttp(char buf[]){
     // 応答用HTTPメッセージ作成
    memset(buf, 0, sizeof(buf));
    snprintf(buf, sizeof(buf),
	 "HTTP/1.0 200 OK\r\n" // レスポンスヘッダ
	 "Content-Length: 20\r\n" // 各種ヘッダ
	 "Content-Type: text/html\r\n" // 各種ヘッダ
	 "\r\n" // 空行
	 "HELLO\r\n"); // ボディ
 }
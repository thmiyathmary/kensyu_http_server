#include <stdio.h> //printf(), fprintf(), perror()
#include <sys/socket.h> //socket(), bind(), accept(), listen()
#include <sys/fcntl.h> // O_RDONLY
#include <arpa/inet.h> // struct sockaddr_in, struct sockaddr, inet_ntoa()
#include <stdlib.h> //atoi(), exit(), EXIT_FAILURE, EXIT_SUCCESS
#include <string.h> //memset()
#include <unistd.h> //close()

#define QUEUELIMIT 5
#define BUFSIZE 2048
unsigned short servPort = 8080;
char *DOCUMENT_ROOT = "C:/Users/HAYATO/Desktop/GItHub/kensyu_http_server/html";

int startUp(unsigned short);
void receiveRequest(int);
void sendHttp(int);
int sendMessage(int, char *);


int main(int argc, char* argv[]) {
    int servSock; //server socket descripter
    int i;
    for (i = 1; i < argc; i++) {
        printf("%d %d\n", i, argc);
        printf("%s %s\n", argv[i], argv[2]);
        if (strcmp(argv[i], "-p") == 0) {
            printf("test%s\n", argv[i]);
            if ((servPort = (unsigned short) atoi(argv[++i])) == 0) {
                fprintf(stderr, "invalid port number.\n");
                exit(EXIT_FAILURE);
            }
        } else if (strcmp(argv[i], "-d") == 0) {
            // printf("%d\n", strcmp(argv[i], "-d"));
            // strcpy(DOCUMENT_ROOT, argv[++i]);
            // printf("%s %s\n", argv[i], argv[2]);
            // DOCUMENT_ROOT[strlen(argv[i])] = '\0';
            printf("%s", DOCUMENT_ROOT);
            DOCUMENT_ROOT = argv[++i];
            printf("%s", DOCUMENT_ROOT);
        }
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

    while(1) {
        clitLen = sizeof(clitSockAddr);
        if ((clitSock = accept(servSock, (struct sockaddr *) &clitSockAddr, &clitLen)) < 0) {
            perror("accept() failed.");
            exit(EXIT_FAILURE);
        }

        printf("connected from %s.tmp\n", inet_ntoa(clitSockAddr.sin_addr));
        sendHttp(clitSock);
    //   close(clitSock);
    }
}

 void sendHttp(int clitSock){
    int len;
    char buf[BUFSIZE];
    char recieveBuf[BUFSIZE];
    char method[16];
    char uri_addr[256];
    char http_ver[64];
    char fileDir[256];
    int fileDesc;

    if (read(clitSock, recieveBuf, BUFSIZE) <= 0) {
        fprintf(stderr, "error: reading a request.\n");
    } else {
        sscanf(recieveBuf, "%s %s %s", method, uri_addr, http_ver);
        printf("uri_addr : %s\n", uri_addr);

        memset(fileDir, 0, sizeof(fileDir));
        snprintf(fileDir, sizeof(fileDir), "%s%s",DOCUMENT_ROOT, uri_addr);
        if (fileDir[strlen(fileDir) - 1] == '/') {
            snprintf(fileDir, sizeof(fileDir), "%s%s",fileDir, "index.html");
        }

        printf("fileDir : %s\n",  fileDir);
        memset(buf, 0, sizeof(char) * BUFSIZE);
        if ((fileDesc = open(fileDir, O_RDONLY, 0666)) == -1) {
            printf("NULL\n");
	        sendMessage(clitSock, "404 Not Found");
        } else {
            printf("Exist\n");
            sendMessage(clitSock, "HTTP/1.0 200 OK\r\n");
            sendMessage(clitSock, "text/html\r\n");
            sendMessage(clitSock, "\r\n");
            // sendMessage(clitSock, "sendMessage\r\n");
            while((len = read(fileDesc, buf, BUFSIZE)) > 0) {
                if (write(clitSock, buf, len) != len) {
                    fprintf(stderr, "error: writing a response.\n");
                    break;
                }
            }
            close(fileDesc);
        }
        close(clitSock);
    }    
 }

int sendMessage(int fd, char *msg) {
    int len;
    len = strlen(msg);
    if ( write(fd, msg, len) != len ){
        fprintf(stderr, "error: writing.");
    }
    return len;
}
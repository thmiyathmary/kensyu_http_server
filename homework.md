【注意事項】HTTPサーバーの開発にはgccを利用すること。
> ただし、C言語で開発することが条件であり、（単一行コメントを除き）C++において導入された言語構文を利用しないこと！

# 課題①
下記サーバおよびクライアントのソケット処理のコードをベースに  
固定のメッセージを返却する簡単なHTTPサーバーを作りなさい。  
[参考 : ソケットとは](http://tech.nikkeibp.co.jp/it/article/COLUMN/20071031/285990/)  

【URL】https://qiita.com/tajima_taso/items/2f0606db7764580cf295  
【URL】https://qiita.com/tajima_taso/items/fb5669ddca6e4d022c15  

【URL】http://www.geekpage.jp/programming/linux-network/http-server.php  
【URL】http://www.geekpage.jp/programming/winsock/http-server.php  

# 課題②
main関数だけのモノリシックな構造を見直して、
HTTPサーバーとしての拡張に耐えるように外科手術を施しなさい。
その際、ソースコードの構造を理解した上で作業を行うこと。

【URL】http://msdn.microsoft.com/en-us/library/windows/desktop/bb530742%28v=vs.85%29.aspx  

# 課題③
定数「DOCUMENT_ROOT」を宣言し、当該ディレクトリ配下のコンテンツを適正に処理しなさい。
リクエストされたファイルが存在すれば、200のOKメッセージとともにその内容を返却する。
リクエストされたファイルが存在しなければ、404のNOT FOUNDメッセージを出力する。
対象となるのは、DOCUMENT_ROOT直下に配置された拡張子「html」または「htm」のみとする。

【URL】http://www.wisdomsoft.jp/403.html  

# 課題④
プログラム内部で定義した定数だけでなく、
環境変数や実行時引数でポート番号やDOCUMENT_ROOTを変更できるようにしなさい。

```
#define DEFAULT_PORT 12345
#define DOCUMENT_ROOT "/Users/USERNAME/desktop/www"

//FIXME priority: 1st runtime parameter, 2nd environment varibale %DOCUMENT_ROOT%, 3rd constants
//TODO ./tcps -p 8080
//TODO ./tcps -d "/Users/USERNAME/tmp/www"
//TODO ./tcps -p 8080 -d "/Users/tmp/desktop/www"
//TODO ./tcps ==> -d (%DOCUMENT_ROOT%) : getenv("DOCUMENT_ROOT")
//TODO ./tcps ==> -p DEAULT_PORT -d DOCUMENT_ROOT
```

# 課題⑤
リクエストの対象をDOCUMENT_ROOT配下のサブディレクトリに拡張しなさい。

# 課題⑥
画像やスタイルシートなど、ファイル種別の限定を解除しなさい。
ただし、CGIのサポートは不要（静的コンテンツのみが対象）で良い。

const string HELLO_YK = "yk"

service HelloWorld {
    void ping(),
    string sayHello(),
    string sayMsg(1:string msg)
}

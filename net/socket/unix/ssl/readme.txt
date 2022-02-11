1,server:
    a,支持导入加密的private key文件
    b,cmd
        ./service self.x509 self.passwd.priv
        通过屏蔽server.c内部的cb代码可以看出对于passwd priv的区别.
    c,pwd:
        self.passwd.priv--> Inspur_pangwz
    
    
2,client:
    a,支持导入可信证书，校验server的cert.  对校验结果有影响.
    b,cmd
        $ ./client |& grep '校验结果'
            20: the issuer certificate could not be found: 
                this occurs if the issuer certificate of an untrusted certificate cannot be found
        $ ./client ./root_inspur.pem |& grep '校验结果'
            26: the supplied certificate cannot be used for the specified purpose.
                证书的用途不对.
    c,通过 man verify 看20，26的含义.


3,字签名证书:
    server:
        ./service pub_1 priv_1
    client:
        [pangwz@rowanInspur unix-ssl]$ ./client ./pub_1 |& grep '校验结果'
            校验结果: 0     -->自签名证书，而且作为信任导入,证书中又不含purpose信息.所以校验通过.
        [pangwz@rowanInspur unix-ssl]$ ./client  |& grep '校验结果'
            校验结果: 18 -->提示伟子签名证书，并且没有作为信任导入.
        [pangwz@rowanInspur unix-ssl]$ 


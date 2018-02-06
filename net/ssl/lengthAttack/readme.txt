1,basic
    let secret = "secret"
    let data = "data"
    let H = md5()
    let signature = hash(secret || data) = 6036708eba0d11f6ef52ad44e8b74d5b
    let append = "....append..."

    a,如果我们知道 H(secret||data) 的值,则在
	不知到secret的情况下,我们也可以计算H(secret||data||append)的值.
    b,'||' 表示concatenate.

2,ref
    ../md5/

3,netinfo
    https://blog.skullsecurity.org/2012/everything-you-need-to-know-about-hash-length-extension-attacks

4,tools
    https://github.com/iagox86/hash_extender/blob/master/hash_extender_engine.c

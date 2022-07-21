0,basic
    a,infgen.c
        https://www.zlib.net/
        https://github.com/madler/infgen/

1,gzip
    a,cmd
        $ echo -n 'rowan pangwz' | gzip -c > vs.txt.gz
        $ xxd  -C -g1 vs.txt.gz
            00000000: 1f 8b 08 00 00 00 00 00 00 03 2b ca 2f 4f cc 53  ..........+./O.S
            00000010: 28 48 cc 4b 2f af 02 00 53 42 66 cc 0c 00 00 00  (H.K/...SBf.....
        $ xxd -C -g1 -b vs.txt.gz
            00000000: 00011111 10001011 00001000 00000000 00000000 00000000  ......
            00000006: 00000000 00000000 00000000 00000011 00101011 11001010  ....+.
            0000000c: 00101111 01001111 11001100 01010011 00101000 01001000  /O.S(H
            00000012: 11001100 01001011 00101111 10101111 00000010 00000000  .K/...
            00000018: 01010011 01000010 01100110 11001100 00001100 00000000  SBf...
            0000001e: 00000000 00000000
    b,fmt parser
            1f 8b: fix IDentification
               08: cmpression method; 8=deflate
               xx:
     2b ca..02 00: Deflate数据
      53 42 66 cc: CRC32
      0c 00 00 00: 最后的0c 00 00 00,表示input size.  multi-byte least-significant;


2,zlib
    a,如下是compress()的输出
        $ ./simple "rowan pangwz"
        compress finish,ret :0,inlen:12,outlen:20
            78 9c 2b ca 2f 4f cc 53 28 48 cc 4b 2f af 02 00 	.....O.S.H.K....
            1f 62 04 df

    b,fmt parser
            78: CMF 字段
                bits 0 to 3  CM     Compression method
                bits 4 to 7  CINFO  Compression info, the base-2 logarithm of the LZ77 window size, minus eight.(=log₂winSize - 8); 下标 :h dig

                8: CM,Compression method; 8=Deflate
                7: CINFO,计算winsize=2^(7+8)=32k, 也即zlib 的default win size.
                    #  define DEF_WBITS MAX_WBITS
                    #  define MAX_WBITS   15        /* 32K LZ77 window */

            9c: FLG
                bits 0 to 4  FCHECK  (check bits for CMF and FLG)
                    FCHECK: header += 31 - (header % 31);       ref: deflate(xx),if (s->status == INIT_STATE) {xx}
                bit  5       FDICT   (preset dictionary)
                bits 6 to 7  FLEVEL  (compression level)

  2b ca..02 00: Deflate数据
   1f 62 04 df: ADLER32

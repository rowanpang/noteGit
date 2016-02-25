1,files
	rowan.prikey.pem
	rowan.x509.der
	rowan.x509.der.txt
	rowan.x509.der.txt.parse
	rowan.x509.der.txt.tbs
	rowan.x509.der.txt.tbs.bin
	rowan.x509.der.txt.tbs.bin.sha1.txt
	rowan.x509.der.txt.tbs.bin.sha1.txt.bin
	rowan.x509.pem
	rowan.x509.pem.sign.bin
	rowan.x509.pem.sign.bin.verify
	rowan.x509.pem.sign.txt
	rowan.x509.text

2,base
	gen key 
		a,gen private key 
			rowan.prikey.pem
		b,gen PEM certificate
			rowan.x509.pem
		  der format
			rowan.x509.der
	txt parser
		rowan.x509.txt

4,signature
	反向verify，得到signature中的sha1sum value
	got:
		rowan.x509.pem.sign.txt			:signature txt from rowan.x509.txt 
		change to bin
			rowan.x509.pem.sign.bin
			~/text/text2bin/a.out rowan.x509.pem.sign.txt -o rowan.xxxx.sign.bin
	verify:
		openssl rsautl -in rowan.x509.pem.sign.bin -inkey rowan.x509.pem -certin -verify |hexdump -C > rowan.x509.pem.sign.bin.verify	

	sha1sum value:
		ec ec dc 1e 1e a9 f6 df d0 60 6f 8e 70 f2 b8 4d ab 63 bf 05 //sha1sum value

5,check
	正向计算出tbsCertificate 的sha1sum value
	got: tbsCertificate	
		a,rowan.x509.pem change format rowan.x509.der
		b,vim xx.der xxd -g 1 to text format rowan.x509.der.txt
		c,delete data no need got rowan.x509.der.txt.tbs
		d,compare can got delete what
	
		e,change to bin
			same to up. got rowan.x509.der.txt.tbs.bin	
		f,got sha1sum value
			sha1sum rowan.x509.der.txt.tbs.bin | awk '{print $1}' > rowan.x509.der.txt.tbs.bin.sha1.txt	
			ececdc1e1ea9f6dfd0606f8e70f2b84dab63bf05	
	compare sha1sum:
		比较正向/反向得到的sha1sum value是否相等
		has the same sha1sum value,so verify ok;	

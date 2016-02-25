1,
	rowan.prikey.pem
	rowan.x509.der
	rowan.x509.der.txt
	rowan.x509.der.txt.all
	rowan.x509.der.txt.bin
	rowan.x509.der.txt.bin.sha1.txt
	rowan.x509.der.txt.bin.sha1.txt.bin
	rowan.x509.pem
	rowan.x509.pem.signature.bin
	rowan.x509.pem.signature.txt			:signature txt from rowan.x509.txt 
	rowan.x509.txt

2,gen 
	a,gen private key 
		rowan.prikey.pem
	b,gen PEM certificate
		rowan.x509.pem
	  der format
		rowan.x509.der
3,txt parser
		rowan.x509.txt
4,got signature
		rowan.x509.pem.signature.txt			:signature txt from rowan.x509.txt 
	change to bin
		rowan.x509.pem.signature.bin


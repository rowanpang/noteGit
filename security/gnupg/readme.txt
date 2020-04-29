sign and encrypt
	gpg -u rowan2 -r rowan -se readme.txt

dencrypt and verify
	gpg -d readme.txt.gpg

gpg2 --recipient torvalds@kernel.org --encrypt --output readme.txt.gpg.trovalds  readme.txt
gpg2 --recipient pangweizhen.2008@qq.com --encrypt --output readme.txt.gpg.qq  readme.txt

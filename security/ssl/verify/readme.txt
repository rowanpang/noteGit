1,
	[pangwz@rowanInspur inspur]$ openssl verify -CApath ./ root_inspur.pem 
		root_inspur.pem: OK
	[pangwz@rowanInspur inspur]$ openssl verify root_inspur.pem
		root_inspur.pem: DC = com, DC = langchao, DC = home, CN = LangChao-CA
		error 18 at 0 depth lookup:self signed certificate
		OK----->表示 operation was successful.
	[pangwz@rowanInspur inspur]$
	 

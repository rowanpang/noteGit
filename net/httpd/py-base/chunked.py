#!/usr/bin/python

import BaseHTTPServer
import binascii
import time

HOST_NAME = ''
PORT_NUMBER = 9999

class myHandler(BaseHTTPServer.BaseHTTPRequestHandler):

    def my404(self):
        self.send_response(404)
        self.send_header("Content-type","text/html")
        self.end_headers()

        self.wfile.write(open("./404.html","r").read())

    def do_GET(self):
        """Respond to a GET request."""
        if self.path == '/404':
            # self.send_error(404,"not found")
            # self.send_error(404)
            self.my404()
            return
        self.send_response(200)
        self.send_header("Content-type", "application/ATFX")
        self.send_header("Content-disposition", '''attatchment;
        filename="rowanFileName.tmp"''')
        self.send_header("Transfer-Encoding", "chunked")
        self.end_headers()

        # for i in range(1,5):
        self.wfile.write(hex(1024*64*3)[2:]+"\r\n")     #chunk1 size+0d0a 192k
        self.wfile.write(open("/dev/zero","r").read(1024*64*3)+"\r\n")
        self.wfile.write(hex(1024*4)[2:]+"\r\n")        #chunk2             4k
        self.wfile.write(open("/dev/zero","r").read(1024*4)+"\r\n")
        self.wfile.write("0"+2*"\r\n")
        # self.wfile.write("<html><head><title>Title goes here.</title></head>")
        # self.wfile.write("<body><p>This is a test.</p>")
        # self.wfile.write("<p>You accessed path: %s</p>" % self.path)
        # self.wfile.write("</body></html>")

if __name__ == '__main__':
    server_class = BaseHTTPServer.HTTPServer
    httpd = server_class((HOST_NAME, PORT_NUMBER), myHandler)
    print time.asctime(), "Server Starts - %s:%s" % (HOST_NAME, PORT_NUMBER)
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        pass
    httpd.server_close()
    print time.asctime(), "Server Stops - %s:%s" % (HOST_NAME, PORT_NUMBER)


#!/usr/bin/python
import mechanize
from BeautifulSoup import BeautifulSoup
import argparse
import urlobject
import configobj
import re

def urlhostname(url):
    # print url
    uobj = urlobject.URLObject(url)
    return uobj.hostname

def crawer(url):
    hnames= []
    print 'crawer:',url
    hdr = {'User-Agent'      : 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.11 \
                                (KHTML, like Gecko) Chrome/23.0.1271.64 Safari/537.11',
           'Accept'          : 'text/html,application/xhtml+xml,application/xml;\
                                q=0.9,*/*;q=0.8',
           'Accept-Charset'  : 'ISO-8859-1,utf-8;q=0.7,*;q=0.3',
           'Accept-Encoding' : 'none',
           'Accept-Language' : 'en-US,en;q=0.8',
           'Connection'      : 'keep-alive'}

    br = mechanize.Browser()
    br.set_handle_referer(True)
    br.set_handle_redirect(True)
    br.set_handle_robots(False)
    br.addheaders = hdr.items()

    try:
        page = br.open(url)
        html = page.read()
        soup = BeautifulSoup(html)
        tags = {'a':True,'link':True}
        hrefs = soup.findAll(name=tags)
        for ref in hrefs:
            if ref.has_key('href') and re.search(':?\/\/',ref['href']):
                href = ref['href']
                if href.startswith('//'):
                    url = 'http:' + href
                elif href.startswith('http'):
                    url = href
                else:
                    raise Exception('url error')
                name = urlhostname(url)
                if name not in hnames:
                    hnames.append(name)
    except Exception as e:
        print e

    hnames.sort
    return hnames

def main():
    cfg = configobj.ConfigObj('./url.txt')
    for sec in cfg.sections:
        secrefs = []
        sectops = []
        f = open('./domain-%s.txt' %(sec),'w')
        body = cfg[sec]
        k = 'entry'
        if k in body.keys():
            for url in body.as_list(k):
                refs = crawer(url)
                for r in refs:
                    if r not in secrefs:
                        secrefs.append(r)

        for n in secrefs:
            dnsl1 = n[n.rfind('.',0,n.rfind('.')-1)+1:]
            if dnsl1 not in sectops:
                sectops.append(dnsl1)
            f.write(n)
            f.write('\n')
        print sectops

        f.close()

if __name__ == '__main__':
    main()

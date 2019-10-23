from urllib import parse
import requests
from bs4 import BeautifulSoup
import xml.dom.minidom as Dom
import json
import os
import shutil

def spider(sstr):
    print("传入的参数为" + sstr)
    str_origin = sstr
    search_word = str_origin
    url = "http://www.imomoe.io/search.asp"

    data = {"searchword":search_word}

    str_encode = parse.quote(str_origin,encoding='gb2312')
    data  = parse.urlencode(data,encoding='gb2312').encode()

    headers = {
         "Accept":"text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8",
         "Accept-Encoding":"gzip, deflate",
         "Accept-Language":"zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2",
         "Connection":"keep-alive",
          "Content-Length":"35",
          "Content-Type":"application/x-www-form-urlencoded",
          "Content-Length":"application/x-www-form-urlencoded",
          "Cookie":"UM_distinctid=16d7addb9d2124-07436dbc9991d98-4c312373-1fa400-16d7addb9d383; CNZZDATA1260742008=1075644271-1569722879-https%253A%252F%252Fwww.baidu.com%252F%7C1571661014; bdshare_firstime=1569724422132; Hm_lvt_38c112aee0c8dc4d8d4127bb172cc197=1570065855,1570241335,1571574330,1571663849; qike123=%u9B3C%u706D%u4E4B%u5203%20%u7B2C26%u96C6^http%3A//www.imomoe.io/player/7498-0-25.html_$_%u65B0%u77F3%u7EAA%20%u7B2C06%u96C6^http%3A//www.imomoe.io/player/7664-0-5.html_$_%u65B0%u77F3%u7EAA%20%u7B2C08%u96C6^http%3A//www.imomoe.io/player/7664-0-7.html_$_%u65B0%u77F3%u7EAA%20%u7B2C09%u96C6^http%3A//www.imomoe.io/player/7664-0-8.html_$_%u65B0%u77F3%u7EAA%20%u7B2C10%u96C6^http%3A//www.imomoe.io/player/7664-0-9.html_$_%u65B0%u77F3%u7EAA%20%u7B2C11%u96C6^http%3A//www.imomoe.io/player/7664-0-10.html_$_%u65B0%u77F3%u7EAA%20%u7B2C12%u96C6^http%3A//www.imomoe.io/player/7664-0-11.html_$_%u65B0%u77F3%u7EAA%20%u7B2C13%u96C6^http%3A//www.imomoe.io/player/7664-0-12.html_$_|; security_session_verify=c3ea8a1089ec6d96c2ef78d9b6a88616; Hm_lpvt_38c112aee0c8dc4d8d4127bb172cc197=1571666369; __music_index__=2; ASPSESSIONIDQQDSRQCA=PBOAJHLCOMJMEPOIBMEDDNHC; first_h=1571666000865; count_h=6; first_m=1571666373766; count_m=1",
          "host":"www.imomoe.io",
          "Referer":"http://www.imomoe.io/search.asp",
          "Upgrade-Insecure-Requests":"1",
          "user-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:69.0) Gecko/20100101 Firefox/69.0"
    }

    result = requests.post(url,data=data,headers=headers)
    result.encoding="GBK"

    soup = BeautifulSoup(result.text,'lxml')
    bangumiInfos = soup.find(name='div',attrs={'class':'pics'})

    informations = []
    bangumiInfoli = bangumiInfos.findAll(name='li')
    if (bangumiInfoli == []):
        print("不存在")
    for bangumiInfo in bangumiInfoli:
        if search_word in str(bangumiInfo):
            try:
                url = "http://www.imomoe.io" + bangumiInfo.a['href']
                img = bangumiInfo.a.img['src']
                title = bangumiInfo.h2.a.string
                span = bangumiInfo.findAll(name='span')
                sub_title = span[0]
                type = span[1]
                brief_introduction = bangumiInfo.p.string
                information = {}
                information['title'] = title
                information['sub_title'] = sub_title.string
                information['type'] = type.string
                information['url'] = url
                information['img'] = img
                information['brief_introduction'] = brief_introduction
                informations.append(information)
            except AttributeError as e:
                print("不存在")
        else:
            print("不存在")
    if informations != []:
        if os.path.exists("./title_images"):
            shutil.rmtree("./title_images")
        write(informations)


def write(informations):
    doc = Dom.Document()
    root_node = doc.createElement("bangumi_Infomations")
    doc.appendChild(root_node)
    for information in informations:
        information_node = doc.createElement("information")
        for key in information.keys():
            key_node = doc.createElement(key)
            text_node = doc.createTextNode(information[key])
            key_node.appendChild(text_node)
            information_node.appendChild(key_node)
            if (key == 'img'):
                print("开始")
                writeImage(information[key])
        root_node.appendChild(information_node)
    if not os.path.exists('./xmlfiles'):
        os.mkdir("./xmlfiles")
    with open('./xmlfiles/informations.xml', 'wb') as f:
        f.write(doc.toprettyxml(indent='\t', newl='\n', encoding='utf-8'))

def writeImage(img_url):
    print(img_url)
    response = requests.get(img_url)
    img_content = response.content
    split_img = img_url.split('/')
    img_name = split_img[len(split_img) - 1]
    if not os.path.exists("./title_images"):
        os.mkdir("./title_images")
    with open("./title_images" +'/' + img_name,"wb") as f:
        f.write(img_content)


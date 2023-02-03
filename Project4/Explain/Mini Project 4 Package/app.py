# -*- coding: UTF-8 -*-
from flask import Flask, request, abort
import DAN,csmapi, random, time, threading
from linebot import LineBotApi, WebhookHandler
from linebot.exceptions import InvalidSignatureError
from linebot.models import *

app = Flask(__name__)

#---------------------------------IoTtalk functions---------------------------------
def IoTtalk_registration():
    # set IoTtalk Server URL
    IoTtalk_ServerURL = 'http://140.114.77.93:9999/'
    
    # set device profile
    DAN.profile['dm_name'] = '111065510Model'
    DAN.profile['df_list'] = ['111065510_input', '111065510_output']
    
    # register device profile to IoTtalk Server
    DAN.device_registration_with_retry(IoTtalk_ServerURL, None)

def IoTtalk_push_and_pull(IDF, ODF, data):
    DAN.push(IDF, data)
    time.sleep(1.5)
    result = DAN.pull(ODF)
    return result
#---------------------------------IoTtalk functions---------------------------------
    
#---------------------------------LineBot API functions---------------------------------
# Channel Access Token
line_bot_api = LineBotApi('nbkCqvTKNqJC34Ioz8sNS4N1Ow0jtzCIEeJicvruRD/y7HeYj8V6QmmvlNP+fm5e0HKwGk7wHWKBRrVqJBkr64xfsjvY+5oIqbGnTQmfOd5UI2NLnXNPFZiv4IG01kEWqMPHDPhS2RlDK10P2qTnaAdB04t89/1O/w1cDnyilFU=')
# Channel Secret
handler = WebhookHandler('2271a2cc2239ac8f1a472248bf7c2159')

# 監聽所有來自 /callback 的 Post Request
@app.route("/callback", methods=['POST'])
def callback():
    # get X-Line-Signature header value
    signature = request.headers['X-Line-Signature']
    # get request body as text
    body = request.get_data(as_text=True)
    app.logger.info("Request body: " + body)
    # handle webhook body
    try:
        handler.handle(body, signature)
    except InvalidSignatureError:
        abort(400)
    return 'OK'

@handler.add(MessageEvent, message=TextMessage)
def handle_message(event):
    
    #get message sent by line bot user
    text = event.message.text
    print(text)
    
    #============================================IMPLEMENT YOUR SCENARIO=====================================#

    #push and pull data through IoTtalk server
    result = IoTtalk_push_and_pull("111065510_input", "111065510_output", text)
    print(result)
    
    # write some codes here to handle the message 
    message = TextSendMessage(result[0])
    
    #replay message to line bot user
    line_bot_api.reply_message(event.reply_token, message)
    
    #========================================================================================================#
    
#---------------------------------LineBot API functions---------------------------------

import os
if __name__ == "__main__":
         
    IoTtalk_registration()
     
    port = int(os.environ.get('PORT', 5002))
    app.run(host='0.0.0.0', port=port)
    

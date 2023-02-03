# -*- coding: UTF-8 -*-
from flask import Flask, request, abort
from linebot import LineBotApi, WebhookHandler
from linebot.exceptions import InvalidSignatureError
from linebot.models import *

app = Flask(__name__)

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
	message = TextSendMessage(text=event.message.text)
	line_bot_api.reply_message(event.reply_token, message)

import os
if __name__ == "__main__":
	port = int(os.environ.get('PORT', 5002))
	app.run(host='0.0.0.0', port=port)
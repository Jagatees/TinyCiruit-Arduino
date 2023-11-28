import telebot
from telebot.types import InlineKeyboardButton,InlineKeyboardMarkup,InputMediaAnimation
import paho.mqtt.client as mqtt
import application
import apiRequests
import threading




#mqtt settings 
MQTT_BROKER = "127.0.0.1"
#MQTT_BROKER  = "172.20.10.2"
#MQTT_BROKER = "192.168.76.192"
#MQTT_BROKER  = "172.20.10.2"
MQTT_PORT = 1883
MQTT_TOPIC = ['tele/Request','tele/pmRequest']
MQTT_TOPICRESPONSE = ['tele/Response']

received_message = None
receivedmessagetopic = None


def on_message(client, userdata, message):
    global received_message
    global receivedmessagetopic
    print('receiving something')
    received_message = message.payload.decode()
    receivedmessagetopic = message.topic


client = mqtt.Client()
client.on_message = on_message
zfchatId='1833363187'
valid = True 
userid = None

try:
    bot = telebot.TeleBot('6825211018:AAEXa7vvIw156tWfWRKAsXCUQLQky3UCI64')
except:
    pass

#send a text to me 

def send_text(message):
    try:
        bot.send_message(zfchatId, message)
    except:
        pass    

#texthandler 
@bot.message_handler(content_types=['text'])
def text_handler(message):
    global valid
    global userid
    
    print(message.text)
    print(message.chat.id)
    print(message.chat.first_name)
    sendername = message.chat.first_name
    senderid = message.chat.id
    #send_text(message.text)
    #bot.reply_to(message, message.text) 
    #reply if the message is not from zfchatid, assuming zf is the prof


    #check if the messsage has the word announcement: in it , if yes, do this flow 
    if 'Announcement:' in message.text:
        if str(message.chat.id) != zfchatId:
            bot.reply_to(message, 'only professors/zf are authenicated to publish announcements')
        else:
            bot.reply_to(message, 'identicated, publishing announcement')
            #get the message after the word announcement: 
            # try:
            #     announcement = message.text.split('announcement:')[1]
            # except:
            announcement = message.text.split('Announcement:')[1]
            #publish the message to mqtt
            client.connect(MQTT_BROKER, MQTT_PORT, 60)
            client.publish('tele/Announcement', announcement)
            #reply to the user 
            bot.reply_to(message, 'announcement sent')
            valid = True
    else:
        # it is for a specific person, look for the name and it should end with :, and retrieve the teleid of the person
        if ':' in message.text:
            #get the name 
            name = message.text.split(':')[0]
            #get the message 
            intendedmessage = message.text.split(':')[1]
            #publish the message to mqtt
            client.connect(MQTT_BROKER, MQTT_PORT, 60)
            client.publish(f'tele/{name}', f'{intendedmessage} by {sendername}')
            #reply to the user 
            bot.reply_to(message, 'message sent to ' + name)

            userid = message.chat.id
            valid = True
        else:
            print('not a valid message')
            bot.reply_to(message, 'not a valid message. If you are intending to do a announcment, please start with announcement: \nif you are intending to send a message to a specific person, please start with name:')
            valid = False

    if valid:
        response = ''   
        #trigger openai to generate a response and publsih those responses to mqtt
        responses = apiRequests.askgpt(message.text)
        for i in range(0,len(responses)):
            # combine them into 1 string split by commas 
            response += responses[i] + ','
        #publish 
        client.publish('tele/SuggestedResponse', response)


#a thread to listen to mqtt, when receive something, send a text to me through the telebot

def subscribeListening():
    global received_message
    global receivedmessagetopic
    global userid

    print('listening')
    client1 = mqtt.Client()
    client1.on_message = on_message

    client1.connect(MQTT_BROKER, MQTT_PORT, 60)
    for topic in MQTT_TOPIC:
        client1.subscribe(topic)

        client1.loop_start()  # Start the loop


    while received_message is None:
        pass
    #print out the message when received and end the function 
    print('things received')
    print(received_message)
    print(receivedmessagetopic)

    if receivedmessagetopic == 'tele/Request':
        send_text(received_message)

    elif receivedmessagetopic == 'tele/pmRequest':
        if userid is not None:
            bot.send_message(userid, received_message)
        else:  
            # if user id is not found, send to zfchatid
            bot.send_message(zfchatId, f'no userid found, message is {received_message}')

    for topic in MQTT_TOPIC:
        client1.unsubscribe(topic)
        client1.loop_stop()

    client1.disconnect()
    #rerun the function 
    received_message = None
    receivedmessagetopic = None

    subscribeListening()




threading.Thread(target=subscribeListening).start()



#publish a false to tele
client.connect(MQTT_BROKER, MQTT_PORT, 60) 
client.publish('tele', 'True')
client.disconnect() 
bot.polling()
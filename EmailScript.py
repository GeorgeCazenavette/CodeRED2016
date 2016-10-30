# Code is strictly for the GUI
import RPi.GPIO as GPIO
#from Tkinter import *
import time
import random, csv
import threading
#from twilio.rest import TwilioRestClient
import smtplib

FIREPIN = 21 
EARTHQUAKEPIN = 20


GPIO.setmode(GPIO.BCM)
GPIO.setup(FIREPIN, GPIO.IN, pull_up_down = GPIO.PUD_DOWN)
GPIO.setup(EARTHQUAKEPIN, GPIO.IN, pull_up_down = GPIO.PUD_DOWN)


def sendSMS(message):
   # number to send the code to 
   number = '9722682745'
   # message to send the user
   msg = message
   
   # setup the connection between the client and the smtp server  
   # instantate an object to interact wih the ser
   server = smtplib.SMTP('smtp.gmail.com', 587)
   # use tls encryption
   server.starttls()
   # login into the server
   server.login("HCSSCRChallenge@gmail.com", "codered2016")
   # list of phone carriers to iterate over
   listOfCarriers = ['sms.alltelwireless.com',
                  'txt.att.net', 
                  'sms.myboostmobile.com',
                  'mms.cricketwireless.net',
                  'msg.fi.google.com',
                  'text.republicwireless.com',
                  'messaging.sprintpcs.com',
                  'tmomail.net',
                  'email.uscc.net',
                  'vtext.com',
                  'vmobi.com',
                  ]
                  
   # send a message to each potential carrier
   for carrier in listOfCarriers:
      # try to send 
      try:
         server.sendmail('HCSSCRChallenge@gmail.com', number+'@'+carrier, msg)
      # if fails, reset the connection
      except:
         server = smtplib.SMTP('smtp.gmail.com', 587)
         server.starttls()
         server.login("HCSSCRChallenge@gmail.com", "codered2016")
         server.sendmail('HCSSCRChallenge@gmail.com', number+'@'+carrier, msg)
   
   server.quit()
    



#sendSMS()

fireMessageSent = False
quakeMessageSent = False

# TO DO //
timeFireSent = time.time()
timeQuakeSent = time.time()
while (True):
	#print GPIO.input(FIREPIN)
	if (GPIO.input(FIREPIN) and fireMessageSent == False):
		message = "Fire Detected! Call 911"
		sendSMS(message)
		fireMessageSent = True;
		timeFireSent = time.time()
	
	if (time.time()-timeFireSent >= 60.0 and fireMessageSent == True):
		fireMessageSent = False
	
	if (GPIO.input(EARTHQUAKEPIN) and quakeMessageSent == False):
		message = "Quake Detected in your area"
		sendSMS(message)
		fireMessageSent = True;
		timeQuakeSent = time.time()
		
	if(time.time()-timeQuakeSent >= 60.0 and quakeMessageSent== True):
		quakeMessageSent = False

# Code is strictly for the GUI
import RPi.GPIO as GPIO
#from Tkinter import *
from time import time, sleep, strftime, localtime
import random, csv
import threading
#from twilio.rest import TwilioRestClient
import smtplib

FIREPIN = 21 

GPIO.setmode(GPIO.BCM)
GPIO.setup(FIREPIN, GPIO.out)


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

# TO DO //
timeFireSent = time.time()
while (True)
	if (GPIO.input(FIREPIN) && fireMessageSent == False):
		message = "Fire Detected! Call 911"
		sendSMS(message)
		fireMessageSent = True;
		timeFireSent = time.time()
	
	if (time.time()-timeFireSent >= 60.0 && fireMessageSent == True):
		fireMessageSent = False
		
	
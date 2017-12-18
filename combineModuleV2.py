import RPi.GPIO as GPIO
import time


GPIO.setmode(GPIO.BCM)

PIN=18
GPIO_SIG = 17
LIGHT1_GPIO=25
SAMPLE_VALUE=10  #number of senser reading to be taken to decide the final distance of the object (person), if any
ROOM_LENGTH=315  #Actual Size is 319 however we need to consider 1% deviation #in CM
ALLOWED_SENSOR_DATA_ERROR_DEVIATION=0.2 #Allowing 20% error deviation 
POST_INACTIVITY_DELAY=1 #In Sec - how much time we want to wait after confirming that room is empty

def measurementPulse(start, stop):

    # Calculate pulse length
    elapsed = stop-start

    # Distance pulse travelled in that time is time
    # multiplied by the speed of sound (cm/s)
    distance = elapsed * 34300

    # That was the distance there and back so halve the value
    distance = distance / 2

    print "Distance captured : %.1f CM approximately" % distance
    return distance

def measurementInCM():

    # setup the GPIO_SIG as output
    GPIO.setup(GPIO_SIG, GPIO.OUT)

    GPIO.output(GPIO_SIG, GPIO.LOW)
    time.sleep(0.2)
    GPIO.output(GPIO_SIG, GPIO.HIGH)
    time.sleep(0.5)
    GPIO.output(GPIO_SIG, GPIO.LOW)
    start = time.time()

    # setup GPIO_SIG as input
    GPIO.setup(GPIO_SIG, GPIO.IN)

    # get duration from Ultrasonic SIG pin
    #start = time.time()
    while GPIO.input(GPIO_SIG) == 0:
        time.time()
    
    while GPIO.input(GPIO_SIG) == 1:
        stop = time.time()
    #stop = time.time()
    return measurementPulse(start, stop)


def ultrasonicModule():
    while True:
        distance=0
        CNT_LESS_DISTANCE=0
        for i in range(SAMPLE_VALUE):
            distance = measurementInCM()
            if distance < ROOM_LENGTH: 
                CNT_LESS_DISTANCE= CNT_LESS_DISTANCE + 1
        #Now if number of sample is lesser than the allowed deviation then it means that 'no one is there in the room'
        #i.e. light should turn off, hence break the loop and continue waiting for a movement
        if CNT_LESS_DISTANCE <= ALLOWED_SENSOR_DATA_ERROR_DEVIATION * SAMPLE_VALUE:
            break
        print "Active Scenario - Yes someone is in the room" 
 
if __name__ == '__main__':
    try:
        GPIO.setup(PIN,GPIO.IN)
        #GPIO.setup(LIGHT1_GPIO,GPIO.OUT)
        while True:
            if GPIO.input(PIN):
                print "Active Scenario - Motion Detected - I think someone just entered the room!!"
                #when motion is detected start the ultrasonic module
                #GPIO.output(LIGHT1_GPIO,GPIO.LOW)
                ultrasonicModule()
                time.sleep(POST_INACTIVITY_DELAY)
                #GPIO.output(LIGHT1_GPIO,GPIO.HIGH)
                print "Inactive Scenario - No one is in the room !!"
            time.sleep(0.1)
    except:
        GPIO.cleanup()  
    
    


  
        


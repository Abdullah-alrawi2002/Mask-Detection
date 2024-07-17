import cv2
import serial
import webbrowser
import pyttsx3

engine = pyttsx3.init('sapi5')
voices = engine.getProperty('voices')
engine.setProperty('voice', voices[1].id)

def say(audio):
    engine.say(audio) 
    engine.runAndWait()

serial_port = serial.Serial('COM8', 115200, timeout=0.1)

face_cascade = cv2.CascadeClassifier('/Users/HP/Desktop/haarcascade_frontalface_default.xml')
mouth_cascade = cv2.CascadeClassifier('/Users/HP/Desktop/Mouth.xml')

cap = cv2.VideoCapture(1)

def detect_faces_and_mouths():
    ret, img = cap.read()
    img = cv2.flip(img, 1)
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    (thresh, black_and_white) = cv2.threshold(gray, 80, 255, cv2.THRESH_BINARY)
    faces = face_cascade.detectMultiScale(gray, 1.1, 4)
    faces_bw = face_cascade.detectMultiScale(black_and_white, 1.1, 4)
    for (x, y, w, h) in faces:
        cv2.rectangle(img, (x, y), (x + w, y + h), (0, 0, 255), 2)
        roi_gray = gray[y:y + h, x:x + w]
        roi_color = img[y:y + h, x:x + w]
        mouth_rects = mouth_cascade.detectMultiScale(gray, 1.5, 5)
        if(len(mouth_rects) == 0):
            print("MASK DETECTED")
        else:
            for (mx, my, mw, mh) in mouth_rects:
                if(y < my < y + h):
                    print("NO MASK DETECTED")
                    say("Sir, please wear a mask")
                    webbrowser.open('http://192.168.43.212/M')
                    break

while True:
    data = serial_port.readline().strip()
    if data == b'H':
        detect_faces_and_mouths()
    elif data == b'D':
        say("Sir, please sanitize your hands")
    elif data == b'S':
        webbrowser.open('http://192.168.43.212/P')
    else:
        print("YOU ARE GREAT AJ")

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()

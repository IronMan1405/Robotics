import cv2

faceCascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')


vid = cv2.VideoCapture(0)

while True:
    check, frame = vid.read()

    if not check:
        break

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    faces = faceCascade.detectMultiScale(gray, scaleFactor = 1.05, minNeighbors = 6, minSize = (30,30))

    for (x,y, w, h) in faces:
        cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 3)

    cv2.putText(frame, f"faces: {len(faces)}", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (2555, 255, 255), 6)
    cv2.putText(frame, f"faces: {len(faces)}", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (2555, 255, 255), 2)

    cv2.imshow('Real Time Face Detection', frame)

    if cv2.waitKey(1) & 0xFF == 'q':
        break

vid.release()
cv2.destroyAllWindows()

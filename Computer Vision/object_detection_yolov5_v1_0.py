import torch
import cv2

model = torch.hub.load('ultralytics/yolov5', 'yolov5m', pretrained = True)
model.conf = 0.6 #confidence threshold

print(f"\n\n{model.names}\n\n")

vid = cv2.VideoCapture(0)

while vid.isOpened():
    ret, frame = vid.read()

    if not ret:
        break

    results = model(frame, size = 208)
    labels, coords = results.xyxyn[0][:, -1], results.xyxyn[0][:, :-1]
    n = len(labels)

    h, w, _ = frame.shape

    for i in range(n):
        row = coords[i]

        if row[4] >= 0.5:
            x1, y1, x2, y2 = int(row[0] * w), int(row[1] * h), int(row[2] * w), int(row[3] * h)

            classID = int(labels[i])
            className = model.names[classID]
            color = (0, 255, 0)
            cv2.rectangle(frame, (x1, y1), (x2, y2), color, 4)
            cv2.putText(frame, className, (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.9, color, 4)

    cv2.imshow("YOLOv5-Tiny Object Detection", frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

vid.release()
cv2.destroyAllWindows()
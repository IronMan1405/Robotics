import cv2
import numpy as np
import os

labels_path = os.path.expanduser('~/mobilenet_ssd/coco_labels.txt')
with open(labels_path, 'r') as f:
    classes = [line.strip() for line in f.readlines()]

model_path = os.path.expanduser('~/mobilenet_ssd/ssd_mobilenet_v1_coco_11_06_2017/orzen_inference_graph.pb')
net = cv2.dnn.readNetFromTensorFlow(model_path)

cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()
    if not ret:
        break

    h, w = frame.shape[:2]

    blob = cv2.dnn.blobFromImage(cv2.resize(frame, (300, 300)), 1.0 / 127.5, (300, 300), (127.5, 127.5, 127.5), swapRB=True)

    net.setInput(blob)
    detections = net.forward()

    for i in range(detections.shape[2]):
        conf = detections[0, 0, i, 2]
        
        if conf > 0.5:
            class_id = int(detections[0, 0, i, 2])

            if class_id < len(classes):
                label = classes[class_id]

                if label == 'NA':
                    continue

                box = detections[0, 0, i, 3:7] * np.array([w, h, w, h])
                (x1, y1, x2, y2) = box.astype(int)

                print(f"detected {label} with {conf:.2f}")
                
                cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
                cv2.putText(frame, f"{label}: {conf:.2f}", (x1, y1-10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 0), 2)

    cv2.imshow("MobileNet SSD Object Detection", frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
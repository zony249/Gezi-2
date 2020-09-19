import sys
import numpy as np

import torch
import cv2


np.set_printoptions(threshold=sys.maxsize)

model = cv2.dnn.readNetFromDarknet('yolov3-spp.cfg','gezi-detector.weights')

im = cv2.imread("images/99032.jpg")
blob = cv2.dnn.blobFromImage(im, 1, (416, 416), swapRB=True, crop=False)
model.setInput(blob)
prediction = model.forward()

boxes = []
confidences = []
classes = []


for cell in prediction:
	class_scores = cell[5:]
	class_ID = np.argmax(class_scores)
	confidence = class_scores[class_ID]

	if confidence > 0.5:
		bbox = cell[0:4] * np.array([416, 416, 416, 416])
		(x, y, width, height)= bbox.astype("int")
		boxes.append([x, y, width, height])
		confidences.append(confidence)
		classes.append(class_ID)
		print(bbox)
		print(confidence)
		print(class_scores)
if len(confidences) > 0:
	postNMS = cv2.dnn.NMSBoxes(boxes, confidences, 0.5, 0.3)

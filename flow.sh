#!/bin/bash

exit 0

#######
# 1st time install.

source install.sh

#######
# On every unpack.
source after_unpacking.sh

#######
# Labeling.

labelImg datasets/train/images/ predefined_classes.txt datasets/train/labels/
labelImg datasets/valid/images/ predefined_classes.txt datasets/valid/labels/

#######
# Augmentation.

./ImageDSP/build/linux/build-ImageDSP-Desktop-Debug/ImageDSP \
	--prog-name "Augmentation 1" \
	--in-img "datasets/train/images/IMG001.jpg" \
	--out-img "datasets/train/images/IMG001_a1_threshold=150_compress=40.jpg" \
	--params "150;40"

#######
# Train.

mkdir weights/

# TODO Time.
time yolo task=detect mode=train \
	data=data.yaml imgsz=640 \
	project=runs/train name='exp' \
	epochs=10 batch=8 optimizer='Adam' \
	model=std_weights/yolov8s.pt \
	device=cpu
LAST_EXP=`ls -w1 runs/train/ | tail -n 1`
cp runs/train/$LAST_EXP/weights/best.pt weights/fruits_s.pt
#TODO results.csv as image
# -H visualize

#########
# Detect.

# On usual weights.
yolo detect predict \
	save=True project=runs/predict \
	name='yolov8n' \
	model=std_weights/yolov8n.pt \
	source=datasets/valid/images/IMG003.jpg

# On our weights.
yolo detect predict \
	save=True project=runs/predict \
	name='fruits_s' \
	model=weights/fruits_s.pt \
	source=datasets/valid/images/IMG003.jpg

#########
# Track.

#TODO Need newer version of ultralytics.
#TODO https://docs.ultralytics.com/modes/track/

# On usual weights.
#TODO weights=std_weights/yolov8n.pt
#TODO weights=std_weights/yolov8n-seg.pt

# On our weights.
#TODO weights/fruits_s.pt

#TODO Camera source=video

#####
# Usefull.

yolo help

# Args.
yolo cfg
# https://docs.ultralytics.com/cfg

import os
import urllib
import traceback
import time
import datetime as dt
import sys
import numpy as np
import cv2
from rknnlite.api import RKNNLite
from PIL import Image, ImageDraw, ImageFont
import serial
 
 
#RKNN_MODEL = 'yolov5s-640-640.rknn'
RKNN_MODEL = 'best.rknn'
#DATASET = './dataset.txt'
 
QUANTIZE_ON = True
 
OBJ_THRESH = 0.25
NMS_THRESH = 0.45
IMG_SIZE = 640
 
'''CLASSES = ("person", "bicycle", "car", "motorbike ", "aeroplane ", "bus ", "train", "truck ", "boat", "traffic light",
           "fire hydrant", "stop sign ", "parking meter", "bench", "bird", "cat", "dog ", "horse ", "sheep", "cow", "elephant",
           "bear", "zebra ", "giraffe", "backpack", "umbrella", "handbag", "tie", "suitcase", "frisbee", "skis", "snowboard", "sports ball", "kite",
           "baseball bat", "baseball glove", "skateboard", "surfboard", "tennis racket", "bottle", "wine glass", "cup", "fork", "knife ",
           "spoon", "bowl", "banana", "apple", "sandwich", "orange", "broccoli", "carrot", "hot dog", "pizza ", "donut", "cake", "chair", "sofa",
           "pottedplant", "bed", "diningtable", "toilet ", "tvmonitor", "laptop	", "mouse	", "remote ", "keyboard ", "cell phone", "microwave ",
           "oven ", "toaster", "sink", "refrigerator ", "book", "clock", "vase", "scissors ", "teddy bear ", "hair drier", "toothbrush ")
'''
CLASSES = ("coke",)

ser = serial.Serial(
    port='/dev/ttyS2',       # 端口名
    baudrate=115200,     # 波特率
    parity=serial.PARITY_NONE,  # 奇偶校验位
    stopbits=serial.STOPBITS_ONE,  # 停止位
    bytesize=serial.EIGHTBITS,  # 数据位
    timeout=1          # 读超时（设置为1秒，便于循环中检查数据）
)
 
def sigmoid(x):
    return 1 / (1 + np.exp(-x))
 
 
def xywh2xyxy(x):
    # Convert [x, y, w, h] to [x1, y1, x2, y2]
    y = np.copy(x)
    y[:, 0] = x[:, 0] - x[:, 2] / 2  # top left x
    y[:, 1] = x[:, 1] - x[:, 3] / 2  # top left y
    y[:, 2] = x[:, 0] + x[:, 2] / 2  # bottom right x
    y[:, 3] = x[:, 1] + x[:, 3] / 2  # bottom right y
    return y
 
 
def process(input, mask, anchors):
 
    anchors = [anchors[i] for i in mask]
    grid_h, grid_w = map(int, input.shape[0:2])
 
    box_confidence = sigmoid(input[..., 4])
    box_confidence = np.expand_dims(box_confidence, axis=-1)
 
    box_class_probs = sigmoid(input[..., 5:])
 
    box_xy = sigmoid(input[..., :2])*2 - 0.5
 
    col = np.tile(np.arange(0, grid_w), grid_w).reshape(-1, grid_w)
    row = np.tile(np.arange(0, grid_h).reshape(-1, 1), grid_h)
    col = col.reshape(grid_h, grid_w, 1, 1).repeat(3, axis=-2)
    row = row.reshape(grid_h, grid_w, 1, 1).repeat(3, axis=-2)
    grid = np.concatenate((col, row), axis=-1)
    box_xy += grid
    box_xy *= int(IMG_SIZE/grid_h)
 
    box_wh = pow(sigmoid(input[..., 2:4])*2, 2)
    box_wh = box_wh * anchors
 
    box = np.concatenate((box_xy, box_wh), axis=-1)
 
    return box, box_confidence, box_class_probs
 
 
def filter_boxes(boxes, box_confidences, box_class_probs):
    """Filter boxes with box threshold. It's a bit different with origin yolov5 post process!
    # Arguments
        boxes: ndarray, boxes of objects.
        box_confidences: ndarray, confidences of objects.
        box_class_probs: ndarray, class_probs of objects.
    # Returns
        boxes: ndarray, filtered boxes.
        classes: ndarray, classes for boxes.
        scores: ndarray, scores for boxes.
    """
    boxes = boxes.reshape(-1, 4)
    box_confidences = box_confidences.reshape(-1)
    box_class_probs = box_class_probs.reshape(-1, box_class_probs.shape[-1])
 
    _box_pos = np.where(box_confidences >= OBJ_THRESH)
    boxes = boxes[_box_pos]
    box_confidences = box_confidences[_box_pos]
    box_class_probs = box_class_probs[_box_pos]
 
    class_max_score = np.max(box_class_probs, axis=-1)
    classes = np.argmax(box_class_probs, axis=-1)
    _class_pos = np.where(class_max_score >= OBJ_THRESH)
 
    boxes = boxes[_class_pos]
    classes = classes[_class_pos]
    scores = (class_max_score* box_confidences)[_class_pos]
 
    return boxes, classes, scores
 
 
def nms_boxes(boxes, scores):
    """Suppress non-maximal boxes.
    # Arguments
        boxes: ndarray, boxes of objects.
        scores: ndarray, scores of objects.
    # Returns
        keep: ndarray, index of effective boxes.
    """
    x = boxes[:, 0]
    y = boxes[:, 1]
    w = boxes[:, 2] - boxes[:, 0]
    h = boxes[:, 3] - boxes[:, 1]
 
    areas = w * h
    order = scores.argsort()[::-1]
 
    keep = []
    while order.size > 0:
        i = order[0]
        keep.append(i)
 
        xx1 = np.maximum(x[i], x[order[1:]])
        yy1 = np.maximum(y[i], y[order[1:]])
        xx2 = np.minimum(x[i] + w[i], x[order[1:]] + w[order[1:]])
        yy2 = np.minimum(y[i] + h[i], y[order[1:]] + h[order[1:]])
 
        w1 = np.maximum(0.0, xx2 - xx1 + 0.00001)
        h1 = np.maximum(0.0, yy2 - yy1 + 0.00001)
        inter = w1 * h1
 
        ovr = inter / (areas[i] + areas[order[1:]] - inter)
        inds = np.where(ovr <= NMS_THRESH)[0]
        order = order[inds + 1]
    keep = np.array(keep)
    return keep
 
 
def yolov5_post_process(input_data):
    masks = [[0, 1, 2], [3, 4, 5], [6, 7, 8]]
    anchors = [[116,90],[156,198],[373,326],[30,61],[62,45],[59,119],[194,482],[205,532],[216,544]]
    boxes, classes, scores = [], [], []
    for input, mask in zip(input_data, masks):
        b, c, s = process(input, mask, anchors)
        b, c, s = filter_boxes(b, c, s)
        boxes.append(b)
        classes.append(c)
        scores.append(s)
 
    boxes = np.concatenate(boxes)
    boxes = xywh2xyxy(boxes)
    classes = np.concatenate(classes)
    scores = np.concatenate(scores)
 
    nboxes, nclasses, nscores = [], [], []
    for c in set(classes):
        inds = np.where(classes == c)
        b = boxes[inds]
        c = classes[inds]
        s = scores[inds]
 
        keep = nms_boxes(b, s)
 
        nboxes.append(b[keep])
        nclasses.append(c[keep])
        nscores.append(s[keep])
 
    if not nclasses and not nscores:
        return None, None, None
 
    boxes = np.concatenate(nboxes)
    classes = np.concatenate(nclasses)
    scores = np.concatenate(nscores)
 
    return boxes, classes, scores
 
 
def draw(image, boxes, scores, classes, fps):
    """Draw the boxes on the image.
    # Argument:
        image: original image.
        boxes: ndarray, boxes of objects.
        classes: ndarray, classes of objects.
        scores: ndarray, scores of objects.
        fps: int.
        all_classes: all classes name.
    """
    for box, score, cl in zip(boxes, scores, classes):
        top, left, right, bottom = box
        print('class: {}, score: {}'.format(CLASSES[cl], score))
        print('box coordinate left,top,right,down: [{}, {}, {}, {}]'.format(top, left, right, bottom))
        top = int(top)
        left = int(left)
        right = int(right)
        bottom = int(bottom)
 
        cv2.rectangle(image, (top, left), (right, bottom), (255, 0, 0), 2)
        cv2.putText(image, '{0} {1:.2f}'.format(CLASSES[cl], score),
                    (top, left - 6),
                    cv2.FONT_HERSHEY_SIMPLEX,
                    0.6, (0, 0, 255), 2)
 
def letterbox(im, new_shape=(640, 640), color=(0, 0, 0)):
    # Resize and pad image while meeting stride-multiple constraints
    shape = im.shape[:2]  # current shape [height, width]
    if isinstance(new_shape, int):
        new_shape = (new_shape, new_shape)
 
    # Scale ratio (new / old)
    r = min(new_shape[0] / shape[0], new_shape[1] / shape[1])
 
    # Compute padding
    ratio = r, r  # width, height ratios
    new_unpad = int(round(shape[1] * r)), int(round(shape[0] * r))
    dw, dh = new_shape[1] - new_unpad[0], new_shape[0] - new_unpad[1]  # wh padding
 
    dw /= 2  # divide padding into 2 sides
    dh /= 2
 
    if shape[::-1] != new_unpad:  # resize
        im = cv2.resize(im, new_unpad, interpolation=cv2.INTER_LINEAR)
    top, bottom = int(round(dh - 0.1)), int(round(dh + 0.1))
    left, right = int(round(dw - 0.1)), int(round(dw + 0.1))
    im = cv2.copyMakeBorder(im, top, bottom, left, right, cv2.BORDER_CONSTANT, value=color)  # add border
    return im, ratio, (dw, dh)

os.chdir(os.path.dirname(__file__))
 
def showChinese(img, text, position, textColor, textSize):
    if isinstance(img, np.ndarray):
        img = Image.fromarray(cv2.cvtColor(img, cv2.COLOR_BGR2RGB))
        draw = ImageDraw.Draw(img)
        fontStyle = ImageFont.truetype("simhei.ttf", textSize, encoding="UTF=8")
        draw.text(position, text, textColor, font=fontStyle)
        return cv2.cvtColor(np.asarray(img), cv2.COLOR_RGB2BGR)

# ==================================
# 如下为改动部分，主要就是去掉了官方 demo 中的模型转换代码，直接加载 rknn 模型，并将 RKNN 类换成了 rknn_toolkit2_lite 中的 RKNNLite 类
# ==================================
 
rknn = RKNNLite()
 
# load RKNN model
print('--> Load RKNN model')
ret = rknn.load_rknn(RKNN_MODEL)
 
# Init runtime environment
print('--> Init runtime environment')
# use NPU core 0 1 2
ret = rknn.init_runtime()
if ret != 0:
    print('Init runtime environment failed!')
    exit(ret)
print('done')
 
# Create a VideoCapture object and read from input file
# If the input is the camera, pass 0 instead of the video file name
cap = cv2.VideoCapture(0)

# Check if camera opened successfully
if not cap.isOpened(): 
    print("Error opening video stream or file")

order = 0

# Read until video is completed
while cap.isOpened():
    start = dt.datetime.utcnow()
    # Capture frame-by-frame
    ret, img = cap.read()
    if not ret:
        break
    

    img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    img = cv2.resize(img, (IMG_SIZE, IMG_SIZE))
    img = np.expand_dims(img, axis=0)  # Add batch dimension

    # Inference
    outputs = rknn.inference(inputs=[img])

    # post process
    if outputs is None or len(outputs) < 3:
        print("Error: Inference outputs are not available or insufficient")
        continue
    
    input0_data = outputs[0]
    input1_data = outputs[1]
    input2_data = outputs[2]

    input0_data = input0_data.reshape([3, -1] + list(input0_data.shape[-2:]))
    input1_data = input1_data.reshape([3, -1] + list(input1_data.shape[-2:]))
    input2_data = input2_data.reshape([3, -1] + list(input2_data.shape[-2:]))

    input_data = [
        np.transpose(input0_data, (2, 3, 0, 1)),
        np.transpose(input1_data, (2, 3, 0, 1)),
        np.transpose(input2_data, (2, 3, 0, 1))
    ]

    boxes, classes, scores = yolov5_post_process(input_data)
    duration = dt.datetime.utcnow() - start
    fps = round(1000000 / duration.microseconds)

    detected = False

    # draw process result and fps
    img_1 = cv2.cvtColor(img[0], cv2.COLOR_RGB2BGR)  # Remove batch dimension for display
    cv2.putText(img_1, f'fps: {fps}', (20, 20), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 125, 125), 2)
    if boxes is not None:
        draw(img_1, boxes, scores, classes, fps)
        detected = True
    
    if detected:
        # time.sleep(3)
        boxes, classes, scores = yolov5_post_process(input_data)
        number = len(boxes)
        if len(set(classes)) == 1:
            detected_classes = [CLASSES[cls] for cls in classes]
            if "coke" in detected_classes:
                message = "1111"
                ser.write(message.encode('utf-8'))
                order += 1
                screen_width, screen_height = 1920,1080
                img = 255*np.ones((screen_height,screen_width,3), dtype = np.uint8)
                center_x, center_y = screen_width//2, screen_height//2
                text = f"{order} 可回收垃圾 {number}"
                font = cv2.FONT_HERSHEY_SIMPLEX
                font_scale = 3.5
                font_thickness = 3
                text_size, _ = cv2.getTextSize(text, font,font_scale,font_thickness)
                text_x = center_x - text_size[0]//2
                text_y = center_y - text_size[1]//2
                image = showChinese(img, text, (text_x, text_y), (0, 0, 0), 150)
                cv2.namedWindow('Img', cv2.WND_PROP_FULLSCREEN)
                cv2.setWindowProperty('Img', cv2.WND_PROP_FULLSCREEN, cv2.WINDOW_FULLSCREEN)
                cv2.imshow('Img', image)
                data = ser.read(ser.in_waiting)  # 读取所有等待中的数据
                decode_data = data.decode('utf-8')
                while 1:
                    if cv2.waitKey(25) & 0xFF == ord('q'):
                        break
                    cv2.imshow('Img', image)
                    data = ser.read(ser.in_waiting)  # 读取所有等待中的数据
                    if b'\x01' in data:
                        text = f"{order} 可回收垃圾 {number} OK!"
                        font = cv2.FONT_HERSHEY_SIMPLEX
                        font_scale = 3.5
                        font_thickness = 3
                        text_size, _ = cv2.getTextSize(text, font,font_scale,font_thickness)
                        text_x = center_x - text_size[0]//2
                        text_y = center_y - text_size[1]//2
                        image = showChinese(img, text, (text_x, text_y), (0, 0, 0), 150)
                        cv2.namedWindow('Img', cv2.WND_PROP_FULLSCREEN)
                        cv2.setWindowProperty('Img', cv2.WND_PROP_FULLSCREEN, cv2.WINDOW_FULLSCREEN)
                        cv2.imshow('Img', image)
                        break

    if ser.in_waiting > 0:  # 检查是否有数据在等待
            data = ser.read(ser.in_waiting)  # 读取所有等待中的数据
            # 检测特定字符串
            if b'\x02' in data:
                screen_width, screen_height = 1920,1080
                img = 255*np.ones((screen_height,screen_width,3), dtype = np.uint8)
                center_x, center_y = screen_width//2, screen_height//2
                text = "满载提示"
                font = cv2.FONT_HERSHEY_SIMPLEX
                font_scale = 3.5
                font_thickness = 3
                text_size, _ = cv2.getTextSize(text, font,font_scale,font_thickness)
                text_x = center_x - text_size[0]//2
                text_y = center_y - text_size[1]//2
                image = showChinese(img, text, (text_x, text_y), (0, 0, 0), 150)
                cv2.namedWindow('Img', cv2.WND_PROP_FULLSCREEN)
                cv2.setWindowProperty('Img', cv2.WND_PROP_FULLSCREEN, cv2.WINDOW_FULLSCREEN)
                cv2.imshow('Img', image)

    # show output
    cv2.imshow("post process result", img_1)

    # Press Q on keyboard to  exit
    if cv2.waitKey(25) & 0xFF == ord('q'):
        break

# When everything done, release the video capture object
cap.release()

# Closes all the frames
cv2.destroyAllWindows()

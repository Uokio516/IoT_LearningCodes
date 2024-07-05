#https://twgo.io/dgnab
import cv2, numpy as np
from ultralytics import YOLO
from shapely.geometry import Polygon

#路徑字典：物件編號+移動路徑座標
from collections import defaultdict
track_history = defaultdict(lambda: [])

#區域三維陣列
area=[
    [[1021, 62],[1203, 62],[1448, 841],[1088, 853]], #區域1
    [[1455, 138],[1703, 129],[1912, 547],[1912, 807],[1812, 823],[1803, 707]], #區域2
    ]


#繪製區域   影像,區域座標,顏色,寬度
def drawArea(f,area,color,th):
    for a in area:
        v =  np.array(a, np.int32)
        cv2.polylines(f, [v], isClosed=True, color=color, thickness=th)
    return f

#取得重疊比例
def inarea(object,area):
    inAreaPercent=[] #area陣列，物件在所有區域的比例
    b=[[object[0],object[1]],[object[2],object[1]],[object[2],object[3]],[object[0],object[3]]]
    for i in range(len(area)):        
        poly1 = Polygon(b)
        poly2 = Polygon(area[i])
        intersection_area = poly1.intersection(poly2).area #重疊區域部分
        poly1Area = poly1.area #物件區域        
        #union_area = poly1.union(poly2).area
        overlap_percent = (intersection_area / poly1Area) * 100
        inAreaPercent.append(overlap_percent)
    return inAreaPercent


model = YOLO("yolov8m.pt")
names = model.names

cv2.namedWindow('YOLOv8', cv2.WINDOW_NORMAL)
target = "python\Counter.mp4"
cap = cv2.VideoCapture(target)

#錄影機一定要在cap被宣告後才可啟動
w, h, fps = (int(cap.get(x)) for x in (cv2.CAP_PROP_FRAME_WIDTH, cv2.CAP_PROP_FRAME_HEIGHT, cv2.CAP_PROP_FPS))
recorder= cv2.VideoWriter("recorder.mp4", #檔名
                       cv2.VideoWriter_fourcc(*'mp4v'), #格式
                       fps, #FPS
                       (w, h)) #解析度
#來客追蹤編號清單
customerList=[]

while cap.isOpened():
    r, frame = cap.read()
    #成功讀取影像
    if r==False:
        break
        #       啟動辨識+追蹤      持續          不顯示結果
    results = model.track(frame, persist=True, verbose=False)
        #三個區域分開不同顏色
    frame = drawArea(frame,[area[0]],(255,0,0),3)
    frame = drawArea(frame,[area[1]],(0,255,0),3)

    #至少有一個物件
    if results[0].boxes.id is not None:      
        LinePersonCount=[0,0]
        for box in results[0].boxes.data:
            x1 = int(box[0]) #左
            y1 = int(box[1]) #上
            x2 = int(box[2]) #右
            y2 = int(box[3]) #下
            trackid = int(box[4]) #每個物件的唯一追蹤編號
            r = round(float(box[5]),2) #信任度
            n = names[int(box[6])] #名字

            #只追蹤人(ClassID=0)

            if n in ['person']:
                # 人位於結帳區0,1內
                tempObj=[x1,y1,x2,y2,trackid,r,n] #組回物件屬性
                ObjInArea = inarea(tempObj,area) #計算物件與區域重疊比例

                #結帳區0
                if ObjInArea[0]>=25:
                    cv2.rectangle(frame,(int(box[0]), int(box[1])), (int(box[2]),int(box[3])), (255, 0, 0),5)

                    #找出物件的移動軌跡座標
                    track = track_history[trackid]

                    #加一個點，物件的中心點為軌跡座標
                    track.append((int((x1 + x2) / 2), int((y1 + y2) / 2)))

                    #只追蹤30個點，約一秒，以fps=30
                    if len(track) > 100:
                        track.pop(0) #刪除最早的那個軌跡

                    # 軌跡List轉換成陣列                      重分配陣列(-1：由系統指定)
                    points = np.array(track, dtype=np.int32).reshape((-1, 1, 2))
                    #現在位置     倒數第一個軌跡座標,半徑,顏色,   填滿(正:外框寬度,負:填滿)
                    cv2.circle(frame, (track[-1]), 7, (0,0,255), -1)
                    #移動路徑                        不封閉:頭尾相接     顏色            寬度
                    cv2.polylines(frame, [points], isClosed=False, color=(0,0,255), thickness=2)
                    #寫上物件追蹤編號
                    cv2.putText(frame,'person#' + str(trackid),(x1,y1-10),cv2.FONT_HERSHEY_PLAIN,2,(255,0,0),2)
                    LinePersonCount[0] += 1
                    #把追蹤編號加入客戶清單
                    if trackid not in customerList:
                        customerList.append(trackid)
                #結帳區1
                if ObjInArea[1]>=25:
                    cv2.rectangle(frame,(int(box[0]), int(box[1])), (int(box[2]),int(box[3])), (0, 255, 0),5)

                    #找出物件的移動軌跡座標
                    track = track_history[trackid]

                    #加一個點，物件的中心點為軌跡座標
                    track.append((int((x1 + x2) / 2), int((y1 + y2) / 2)))

                    #只追蹤30個點，約一秒，以fps=30
                    if len(track) > 100:
                        track.pop(0) #刪除最早的那個軌跡

                    # 軌跡List轉換成陣列                      重分配陣列(-1：由系統指定)
                    points = np.array(track, dtype=np.int32).reshape((-1, 1, 2))
                    #現在位置     倒數第一個軌跡座標,半徑,顏色,   填滿(正:外框寬度,負:填滿)
                    cv2.circle(frame, (track[-1]), 7, (0,0,255), -1)
                    #移動路徑                        不封閉:頭尾相接     顏色            寬度
                    cv2.polylines(frame, [points], isClosed=False, color=(0,0,255), thickness=2)
                    #寫上物件追蹤編號
                    cv2.putText(frame,'person#' + str(trackid),(x1,y1-10),cv2.FONT_HERSHEY_PLAIN,2,(0,255,0),2)
                    LinePersonCount[1] += 1
                   
                    #把追蹤編號加入客戶清單
                    if trackid not in customerList:
                        customerList.append(trackid)
    #左上角寫上現在結帳人數
    cv2.putText(frame,'Line0:' + str(LinePersonCount[0]),(10,50),cv2.FONT_HERSHEY_PLAIN,3,(255,0,0),2)
    cv2.putText(frame,'Line1:' + str(LinePersonCount[1]),(10,100),cv2.FONT_HERSHEY_PLAIN,3,(0,255,0),2)
    #目前總來客數
    cv2.putText(frame,'Total:' + str(len(customerList)),(10,150),cv2.FONT_HERSHEY_PLAIN,3,(0,255,255),2)
    cv2.imshow('YOLOv8',frame)
    recorder.write(frame)       # 將取得的每一幀圖像寫入空的影片

    if cv2.waitKey(1) == 27: #ESC退出
        break

recorder.release()      # 釋放資源






#測試影片：https://twgo.io/vlpwc
#tw live可搜尋台灣即時影像：https://trafficvideo2.tainan.gov.tw/b596d902

from ultralytics import YOLO
import cv2,time,numpy as np
from shapely.geometry import Polygon
#設定視窗名稱及型態
cv2.namedWindow('YOLOv8', cv2.WINDOW_NORMAL)

target='python\Counter.mp4'
model = YOLO('yolov8m.pt')  # 預設模型：n,s,m,l,x 五種大小

names=model.names #認識的80物件 字典：編號及名稱
print(names)

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



cap=cv2.VideoCapture(target)

while 1:
    st=time.time()  
    r,frame = cap.read()
    if r==False: #讀取失敗
        break
    results = model(frame, verbose=False) #YOLO辨識verbose=False不顯示文字結果
    # frame= results[0].plot() #畫出辨識結果，[0]第一張照片

    #三個區域分開不同顏色
    frame = drawArea(frame,[area[0]],(255,0,0),3)
    frame = drawArea(frame,[area[1]],(0,255,0),3)

    personCount=[0,0] #初始汽車數量
    for box in results[0].boxes.data:
        x1 = int(box[0]) #左
        y1 = int(box[1]) #上
        x2 = int(box[2]) #右
        y2 = int(box[3]) #下
        r = round(float(box[4]),2) #信任度
        n = names[int(box[5])] #名字
        if n != 'person':
            continue #下一個
        # 進入區域才畫框
        # 計算物件是否進入區域
        tempObj = [x1,y1,x2,y2,r,n]
        # 計算物件與區域的重疊比例    物件,區域
        ObjInArea = inarea(tempObj,area)
        #物件在區域0的比例>=25
        #區域0 藍色
        if ObjInArea[0]>=25:
            # 自己畫外框   影像   左上角   右下角
            cv2.rectangle(frame, (x1,y1), (x2,y2),(255,0,0),2 )
            # 寫上物件名稱
            cv2.putText(frame, n, (x1, y1), cv2.FONT_HERSHEY_PLAIN, 1, (255, 0, 0), 2, cv2.LINE_AA)
            personCount[0]+=1
        #區域1 綠色
        if ObjInArea[1]>=25:
            # 自己畫外框   影像   左上角   右下角
            cv2.rectangle(frame, (x1,y1), (x2,y2),(0,255,0),2 )
            # 寫上物件名稱
            cv2.putText(frame, n, (x1, y1), cv2.FONT_HERSHEY_PLAIN, 1, (0, 255, 0), 2, cv2.LINE_AA)
            personCount[1]+=1


    #印出汽車總量
    print(personCount)
    #區域0
    cv2.putText(frame, 'Area0=' + str(personCount[0]), (20, 80), cv2.FONT_HERSHEY_PLAIN, 2, (255, 0, 0), 2, cv2.LINE_AA)
    #區域1
    cv2.putText(frame, 'Area1=' + str(personCount[1]), (20, 110), cv2.FONT_HERSHEY_PLAIN, 2, (0, 255, 0), 2, cv2.LINE_AA)
   
    et=time.time()  
    FPS=round((1/(et-st)),1)
    #在畫面寫字  影像       文字內容       位置(x,y)     字型               大小  顏色(BGR)   粗細   樣式
    cv2.putText(frame, 'FPS=' + str(FPS), (20, 50), cv2.FONT_HERSHEY_PLAIN, 2, (255, 0, 0), 2, cv2.LINE_AA)
    cv2.imshow('YOLOv8', frame)
    key=cv2.waitKey(1)
    if key==27:
        break





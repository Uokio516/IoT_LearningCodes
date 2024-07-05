
#https://twgo.io/xpuka
#測試影片：https://twgo.io/vlpwc
#tw live可搜尋台灣即時影像：https://trafficvideo2.tainan.gov.tw/b596d902

from ultralytics import YOLO
import cv2,time
#設定視窗名稱及型態
cv2.namedWindow('YOLOv8', cv2.WINDOW_NORMAL)#設定這行目的是為了轉換為windows傳統視窗，使其視窗可以放大縮小

target='python\city.mp4'
model = YOLO('yolov8l.pt')  # pretrained YOLOv8m model
#認識的物件名（字典：編號可以查詢名稱）
names=model.names

#影像擷取裝置
cap=cv2.VideoCapture(target)

while 1:
    try:
        st=time.time()  #取得開始時間
        r,frame = cap.read() #r=影像擷取成功?，frame=影像內容
        results = model(frame,verbose=False) # 用yolo model分析frame，得到結果存在results
        #自己做分析


        CarCount = 0
        TruckCount = 0
        BusCount = 0


        for box in results[0].boxes.data:
            x1 = int(box[0])
            y1 = int(box[1])
            x2 = int(box[2])
            y2 = int(box[3])
            r = float(box[4])
            name = names[int(box[5])]
            #print(x1,y1,x2,y2,r,name)
            #if name == 'car' or name == 'bus' or name == 'truck' :
            if name in ['car','truck','bus']:
                cv2.rectangle(frame,(x1,y1),(x2,y2),(0,0,225),2)
                cv2.putText(frame, name, (x1,y1), cv2.FONT_HERSHEY_PLAIN, 1, (0, 255, 0), 2, cv2.LINE_AA)
            if(name=='car'):
                CarCount +=1
            if(name=='truck'):
                TruckCount +=1
            if(name=='bus'):
                BusCount +=1
        cv2.putText(frame, 'Car=' + str(CarCount), (20, 50), cv2.FONT_HERSHEY_PLAIN, 3, (0, 0, 255), 2, cv2.LINE_AA)
        cv2.putText(frame, 'Truck=' + str(TruckCount), (20, 85), cv2.FONT_HERSHEY_PLAIN, 3, (0, 0, 255), 2, cv2.LINE_AA)
        cv2.putText(frame, 'Bus=' + str(TruckCount), (20, 120), cv2.FONT_HERSHEY_PLAIN, 3, (0, 0, 255), 2, cv2.LINE_AA)
        #frame= results[0].plot() #把發現的結果繪圖出來覆蓋frame，results[0]=第1個結果=第1張照片
        #用plot畫出結果[x1,y1,x2,y2,r,name]
        #計算車輛數 results[0].boxes.data
        
        carCount=0
        for obj in results[0].boxes.data:
            x1,y1,x2,y2=int(obj[0]),int(obj[1]),int(obj[2]),int(obj[3])
            r=round(float(obj[4]),2)
            n=names[int(obj[5])]
            if n=='car':
                carCount=carCount+1
            print(x1,y1,x2,y2,r,n)
        print('carCount=',carCount)        
       
        et=time.time()
       
        FPS=round((1/(et-st)),1) #評估時間
        #            圖         文字內容        座標            字型             大小    顏色BGR   粗細   樣式(實心)
        #putText() 在畫面上寫字
        #cv2.putText(frame, 'FPS=' + str(FPS), (20, 150), cv2.FONT_HERSHEY_PLAIN, 3, (0, 255, 255), 2, cv2.LINE_AA)
        #cv2.putText(frame, 'Car=' + str(carCount), (20, 50), cv2.FONT_HERSHEY_PLAIN, 3, (0, 0, 255), 2, cv2.LINE_AA)
        cv2.imshow('YOLOv8', frame)#顯示影像 cv2.imshow(視窗名稱，影像內容)
        key=cv2.waitKey(1)#等候使用者按鍵盤 *等候1/1000秒
        if key==27: #27=ESC鍵
            break
    except Exception as e :
        print(e)
        break




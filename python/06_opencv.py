
#twgo.io/pgibn
import cv2,numpy as np
#設定視窗名稱及型態
cv2.namedWindow('YOLOv8', cv2.WINDOW_NORMAL)
cap=cv2.VideoCapture(0)

area=[
    [[10,10],[50,10],[50,50],[10,50]], #區域1
    [[390,5],[590,5] ,[590,307],[590,460],[320,280]], #區域2
    ]

#繪製區域
def drawArea(f,area,color,th):
    for a in area:
        v =  np.array(a, np.int32)
        cv2.polylines(f, [v], isClosed=True, color=color, thickness=th)
    return f

 

while 1:
    r,frame=cap.read() #讀取一張影像
    frame = drawArea(frame,area,(0,0,255),3)
    cv2.imshow('YOLOv8',frame) #顯示影像
    key=cv2.waitKey(1) #使用者按了鍵盤
    if key==27: #27代表鍵盤的ESC
        break   #退出迴圈





#https://twgo.io/tpbts
import cv2
#設定視窗名稱及型態
cv2.namedWindow('YOLOv8', cv2.WINDOW_NORMAL)
cap=cv2.VideoCapture(0)

while 1:
    r,frame=cap.read() #讀取一張影像
    cv2.imshow('YOLOv8',frame) #顯示影像
    key=cv2.waitKey(1) #使用者按了鍵盤
    if key==27: #27代表鍵盤的ESC
        break   #退出迴圈


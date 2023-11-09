import datetime
import math
import cv2
import time
import numpy as np
import pandas as pd
import PoseModule as PM
import FaceMeshModule as FM

cap = cv2.VideoCapture(r"C:\Users\sampr\Desktop\check\Sleeping-Baby-Pose-Detection\PoseVideos\1.mp4")
detector = PM.poseDetector()
detector2 = FM.FaceMeshDetector()

frame = 0
pPer1 = 0
pPer2 = 0
pState = 0
state = ""
pTime = 0
tipIds = [15, 16]
sleep_start_time = None
total_sleep_time = 0
sleep_durations = []  # List to store sleep durations

while True:
    if frame % 30 == 0:
        success, img = cap.read()

        img1 = detector.findPose(img, False)
        img2, face = detector2.findFaceMesh(img, False)
        lmList = detector.findPosition(img1, False)

        bodyImg = detector.findBody(img1)
        detector2.drawEyes(img)

        if len(lmList) != 0:
            angle1 = detector.findAngle(img, 24, 26, 28)
            angle2 = detector.findAngle(img, 23, 25, 27)
            per1 = np.interp(angle1, (120, 150), (0, 100))
            per2 = np.interp(angle1, (240, 210), (0, 100))

            legLength1 = math.sqrt((lmList[24][1] - lmList[28][1])**2 + (lmList[24][2] - lmList[28][2])**2)
            leghold1 = 3 * math.sqrt((lmList[12][1] - lmList[24][1])**2 + (lmList[12][2] - lmList[24][2])**2) / 5
            legLength2 = math.sqrt((lmList[24][1] - lmList[28][1])**2 + (lmList[24][2] - lmList[28][2])**2)
            leghold2 = 3 * math.sqrt((lmList[12][1] - lmList[24][1])**2 + (lmList[12][2] - lmList[24][2])**2) / 5

            if per1 != 100:
                pPer1 = 0
            if per1 == 100 and pPer1 == 0 and legLength1 > leghold1:
                print(state, ",", "kick leg", ",", datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S'))
                cv2.putText(img1, str("kick leg"), (lmList[26][1] - 10, lmList[26][2]), cv2.FONT_HERSHEY_PLAIN, 3,
                            (0, 255, 0), 4)
                pPer1 = 1

            if per2 != 100:
                pPer2 = 0
            if per2 == 100 and pPer2 == 0 and legLength2 > leghold2:
                print(state, ",", "kick leg", ",", datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S'))
                cv2.putText(img1, str("kick leg"), (lmList[25][1] + 10, lmList[25][2]), cv2.FONT_HERSHEY_PLAIN, 3,
                            (0, 255, 0), 4)
                pPer2 = 1

            ix1 = min(lmList[7][1], lmList[8][1])
            ix2 = max(lmList[7][1], lmList[8][1])

            eyehold = math.sqrt((face[33][1] - face[155][1])**2 + (face[33][2] - face[155][2])**2)
            leftEyeopen = math.sqrt((face[159][1] - face[144][1])**2 + (face[159][2] - face[144][2])**2)
            rightEyeopen = math.sqrt((face[385][1] - face[374][1])**2 + (face[385][2] - face[374][2])**2)

            if (lmList[15][1] > ix1 and lmList[15][1] < ix2) or (lmList[16][1] > ix1 and lmList[16][1] < ix2):
                cv2.putText(img1, str("touch face"), (lmList[7][1], lmList[7][2]), cv2.FONT_HERSHEY_PLAIN, 2, (255, 0, 255), 2)
            elif leftEyeopen > eyehold / 3 or rightEyeopen > eyehold / 3:
                state = "awake"
            else:
                state = "sleep"

        if state == "sleep":
            if sleep_start_time is None:
                sleep_start_time = time.time()
        else:
            if sleep_start_time is not None:
                sleep_end_time = time.time()
                sleep_duration = sleep_end_time - sleep_start_time
                if 0 < sleep_duration <= 3600:  # within one hour
                    total_sleep_time += sleep_duration
                    print("Total sleep time within the last one hour:", total_sleep_time, "seconds")
                    sleep_durations.append(sleep_duration)  # Append sleep duration to the list
                sleep_start_time = None

        if state == "awake":
            pState = 0

        if state == "awake" and pState == 0:
            print(state, ",", datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S'))
            pState = 1

        cTime = time.time()
        fps = 1 / (cTime - pTime)
        pTime = cTime
        Mytime = datetime.datetime.now()

        text_position = (int(face[10][1]), int(face[10][2]))
        cv2.putText(img1, str(state), text_position, cv2.FONT_HERSHEY_PLAIN, 3, (0, 255, 0), 3)

        cv2.putText(img, f'FPS:{int(fps)}', (10, 70), cv2.FONT_HERSHEY_PLAIN, 2, (255, 0, 255), 2)
        cv2.putText(img, str(Mytime), (5, 30), cv2.FONT_HERSHEY_PLAIN, 1, (0, 255, 0), 1)
        cv2.imshow("Image", img)
        cv2.waitKey(1)
        frame = 0

    if frame % 900 == 0:  # Check every 900 frames (30 seconds)
        # Convert sleep durations to a pandas DataFrame
        sleep_df = pd.DataFrame({"Sleep Durations": sleep_durations})
        
        # Save the DataFrame to an Excel file
        sleep_df.to_excel(r"C:\Users\sampr\Desktop\Sleeping-Baby-Pose-Detection-main\Sleeping-Baby-Pose-Detection-main\sleep.xlsx", index=False)

    frame += 1

# Release the video capture and close all windows
cap.release()
cv2.destroyAllWindows()


















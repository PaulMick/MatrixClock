import numpy as np
import cv2

width = 5
height = 7
frames = 6

fname = "wifi_connecting"
cname = "WIFI_CONNECTING"

print(f"static int {cname}_WIDTH = {width};")
print(f"static int {cname}_HEIGHT = {height};")
print(f"static int {cname}_FRAMES = {frames};")
print(f"static uint8_t {cname}_IMG[{frames}][{height}][{width}][3] = {'{'}")
for f in range(frames):
    cv_im = cv2.imread(f"misc_python/image_input/{fname}/{f}.png")
    np_im = np.asarray(cv_im)

    print("    {")
    for y in range(height):
        print("        {", end = "")
        for x in range(width):
            r = int(np_im[x, y, 0])
            g = int(np_im[x, y, 0])
            b = int(np_im[x, y, 0])
            print(f"{'{'}{r:#0{2}x}, {g:#0{2}x}, {b:#0{2}x}{'}'}, ", end = "")
        print("},")
    print("    },")
print("};")
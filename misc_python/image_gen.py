import numpy as np
import cv2

width = 5
height = 6

fname = "wifi_connected"
cname = "WIFI_CONNECTED"

cv_im = cv2.imread(f"misc_python/image_input/{fname}.png")
np_im = np.asarray(cv_im)

print(f"static int {cname}_WIDTH = {width};")
print(f"static int {cname}_HEIGHT = {height};")
print(f"static uint8_t {cname}_IMG[{height}][{width}][3] = {'{'}")
for y in range(height):
    print("    {", end = "")
    for x in range(width):
        r = int(np_im[y * 2, x * 2, 0])
        g = int(np_im[y * 2, x * 2, 1])
        b = int(np_im[y * 2, x * 2, 2])
        print(f"{'{'}{r:#0{2}x}, {g:#0{2}x}, {b:#0{2}x}{'}'}, ", end = "")
    print("},")
print("};")